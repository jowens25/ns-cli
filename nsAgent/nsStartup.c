/*****************************************************************************
 * Project			: nsAgent
 * Author			: Bryan Wilcutt bwilcutt@yahoo.com
 * Date				: 5-13-18
 * System			: Nano PI
 * File				: nsStartup.c
 *
 * Description		:
 *
 * This file contains the start up and management code of the nsAgent application.
 * This applicate reads NMEA data, PPS and Radio Data, extending that data to
 * an SNMP interface while sending NMEA/PPS to NTPv4.
 *
 * Written for Novus Power.
 *
 * Copyright (c) Novus Power All Rights Reserved
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <linux/kernel.h>
#include <ctype.h>
#include <syslog.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/shm.h>



#include "novusAgent.h"
#include "nsRadioSer.h"
#include "nsStrParser.h"
#include "nsStartup.h"
#include "nsTrap.h"

#define NOVUS_NAME "Novus-Agent"
#define MAX_BUFFER_SIZE (256)

// Local prototypes

void *radioTask(void *vptr);
int getSerLine(int fd, char *buf, int buflen);
char* rtrim(char* string);
char* ltrim(char *string); 
int agentConfigFile();
int getrval(char *, char *);
int getlval(char *, char *);
void init_defaults();
void writeRadio(char *str);

// Trigger to kill radio task.

static int keep_running = 0;

// Global shared variables
//
// Set in nsagent.conf file, 1 = print debug info, 0 = no debug info

float ps_temp_high = DEF_TEMP_HIGH;
float ps_temp_low = DEF_TEMP_LOW;

// Config option fields
int dbg 						= 0; // Debug output on or off (default off)
int slog 						= 0; // Use syslog on or off (Default off)
int traps 						= 1; // Use traps, on or off (Default on)
int agentx_subagent				= 1; // 1 = Be an SNMP server, 0 = Be an SNMP agent

char nmeaName[DEVICE_NAME_LEN];		// Radio NMEA source port/device
char nr2316Name[DEVICE_NAME_LEN];	// Radio data source port/device

char commandResultStr[RESULT_MAX_LEN];

// Global file desciptor of opened radio/port device

int radio_fd = -1;

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

//SHM
int fd_shm = -1;
char* shm_data = NULL;
int shm_id = -1;

RETSIGTYPE stop_server(int a)
{
    keep_running = 0;
}

/************************************************
* Function       : daemonize
* Input          : None
* Output         : None
* Description    : Daemonizes nsAgent.
************************************************/
static void daemonize()
{
    pid_t pid, sid;
    int fd;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS); /*Killing the Parent Process*/
    }

    /* At this point we are executing as the child process */

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory. */
    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }


    fd = open("/dev/null",O_RDWR, 0);

//    if (fd != -1)
    if (fd < 0)
    {
        dup2 (fd, STDIN_FILENO);
        dup2 (fd, STDOUT_FILENO);
        dup2 (fd, STDERR_FILENO);

        if (fd > 2)
        {
            close (fd);
        }
    }

    /*resetting File Creation Mask */
    umask(027);
}

/************************************************
* Function       : main
* Input          : int argc - unused
*                  char **argv unused
* Output         : Zero
* Description    : This is the main start function
*	for the nsAgent application.  It allocates
*	necessary resources, starts SNMP, and begins
*	merging NMEA/Radio data to the SNMP engine.
************************************************/
int main (int argc, char **argv)
{
  int background = 0; /* change this if you want to run in the background */
  pthread_t serthread;

  if (argc == 2)
  {
	  if (strcmp(argv[1], "-d") == 0) {
		  /* Detach from the real world and wonder off into daemon-land. */

		  daemonize();
	  } else {
		  printf("\n\nNovus SNMP Agent\n\nby Bryan Wilcutt\n5-19-18  v1.0\n\n");
		  printf("Usage: nsAgent {-d}\n\n-d   - Daemonize agent\n\n");
		  return 0;
	  }
  } else if (argc > 2) {
	  printf("\nType nsAgent --help for usage.\n");
	  return 0;
  }

  commandResultStr[0] = 0;

  /* Seek out and handle the configuration file located at: /etc/nsagent.conf */

  if (!agentConfigFile())
  {
	if (dbg)
	{
		syslog(LOG_INFO, "AgentX = %d\n", agentx_subagent);
		syslog(LOG_INFO, "SysLog = %d\n", slog);
		syslog(LOG_INFO, "Debug = %d\n", dbg);
		syslog(LOG_INFO, "NMEA Dev = %s\n", nmeaName);
		syslog(LOG_INFO, "NR2316 Dev = %s\n", nr2316Name);
	}

	  /* print log errors to syslog or stderr */
	  if (slog)
	    snmp_enable_calllog();
	  else
	    snmp_enable_stderrlog();

	  netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1);

	  /* run in background, if requested */
	  if (background && netsnmp_daemonize(1, !slog))
	      exit(1);

	  /* initialize tcpip, if necessary */
	  SOCK_STARTUP;

	  /* Init data at startup to defaults */

	  init_defaults();

	  /* initialize the agent library */

	  init_agent(NOVUS_NAME);

	  /* initialize mib code here */

	  init_novus();  

	  init_snmp(NOVUS_NAME);
	  init_notification();

	  /* If we're going to be a snmp master agent, initial the ports */
	  if (!agentx_subagent) {
	    init_master_agent();  /* open the port to listen on (defaults to udp:161) */
	  }

	  /* In case we receive a request to stop (kill -TERM or kill -INT) */
	  keep_running = 1;
	  signal(SIGTERM, stop_server);
	  signal(SIGINT, stop_server);

	  /* Kick off thread to read/write serial port */

	  syslog(LOG_INFO, "Creating serial thread\n"); 
	  pthread_create(&serthread, NULL, radioTask, NULL);

	  snmp_log(LOG_INFO,"NovusAgent is up and running.\n");

	  while(keep_running) {
	    /* if you use select(), see snmp_select_info() in snmp_api(3) */
	    /*     --- OR ---  */
	    agent_check_and_process(1); /* 0 == don't block */

	   if (dbg)
	   {
	  	syslog(LOG_INFO, "SNMP pkt recv\n"); 
	   }
      	  }

 	 /* at shutdown time */
	  snmp_shutdown(NOVUS_NAME);
	  SOCK_CLEANUP;
  }

  return 0;
}

