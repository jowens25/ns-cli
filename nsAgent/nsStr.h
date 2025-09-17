/*****************************************************************************
 * Project			: nsAgent
 * Author			: Bryan Wilcutt bwilcutt@yahoo.com
 * Date				: 5-13-18
 * System			: Nano PI
 * File				: nsStr.h
 *
 * Description		:
 *
 * This file contains support constructs for nsStr.c.
 *
 * Written for Novus Power.
 *
 * Copyright (c) Novus Power All Rights Reserved
 *****************************************************************************/
#ifndef NSSTR_H
#define NSSTR_H

#define MAX_STR_WIDTH 32
#define ARRAY_DEPTH 10

extern int verifyStr(char *s, char *f);
int strSplit (const char *str, char c, int n, char (*sarray)[MAX_STR_WIDTH]);

#endif
