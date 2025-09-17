/*****************************************************************************
 * Project			: nsAgent
 * Author			: Bryan Wilcutt bwilcutt@yahoo.com
 * Date				: 5-13-18
 * System			: Nano PI
 * File				: nsRadioSer.c
 *
 * Description		:
 *
 * This file contains the start up code for serial ports.
 *
 * Written for Novus Power.
 *
 * Copyright (c) Novus Power All Rights Reserved
 *****************************************************************************/
#include <linux/kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <ctype.h>
#include <syslog.h>

void set_blocking(int fd, int should_block);

extern char nr2316Name[];
extern int dbg;

/************************************************
* Function       : radioIFStart
* Input          : None
* Output         : Int FD pointer, -1 if error
* Description    : Opens up and configurations the
*	radio serial port specified in the nsAgent
*	configuration file.
************************************************/
int radioIFStart()
{
	  int fd ;
      struct termios tty;
      memset (&tty, 0, sizeof tty);

	  fd = open(nr2316Name, O_RDWR | O_NOCTTY );
	  if (fd == -1)
	  {
	    if (dbg)
		{
			syslog(LOG_INFO, "Error no is : %d\n", errno);
	    	syslog(LOG_INFO, "Error description is : %s\n",strerror(errno));
		}
	    return(-1);
	  }

	  /* Set baud 38400, 8, N, 1
	   *
	   */

      if (tcgetattr (fd, &tty) != 0)
       {
               return -1;
       }

       cfsetospeed (&tty, B38400);
       cfsetispeed (&tty, B38400);

       tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
       // disable IGNBRK for mismatched speed tests; otherwise receive break
       // as \000 chars
       tty.c_iflag &= ~IGNBRK;         // disable break processing
       tty.c_lflag = 0;                // no signaling chars, no echo,
                                       // no canonical processing
       tty.c_oflag = 0;                // no remapping, no delays
       tty.c_cc[VMIN]  = 0;            // read doesn't block
       tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

       tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

       tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                       // enable reading
       tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
       tty.c_cflag &= ~CSTOPB;
       tty.c_cflag &= ~CRTSCTS;

       if (tcsetattr (fd, TCSANOW, &tty) != 0)
       {
               if (dbg) printf("error %d from tcsetattr", errno);
               return -1;
       }

       set_blocking(fd, 1);
       return fd;
}

/************************************************
* Function       : set_blocking
* Input          : int FD
* 				   0 = No blocking, 1 = Block
* Output         : None
* Description    : Sets the given serial port (fd)
*	to blocking or non-blocking.
************************************************/
void set_blocking(int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);

        if (tcgetattr (fd, &tty) != 0)
        {
                if (dbg) printf("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                if (dbg) printf("error %d setting term attributes", errno);
}