/************************************************
* Function       : agentConfigFile
* Input          : None
* Output         : 0 = No error, 1 = Error
* Description    : Reads the nsAgent configuration
* 	file located at /etc/nsagent.conf.  The config
* 	file locates the serial port of NMEA data and
* 	radio data.   It also specifies the GPIO to use
* 	for PPS signalling.
************************************************/
int agentConfigFile()
{
	int retVal = 0; // No error
	FILE *fp = NULL;
    char aline[DEF_BUFSIZE];
	size_t alen = DEF_BUFSIZE - 1;
	char *aptr = aline;
	char rval[512], lval[512];

        // Set defaults
      
    agentx_subagent = DEF_AGENTX;
    slog = DEF_SYSLOG;
    dbg = DEF_DBG;
	memset(nmeaName, 0, sizeof(nmeaName));
	memset(nr2316Name, 0, sizeof(nr2316Name));

	if ((fp = fopen(CONFIG_FILE, "rb")) != NULL)
	{
		// Read a line

		while ((getline(&aptr, &alen, fp)) != -1) 
       	        {
			// Clean up the line of spaces

			ltrim(aline);
			rtrim(aline);

			// DEVICES

			if (aline[0] == '#' || aline[0] == 0)
			{
				// Ignore comment lines
			} else {
				if (getlval(aline, lval) && getrval(aline, rval))
				{
					if (strcasecmp(lval, DEVICENMEA_PARM) == 0)
					{
						strcpy(nmeaName, rval);
			        } else if (strcasecmp(lval, DEVICENR2316_PARM) == 0) {
						strcpy(nr2316Name, rval);	
					} else if (strcasecmp(lval, AGENTX_PARM) == 0) {
						agentx_subagent = atoi(rval);	
					} else if (strcasecmp(lval, SYSLOG_PARM) == 0) {
						slog = atoi(rval);
					} else if (strcasecmp(lval, DBG_PARM) == 0) {
						dbg = atoi(rval);
					} else if (strcasecmp(lval, TRAP_PARM) == 0) {
						traps = atoi(rval);
					} else if (strcasecmp(lval, TEMP_HIGH_PARM) == 0) {
						ps_temp_high = atof(rval);
					} else if (strcasecmp(lval, TEMP_LOW_PARM) == 0) {
						ps_temp_low = atof(rval);
					}
				}
			}
		}

	fclose(fp);

	}


	return retVal;
}

/************************************************
* Function       : getlval
* Input          : string pointer
*                  Returned lval char *
* Output         : 0 = error, 1 = success
* Description    : Takes a string using an "="
*	delimiter and returns the left side of the
*	equal sign.
************************************************/
int getlval(char *sstr, char *lval)
{
	int retVal = 0; // Error 
	char *eq;

	if (sstr && lval)
	{
		if ((eq = strchr(sstr, '=')) != NULL)
		{
			memcpy(lval, sstr, eq - sstr);
			lval[eq - sstr] = 0;

			ltrim(lval);
			rtrim(lval);
			retVal = 1;
		}
	}

	return retVal;
}

/************************************************
* Function       : getrval
* Input          : string pointer
*                  Returned lval char *
* Output         : 0 = error, 1 = success
* Description    : Takes a string using an "="
*	delimiter and returns the right side of the
*	equal sign.
************************************************/
int getrval(char *sstr, char *rval)
{
	int retVal = 0; // Error 
	char *eq;
	char *q;
	if (sstr && rval)
	{
		if ((eq = strchr(sstr, '=')) != NULL)
		{
			strcpy(rval, eq+1);
			ltrim(rval);
			rtrim(rval);

			// Strip any quotes

			if (rval[0] == '\"')
			{
				strcpy(&rval[0], &rval[1]);
				if ((q = strchr(rval, '\"')) != NULL)
					*q = 0;
			}
				
			retVal = 1;
		}
	}

	return retVal;
}	

