/*****************************************************************************
 * Project			: nsAgent
 * Author			: Bryan Wilcutt bwilcutt@yahoo.com
 * Date				: 5-13-18
 * System			: Nano PI
 * File				: nsStr.c
 *
 * Description		:
 *
 * This file contains the string parsing helper functions for parsing
 * multi-parameter constructs.
 *
 * Written for Novus Power.
 *
 * Copyright (c) Novus Power All Rights Reserved
 *****************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "nsStr.h"
 
/************************************************
* Function       : strSplit
* Input          : Source str pointer
*                        Delimiter value character
*                        Max return values
*                        Pointer to return array[Max Return Values][MAX_STR_WIDTH]
* Output         : 0-n = Parameter count, -1 = error
* Description    : Splits a string up on the given parameter character, such as ","
*        and places each parameter into the given array.  The array must be deep
*          enough to hold n parameters, wide enough to hold the largest width of any
*          parameter.  NO MEMORY ALLOCATION IS PERFORMED.
*
* Format specifiers: s (string), f (float), c (char), d (int)
* Floats may have L.R specifier, indicating number of digits Left and Right of dec.
* All others may have L only, specifying width.
*
* Example: f5.3 = 00000.000
* Example: s9 = abcdefghi
* Example: d = 123
* Example: x4 = 0x1234
* Example: "f5.3,f2.1,s10,s1,c1,s15,d,d,d"
************************************************/
int strSplit (const char *str, char c, int n, char (*sarray)[MAX_STR_WIDTH])
{
    int count = 1;
    int token_len = 0;
    int i = 0;
    char *p;
    char *t;
 
    // Count the number of delimiters.  We can have >= n but not <n.  We should always
    // have at least 1, hence why count = 1 at the beginning.

    p = (char *) str;
    while (*p != '\0')
    {
        if (*p == c)
            count++;
        p++;
    }

    // If there are more delimiters than what we are looking for, set counter to just
    // the number of delimiters we need.  If fewer delimiters than what we need then
    // produce an error.

    if (count > n)
    	count = n;
    else if (count < n)
        return -1;
 
    // Make sure none of our parameters are larger than our MAX_STR_WIDTH

    p = (char *) str;
    while (*p != '\0')
    {
        if (*p == c)
        {
        	if (token_len > MAX_STR_WIDTH)
        		return -1; // Too big!

            token_len = 0;
            i++;
        }
        p++;
        token_len++; // Make sure no parameter exceeds MAX_STR_WIDTH
    }
 
    // Pull each parameter and store in parameter array.

    i = 0;
    p = (char *) str;
    t = &sarray[i][0];

    while (*p != '\0' && i < count)
    {
        if (*p != c && *p != '\0')
        {
        	if (*p >= 0x20 && *p <= 0x7e)
            {
        		*t = *p;
            	t++;
            	*t = 0;
            }
        } else {
        	*t = 0;
            i++; // Next parameter
            if (i < count)
            	t = &sarray[i][0];
        }
        p++;
    }
 
    return count;
}
 
/************************************************
* Function       : getDelimiters
* Input          : Source str pointer
*                        lval int pointer
*                        rval int pointer
* Output         : 0 = Error, 1 = Verified ok
* Description    : Returns the lval and rval of a format
*          specifier.  For example, "f3.5", this function
*          will return lval=3, rval=5.
************************************************/
int getDelimiters(char *s, int *l, int *r)
{
      char *p;
      char buf[20];
 
      if (s)
      {
            if (l)
            {
                  *l = 0;
                  strcpy(buf, s);
 
                  p = strchr(buf, '.');
                  if (p)
                  {
                        *p = 0;
                  }
                  
                  *l = atoi(buf);
            }
      
            if (r)
            {
                  *r = 0;
                  p = strchr(s, '.');
                  if (p)
                  {
                        *r = atoi(p + 1);
                  }
            }
 
            return 1;
      } else {
            return 0;
      }
}
/************************************************
* Function       : verifyStr
* Input          : Source str pointer
*                        Format str poointer
* Output         : 0 = Error, 1 = Verified ok
* Description    : Determines if a string (s) follows the
*          same source format (f).
************************************************/
int verifyStr(char *s, char *f)
{
      int ldelim, rdelim;
      char *p;
 
      if (s && f)
      {
            switch(f[0])
            {
                  case 'f':
                        // Get any deliminitors
 
                        if (getDelimiters(f+1, &ldelim, &rdelim))
                        {
                              p = strchr(s, '.');
 
                              if ((p - s) > ldelim && ldelim != 0)
                                    return 0;
                              if (strlen(p+1) > rdelim && rdelim != 0)
                                    return 0;
                        } else {
                              return 0;
                        }
 
                        break;

                  case 'c':
                  case 's':
                  case 'd':
                        if (getDelimiters(f+1, &ldelim, NULL))
                        {
                              if (ldelim && (strlen(s) > ldelim))
                                    return 0;
                        }
                        break;
 
                  default:
                        return 0;
            }
      }
 
      return 1;
}

