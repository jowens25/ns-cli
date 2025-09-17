/*****************************************************************************
 * Project			: nsAgent
 * Author			: Bryan Wilcutt bwilcutt@yahoo.com
 * Date				: 5-13-18
 * System			: Nano PI
 * File				: nsStartup.h
 *
 * Description		:
 *
 * Support constructs for nsStartup.c.
 *
 * Written for Novus Power.
 *
 * Copyright (c) Novus Power All Rights Reserved
 *****************************************************************************/
#ifndef NSSTARTUP_H
#define NSSTARTUP_H

#define DEF_AGENTX 1
#define DEF_SYSLOG 0
#define DEF_DBG    0
#define DEF_TEMP_HIGH (99)
#define DEF_TEMP_LOW (-40)
#define DEF_BUFSIZE 512

#define DEVICENMEA_PARM "DEVICE_NMEA"
#define DEVICENR2316_PARM "DEVICE_NR2316"

#define AGENTX_PARM "AGENTX"
#define SYSLOG_PARM "SYSLOG"
#define DBG_PARM    "DEBUG"
#define TRAP_PARM	"TRAPS"
#define TEMP_HIGH_PARM "TEMPHIGH"
#define TEMP_LOW_PARM "TEMPLOW"

#define SYSTEM_NAME	"Novus NR"
#define DEVICE_NAME_LEN 256

#define CONFIG_FILE "/etc/nsagent.conf"

#define RESULT_MAX_LEN 254

extern void writeRadio(char *str);
extern int dbg;
extern char commandResultStr[RESULT_MAX_LEN];
#define	SHARED_MEMORY_KEY 		672213396   		//Shared memory unique key (SAME AS PHP KEY)

extern char* shm_data;


#endif
