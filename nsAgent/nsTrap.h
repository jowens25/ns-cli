/*****************************************************************************
 * Project			: nsAgent
 * Author			: Bryan Wilcutt bwilcutt@yahoo.com
 * Date				: 5-13-18
 * System			: Nano PI
 * File				: nsTrap.h
 *
 * Description		:
 *
 * This file contains support constructs for nsTrap.c.
 *
 * Written for Novus Power.
 *
 * Copyright (c) Novus Power All Rights Reserved
 *****************************************************************************/
#ifndef NSTRAP_H_
#define NSTRAP_H_

extern void init_notification();

#define CHAN_OK 0
#define CHAN_FAULT 1

#define PS_OK 0
#define PS_FAULT 1

#define TEMP_HIGH 0
#define TEMP_LOW 1
#define TEMP_OK 2

#define GPS_UNLOCK 0
#define GPS_LOCK 2

typedef enum {
    TRAP_NSPS1STATUS = 0,
    TRAP_NSPS2STATUS,
    TRAP_NSPS3STATUS,
    TRAP_NSPS4STATUS,
    TRAP_NSPS5STATUS,
    TRAP_NSPS6STATUS,
    TRAP_NSPS7STATUS,
    TRAP_NSPS8STATUS,
    TRAP_NSPSTEMP,
    TRAP_NSCHANNEL1VRMS,
    TRAP_NSCHANNEL2VRMS,
    TRAP_NSCHANNEL3VRMS,
    TRAP_NSCHANNEL4VRMS,
    TRAP_NSCHANNEL5VRMS,
    TRAP_NSCHANNEL6VRMS,
    TRAP_NSCHANNEL7VRMS,
    TRAP_NSCHANNEL8VRMS,
    TRAP_NSCHANNEL9VRMS,
    TRAP_NSCHANNEL10VRMS,
    TRAP_NSCHANNEL11VRMS,
    TRAP_NSCHANNEL12VRMS,
    TRAP_NSCHANNEL13VRMS,
    TRAP_NSCHANNEL14VRMS,
    TRAP_NSCHANNEL15VRMS,
    TRAP_NSCHANNEL16VRMS,
	TRAP_NSEVENTGPSLOCK,
	TRAP_COUNT
} trap_t;

extern char trap_status[TRAP_COUNT];

#endif /* NSTRAP_H_ */