/************************************************
* Function       : radioTask
* Input          : vptr void * - Unused
* Output         : None
* Description    : The main() function spawns this
*	function to communicate with the radio in the
*	background.
************************************************/
void *radioTask(void *vptr)
{
	int serlen;

	char buf[MAX_BUFFER_SIZE];

    if (dbg)
    	syslog(LOG_INFO, "Entering radioTask\n");

    setpriority(PRIO_PROCESS, 0, 5);

	if ((radio_fd = radioIFStart()) != -1)
	{

//		// Set up shared memory             
		shm_id = shmget((key_t)SHARED_MEMORY_KEY, SHM_STORAGE_SIZE, 0666 | IPC_CREAT);	
		if(shm_id == -1)
		{
			perror("radioTask shmget fail.");
			syslog(LOG_INFO, "radioTask shmget fail:");
			return 0;
		}

		void* shm_ptr;
		shm_ptr = shmat(shm_id, (void *)0, 0);
		if (shm_ptr == (void *)-1)
		{
			perror("Shared memory shmat() failed\n");
			syslog(LOG_INFO, "radioTask shmat fail:");
			return 0;
		}
		shm_data = (char*)shm_ptr;


		while (keep_running)
		{
			/* Read line from serial port */

			buf[0] = 0;
			if ((serlen = getSerLine(radio_fd, buf, MAX_BUFFER_SIZE)) < MAX_BUFFER_SIZE)
			{
				if (dbg)
					syslog(LOG_INFO, "Ser Pkt (%d):\n%s\n", serlen, buf);
				strParser(buf);
			} else {
				if (dbg)
					syslog(LOG_INFO, "Unknown packet (%d):\n%s\n", serlen, buf);
			}
		}
	} else {
		syslog(LOG_INFO, "Could not open radio interface.\n");
		keep_running = 0;
	}

	

	shmdt(shm_data);
	shmctl(shm_id, IPC_RMID,0);

    syslog(LOG_INFO, "Exiting radioTask\n");
	return 0;
}

/************************************************
* Function       : writeRadio
* Input          : char *str - string to write
* Output         : None
* Description    : Writes the given null terminated
*   string to the opened radio port.  If port is
*	not opened, string is dropped.
************************************************/
void writeRadio(char *str)
{
	char *s = NULL;

	if (str && radio_fd != -1)
	{
		if ((s = strdup(str)) != NULL)
		{
			ltrim(rtrim(s));
			commandResultStr[0] = 0; // No results, yet...
			write(radio_fd, s, strlen(s));
		}
	}

	if (s)
		free(s);
}

/************************************************
* Function       : getSerLine
* Input          : int File ID of serial port
*                  Char * buffer to returned data
*                  int length of buffer
* Output         : # of bytes read from serial port
* Description    : Reads an entire line from the serial
*	port, ending with cr/lf, and returns it.
************************************************/
int getSerLine(int fd, char *buf, int buflen)
{
	char c = 0;
	int pos = 0;

	if (fd)
	{
		do {
			// Strings begin with a $ and end in a 0x0d 0x0a

			if (read(fd, &c, 1) == 1)
			{
				if (c == '$')
				{
					pos = 1;
					buf[0] = c;
				} else if (c >= ' ') {
					buf[pos++] = c;
				}
			}
		} while (pos < (buflen - 1) && c != 0x0a);

		buf[pos] = 0;
	}

	return pos;
}

/************************************************
* Function       : rtrim
* Input          : char *
* Output         : Returned input string char *
* Description    : Removes white spaces from the
*	input parameter from the end.
************************************************/
char* rtrim(char* string)
{
    int i = strlen(string) - 1;

    while (i)
    { 
       if (string[i] == ' ')
       {
            string[i] = 0;
            i--;
       } else {
            break;
       } 
    }

    return string;
}

/************************************************
* Function       : ltrim
* Input          : char *
* Output         : Returned input string char *
* Description    : Removes white spaces from the
*	input parameter from the beginning.
************************************************/
char* ltrim(char *string)
{
    while (string[0] == ' ')
    	strcpy(&string[0], &string[1]);

    return string;
}

/************************************************
* Function       : init_defaults
* Input          : None
* Output         : None
* Description    : Initialize default data.
************************************************/
void init_defaults()
{

	// Clear radio buffer.

	memset(&radio, 0, sizeof(radio));

	// Set all traps to "not active".

	memset((void *) trap_status, 0xff, sizeof(trap_status));

	strcpy(radio.nsSysIdentifier, SYSTEM_NAME);
}
