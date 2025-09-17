/*****************************************************************************
 * Project			: nsAgent
 * Author			: Bryan Wilcutt bwilcutt@yahoo.com
 * Date				: 5-13-18
 * System			: Nano PI
 * File				: nsStrParser.c
 *
 * Description		:
 *
 * This file contains string-to-MIB parsers for each radio string type
 * supported (1-10).
 *
 * Written for Novus Power.
 *
 * Copyright (c) Novus Power All Rights Reserved
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <linux/kernel.h>
#include <ctype.h>
#include <syslog.h>

#include "nsStrParser.h"
#include "novusAgent.h"
#include "nsStr.h"
#include "nsStartup.h"

extern int dbg;

void parseId1(char *str);
void parseId2(char *str);
void parseId3(char *str);
void parseId4(char *str);
void parseId5(char *str);
void parseId6(char *str);
void parseId7(char *str);
void parseId8(char *str);
void parseId9(char *str);
void parseId10(char *str);
void parseId11(char *str);
void parseId13(char *str);
void parseId14(char *str);
void parseId15(char *str);
void parseId16(char *str);
void parseId99(char *str);

bool chksum(char *str, int cs);

const char novstr[] = "$GPNVS,";

extern radioBlock_t radio;


char interfaceStr [][36] = 
{
	"# Wired adapter #1\r\n",
	"allow-hotplug eth0\r\n",
	"no-auto-down eth0\r\n",
	"iface eth0 inet static\r\n",
	"address 192.168.7.224\r\n",
	"netmask 255.255.255.0\r\n",
	"gateway 192.168.7.254\r\n",
	"dns-nameservers 8.8.8.8 8.8.4.4\r\n",

	"# Local loopback\r\n",
	"auto lo\r\n",
	"iface lo inet loopback\r\n",


};

extern char* shm_data;
#define WRITE_TO_SHM_FLAG_STRING (1)
void writeToShm(void)
{
	if(shm_data == NULL) return;
	memcpy(shm_data, (char*)&nvwData, sizeof(NOVUS_WEB_DATA_T));
	memcpy(shm_data  + sizeof(NOVUS_WEB_DATA_T), (char*)&radio, sizeof(radioBlock_t));
	
}

NOVUS_WEB_INPUT_CMD_T webCmdInput;
int readFromShm(char* buff)
{
	int msgReady = 0;
	if(shm_data == NULL) return;
	if( *( shm_data + sizeof(NOVUS_WEB_DATA_T) + sizeof(radioBlock_t), sizeof(NOVUS_WEB_INPUT_CMD_T) - 1) != 0)
	{
		memcpy(webCmdInput,shm_data  + sizeof(NOVUS_WEB_DATA_T) + sizeof(radioBlock_t), sizeof(NOVUS_WEB_INPUT_CMD_T) );
		*( shm_data + sizeof(NOVUS_WEB_DATA_T) + sizeof(radioBlock_t), sizeof(NOVUS_WEB_INPUT_CMD_T) - 1) = 0;
		msgReady = 1;
	}
	return msgReady;
}

/************************************************
 * Function       : strParser
 * Input          : char * string to parse
 * Output         : False = failed, True = successful
 * Description    : Parses a string received from the
 * modem and calls the appropriate parsing function.
 ************************************************/
bool strParser(char *str)
{
	bool retVal = false; // Default failed
	int parm1;
	char *pstr = NULL;
   	char *lastchr;
   	unsigned int cs = 0;

	if (str)
	{
		// Is this a Novus string?

		pstr = strdup(str);

		// Look for "$GPNVS,n," where n=string number.

		if (sscanf((char *) str, "$GPNVS,%d,%*s", &parm1) == 1) {
			if (parm1 >= 0 && parm1 <= 99)
			{
				// Verify checksum

				if ((lastchr = strrchr(str, '*')) != NULL) 
                                {
					sscanf(lastchr+1,"%x", &cs);
					*lastchr = 0;	

					if (chksum(str+1, cs) == true) // Match!
					{
						// Remove GPNVS

						switch (parm1)
						{
							case 1:
								parseId1(str+9);
								
								break;
							case 2:
								parseId2(str+9);
								break;
							case 3:
								parseId3(str+9);
								break;
							case 4:
								parseId4(str+9);
								break;
							case 5:
								parseId5(str+9);
								break;
							case 6:
								parseId6(str+9);
								break;
							case 7:
								parseId7(str+9);
								break;
							case 8:
								parseId8(str+9);
								break;
							case 9:
								parseId9(str+9);
								break;
							case 10:
								parseId10(str+10);
								break;
							case 11:
								parseId11(str+10);
								break;
							case 13:
								parseId13(str+10);
								break;
							case 14:
								parseId14(str+10);
								break;
							case 15:
								parseId15(str+10);
								break;
							case 16:
								parseId16(str+10);
								break;
							case 99:
								parseId99(str+10);
								break;
							default:
								retVal = false;
						}

						if(WRITE_TO_SHM_FLAG_STRING == parm1)
						{
							writeToShm();
							if(readFromShm(n))
						}
					}
				} 
			}
		} else if (str[0] == '$') {
			if (str[1] != 'G')
			{
				if (strlen(str) < RESULT_MAX_LEN)
					strcpy(commandResultStr, str);
			}

			retVal = true;
		}
	}

	if (pstr)
		free(pstr);

	return retVal;
}

/**************************************************
 * Function		: chksum
 * Input		: char *Source string
 *                uint checksum to compare
 * Output		: True = Checksum matches,
 * 		          False= Checksum mismatch
 * Description	: Calculates the NMEA checksum of
 *                the input string.
 **************************************************/
bool chksum(char *str, int cs)
{
	int retVal = false;
	int checksum = 0;
	int i;

	/* Checksum between first $ and last *. */

	if (str)
	{
		for (i = 0; i < strlen((char *) str); i++)
		{
			checksum ^= str[i];
		}

		if (checksum == cs)
			retVal = true;
	}

	return retVal;
}
/*
(String 5) Changed Temperature range to be -40 to 99
(String 6) Changed maximum channel status words to be 0xffff, instead of 0x7fff
(String 9 (standard version)) Corrected string table with additional fields to match description.
(String 9 (standard version))  Changed temperature range to max 99.
(String 10) Modified string table slightly to avoid indicating extra chars.
*/

/*
	1. Identifier $GPNVS
	2. String ID 1
	3. Time (UTC) hhmmss
	4. Date mmddyy
	5. GPS 1 Lock (Valid) A = Valid, V = Not Valid, N = N/A
	6. GPS 2 Lock (Valid) A = Valid, V = Not Valid, N = N/A
	7. # of Sats in View (1) Greater of GPS or GNSS count, �N� = N/A
	8. # of Sats in View (2) Greater of GPS or GNSS count, �N� = N/A
	9. Channel Fault Byte 0x0000 to 0xFFFF (Hex OR�d value)
	10. Power Supply Fault Byte 0x00 to 0xFF (Hex OR�d value)
	11. Error Message Byte 0x00 to 0xFF (Hex OR�d value)
	12. Antenna 1 0 = Ok, 1 = Error, N = N/A
	13. Antenna 2 0 = Ok, 1 = Error, N = N/A
	14. NMEA Checksum *XX (xor'd value of bytes between $ and *)

const char id1[] = "s6,s6,s1,s1,d,s1,x4,x4,x2,d,s1";
*/

void parseId1(char *str)
{
	char strarray[11][MAX_STR_WIDTH];
	char fmtarray[11][MAX_STR_WIDTH];

    if (strSplit(id1, ',', 11, &fmtarray[0]) == 11)
    {
    	if (strSplit(str, ',', 11, &strarray[0]) == 11)
    	{
			memcpy(nvwData.nsTime,&strarray[0][0],2);
			nvwData.nsTime[2] = ':';
			memcpy(nvwData.nsTime + 3,&strarray[0][2],2);
			nvwData.nsTime[5] = ':';
			memcpy(nvwData.nsTime + 6,&strarray[0][4],2);
			nvwData.nsTime[8] = 0;

			memcpy(nvwData.nsDate,&strarray[1][0],2);
			nvwData.nsDate[2] = '-';
			memcpy(nvwData.nsDate + 3,&strarray[1][2],2);
			nvwData.nsDate[5] = '-';
			memcpy(nvwData.nsDate + 6,&strarray[1][4],2);
			nvwData.nsDate[8] = 0;



    		strcpy(radio.nsFaultGPS1Lock, &strarray[2][0]);
    		strcpy(radio.nsFaultGPS2Lock, &strarray[3][0]);
    		strcpy(radio.nsFaultSatView1,  &strarray[4][0]);
    		strcpy(radio.nsFaultSatView2, &strarray[5][0]);
    		strcpy(radio.nsFaultChannelBytes, &strarray[6][0]);
    		strcpy(radio.nsFaultPowerSupplyByte, &strarray[7][0]);
    		strcpy(radio.nsFaultErrMsgByte, &strarray[8][0]);
    		strcpy(radio.nsFaultAnt1Stat, &strarray[9][0]);
    		strcpy(radio.nsFaultAnt2Stat, &strarray[10][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S1: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S1: Format err\n");
		}
	}
}

/*
1. Identifier $GPNVS
2. String ID 2
3. Time (UTC) hhmmss
4. Date mmddyy
5. Channel 1 Vrms 0.00 to 3.30 [V]
6. Channel 2 Vrms 0.00 to 3.30 [V]
7. Channel 3 Vrms 0.00 to 3.30 [V]
8. Channel 4 Vrms 0.00 to 3.30 [V]
9. Channel 5 Vrms 0.00 to 3.30 [V]
10. Channel 6 Vrms 0.00 to 3.30 [V]
11. Channel 7 Vrms 0.00 to 3.30 [V]
12. Channel 8 Vrms 0.00 to 3.30 [V]
13. NMEA Checksum *XX (xor�d value of bytes between $ and *)

const char id2[] = "%6s,%6s,%4s,%4s,%4s,%4s,%4s,%4s,%4s,%4s";
034852,050218,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20

*/
void parseId2(char *str)
{
	char strarray[10][MAX_STR_WIDTH];
	char fmtarray[10][MAX_STR_WIDTH];

    if (strSplit(id2, ',', 10, &fmtarray[0]) == 10)
    {
    	if (strSplit(str, ',', 10, &strarray[0]) == 10)
    	{
			strcpy(radio.nsChannel1Vrms, &strarray[2][0]);
			strcpy(radio.nsChannel2Vrms, &strarray[3][0]);
			strcpy(radio.nsChannel3Vrms, &strarray[4][0]);
			strcpy(radio.nsChannel4Vrms, &strarray[5][0]);
			strcpy(radio.nsChannel5Vrms, &strarray[6][0]);
			strcpy(radio.nsChannel6Vrms, &strarray[7][0]);
			strcpy(radio.nsChannel7Vrms, &strarray[8][0]);
			strcpy(radio.nsChannel8Vrms, &strarray[9][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S2: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S2: Format err\n");
		}
	}
}

/*
	1. Identifier $GPNVS
	2. String ID 3
	3. Time (UTC) hhmmss
	4. Date mmddyy
	5. Power Supply 1 -30.0 to 30.0 [V]
	6. Power Supply 2 -30.0 to 30.0 [V]
	7. Power Supply 3 -30.0 to 30.0 [V]
	8. Power Supply 4 -30.0 to 30.0 [V]
	9. Power Supply 5 -30.0 to 30.0 [V]
	10. Power Supply 6 -30.0 to 30.0 [V]
	11. Power Supply 7 -30.0 to 30.0 [V]
	12. Power Supply 8 -30.0 to 30.0 [V]
	13. Built in Test (BIT) 0 = Ok, 1 = Fail
	14. Temperature (C) -40 to 99
	15. NMEA Checksum *XX (xor�d value of bytes between $ and *)
"s6,s6,s5,s5,s5,s5,s5,s5,s5,s5,s1,s3"
$GPNVS,3,011445,050918,11.9,0.31,0.00,0.00,-10.4,,,,1,24*49
*/

void parseId3(char *str)
{
	char strarray[12][MAX_STR_WIDTH];
	char fmtarray[12][MAX_STR_WIDTH];

    if (strSplit(id3, ',', 12, &fmtarray[0]) == 12)
    {
    	if (strSplit(str, ',', 12, &strarray[0]) == 12)
    	{
    		strcpy(radio.nsPS1Status, &strarray[2][0]);
			strcpy(radio.nsPS2Status, &strarray[3][0]);
			strcpy(radio.nsPS3Status, &strarray[4][0]);
			strcpy(radio.nsPS4Status, &strarray[5][0]);
			strcpy(radio.nsPS5Status, &strarray[6][0]);
			strcpy(radio.nsPS6Status, &strarray[7][0]);
			strcpy(radio.nsPS7Status, &strarray[8][0]);
			strcpy(radio.nsPS8Status, &strarray[9][0]);
			strcpy(radio.nsBITStatus, &strarray[10][0]);
			strcpy(radio.nsPSTemp, &strarray[11][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S3: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S3: Format err\n");
		}
	}
}

/*
	1. Identifier $GPNVS
	2. String ID 4
	3. Time (UTC) hhmmss
	4. Date mmddyy
	5. Channel 9 Vrms 0.00 to 3.30 [V]
	6. Channel 10 Vrms 0.00 to 3.30 [V]
	7. Channel 11 Vrms 0.00 to 3.30 [V]
	8. Channel 12 Vrms 0.00 to 3.30 [V]
	9. Channel 13 Vrms 0.00 to 3.30 [V]
	10. Channel 14 Vrms 0.00 to 3.30 [V]
	11. Channel 15 Vrms 0.00 to 3.30 [V]
	12. Channel 16 Vrms 0.00 to 3.30 [V]
	13. NMEA Checksum *XX (xor�d value of bytes between $ and *)

const char id4[] = "%6c,%6c,%4c,%4c,%4c,%4c,%4c,%4c,%4c,%4c";
*/

void parseId4(char *str)
{
	char strarray[11][MAX_STR_WIDTH];
	char fmtarray[11][MAX_STR_WIDTH];

    if (strSplit(id4, ',', 10, &fmtarray[0]) == 10)
    {
    	if (strSplit(str, ',', 10, &strarray[0]) == 10)
    	{
    		strcpy(radio.nsChannel9Vrms, &strarray[2][0]);
			strcpy(radio.nsChannel10Vrms, &strarray[3][0]);
			strcpy(radio.nsChannel11Vrms, &strarray[4][0]);
			strcpy(radio.nsChannel12Vrms, &strarray[5][0]);
			strcpy(radio.nsChannel13Vrms, &strarray[6][0]);
			strcpy(radio.nsChannel14Vrms, &strarray[7][0]);
			strcpy(radio.nsChannel15Vrms, &strarray[8][0]);
			strcpy(radio.nsChannel16Vrms, &strarray[9][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S4: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S4: Format err\n");
		}
	}
}

/*
	1. Identifier $GPNVS
	2. String ID 5
	3. Time (UTC) hhmmss
	4. Date mmddyy
	5. Potentiometer Value 0 to 63
	6. Fan PWM % 0 to 90
	7. Temperature -40 to 90 [C]
	8. NMEA Checksum *XX (xor�d value of bytes between $ and *)

Changed Temperature range to be -40 to 99

const char id5[] = "s6,s6,2s,2s,3s";

*/

void parseId5(char *str)
{
	char strarray[6][MAX_STR_WIDTH];
	char fmtarray[6][MAX_STR_WIDTH];

    if (strSplit(id5, ',', 6, &fmtarray[0]) == 6)
    {
    	if (strSplit(str, ',', 6, &strarray[0]) == 6)
    	{
    		strcpy(radio.nsSensorPotentiometer, &strarray[2][0]);
			strcpy(radio.nsSensorFanPWM, &strarray[3][0]);
		    strcpy(radio.nsSensorTemperature, &strarray[4][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S5: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S5: Format err\n");
		}
	}
}

/*
	1. Identifier $GPNVS
	2. String ID 6
	3. Active PCB Assembly 0 or 1
	4. GNSS Lock A = Locked, V = Unlocked
	5. Input Error 0 = Ok, 1 = A Error, 2 = B error
	6. Channel Status Word 0x0000 to 0x7FFF
	7. Primary PS Status 0x00 to 0Xff
	8. Secondary PS Status 0x00 to 0xFF
	9. Active PCB Status 0x00 to 0xFF
	10. Checksum Status 00 to 999
	11. Channel Fault Bin 0x0000 to 0x7FFF
	12. Primary PCB Amp Status 0x0000 to 0x7FFF
	13. Backup PCB Amp Status 0x0000 to 0x7FFF
	14. NMEA Checksum *XX (xor�d value of bytes between $ and *)

(String 6) Changed maximum channel status words to be 0xffff, instead of 0x7fff

const char id6[] = "s1,s1,s1,s6,s4,s4,s4,s3,s6,s6,s6";
*/

void parseId6(char *str)
{
	char strarray[11][MAX_STR_WIDTH];
	char fmtarray[11][MAX_STR_WIDTH];

    if (strSplit(id6, ',', 11, &fmtarray[0]) == 11)
    {
    	if (strSplit(str, ',', 11, &strarray[0]) == 11)
    	{
    		strcpy(radio.nsSysActivePCBAssy, &strarray[0][0]);
			strcpy(radio.nsSysGNSSLock, &strarray[1][0]);
			strcpy(radio.nsSysInputErr, &strarray[2][0]);
			strcpy(radio.nsSysChanStatusWord, &strarray[3][0]);
			strcpy(radio.nsSysPriPSStatus, &strarray[4][0]);
			strcpy(radio.nsSysSecPSStatus, &strarray[5][0]);
			strcpy(radio.nsSysActivePCBStatus, &strarray[6][0]);
			strcpy(radio.nsSysChksumStatus, &strarray[7][0]);
		    strcpy(radio.nsSysChanFaultBin, &strarray[8][0]);
			strcpy(radio.nsSysPriPCBAmpStatus, &strarray[9][0]);
		    strcpy(radio.nsSysBkupPCBAmpStatus, &strarray[10][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S6: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S6: Format err\n");
		}
	}
}

/* NEEDS CHECKING
	1. Identifier $GPNVS
	2. String ID 7
	3. Time hhmmss
	4. Date mmddyy
	5. GPS Lock 'A' = Valid, 'V' = Not Valid
	6. # of Sats in View (1) Greater of GPS or GNSS count, �N� = N/A
	7. Error Byte 0x00 to 0xFF
	8. Freq Diff �999 (last count, clock cycles)
	9. PPS Diff �999 (last count, clock cycles)
	10. Freq Correction Slice �999 (DAC bits, per second)
	11. DAC Value Integer Representation, n x 1/(2^20)
	12. Power Supply Vdc
	13. Power Supply Vdc
	14. NMEA Checksum *XX (xor�d value of bytes between $ and *)

$GPNVS,7,051155,050918,A,13,0x00,0,-2,0,510697,+4.77,-4.30*76

const char id7[] = "%6c,%6c,%1c,%2c,%4c,%3c,%3c,%3c,%12c,%6c,%6c";
*/

void parseId7(char *str)
{
	char strarray[11][MAX_STR_WIDTH];
	char fmtarray[11][MAX_STR_WIDTH];

    if (strSplit(id7, ',', 11, &fmtarray[0]) == 11)
    {
    	if (strSplit(str, ',', 11, &strarray[0]) == 11)
    	{

    		strcpy(radio.nsSysGPSLock, &strarray[2][0]);
			strcpy(radio.nsSysSatView, &strarray[3][0]);
			strcpy(radio.nsSysErrorByte,&strarray[4][0]);
			strcpy(radio.nsSysFreqDiff, &strarray[5][0]);
			strcpy(radio.nsSysPPSDiff, &strarray[6][0]);
			strcpy(radio.nsSysFreqCorSlice, &strarray[7][0]);
			strcpy(radio.nsSysDACValue, &strarray[8][0]);
			strcpy(radio.nsSysPS1VDC, &strarray[9][0]);
			strcpy(radio.nsSysPS2VDC, &strarray[10][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S7: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S7: Format err\n");
		}
	}
}

/*
	1. Identifier $GPNVS
	2. String ID 8
	3. Discipline Counter 0 = Off, 1 = Disciplined to Synthetic PPS
	4. User Enabled 0 = Off, 1 = On
	5. Event Enabled(System) 0 = Events Disabled, 1 = Events Enabled
	6. GPS Lock Achieved 0 = No Lock, 2 = Locked or previously locked
	7. Event Index 0-99, Current count of events in RAM
	8. Event Errors 0
	9. Event Time Alignmet 2 = LS applied, 1 = GPS, 0 = RTC
	10. Estimated Accuracy 0-999999 [ns]
	11. Edge Detect Direction 0 = Falling Edge, 1 = Rising Edge
	12. NMEA Checksum *XX (xor�d value of bytes between $ and *)

const char id8[] = "%1c,%1c,%1c,%1c,%2c,%1c,%1c,%6c,%1c";
*/

void parseId8(char *str)
{
	char strarray[8][MAX_STR_WIDTH];
	char fmtarray[8][MAX_STR_WIDTH];

    if (strSplit(id8, ',', 8, &fmtarray[0]) == 8)
    {
    	if (strSplit(str, ',', 8, &strarray[0]) == 8)
    	{
    		strcpy(radio.nsEventDiscCounter, &strarray[0][0]);
			strcpy(radio.nsEventUserEnabled, &strarray[1][0]);
			strcpy(radio.nsEventSysEnabled, &strarray[2][0]);
			strcpy(radio.nsEventGPSLock, &strarray[3][0]);
			strcpy(radio.nsEventRAMIndex, &strarray[4][0]);
			strcpy(radio.nsEventTimeAlignment, &strarray[5][0]);
			strcpy(radio.nsEventEstAccuracy, &strarray[6][0]);
			strcpy(radio.nsEventEdgeDetDir, &strarray[7][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S8: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S8: Format err\n");
		}
	}
}

/*
	1. Identifier $GPNVS
	2. String ID 9
	5. Measured Frequency 9999900.000 to 10000100.000
	6. DAC Volts
	7. Freq
	8. multilength
	9. Antenna Voltage
	10. Output Vrms
	11. Batt Volts
	12. Temperature
	13. Heater Set Temperature
	14. NMEA Checksum *XX (xor�d value of bytes between $ and *)
*/

void parseId9(char *str)
{
	char strarray[9][MAX_STR_WIDTH];
	char fmtarray[9][MAX_STR_WIDTH];

    if (strSplit(id9, ',', 9, &fmtarray[0]) == 9)
    {
    	if (strSplit(str, ',', 9, &strarray[0]) == 9)
    	{
    		strcpy(radio.nsMeasureFreq, &strarray[0][0]);
			strcpy(radio.nsMeasureDAC, &strarray[1][0]);

			strcpy(radio.nsMeasureAnt, &strarray[4][0]);

			strcpy(radio.nsMeasureTemp, &strarray[7][0]);
			strcpy(radio.nsMeasureHeaterTemp, &strarray[8][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S9: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S9: Format err\n");
		}
	}
}

/*
	1. Identifier $GPNVS
	2. String ID 10
	3. PPS Stability Enabled 0 = Off, 1 = On
	4. PPS Disciplining to GPS 0 = Off, 1 = Actively Synchronized
	5. PPS Output Type 0 = Synthetic PPS, 1 = GPS PPS
	6. PPS Difference +/-999 (clock cycles)
	7. PPS pull Cal Factor 0.1 to 10.0
	8. PPS active Time Cal Factor 0 to 9
	9. Frequency Variance 0-999 (clock cycles per Loop period)
	10. NMEA Checksum *XX (xor'd value of bytes between $ and *)
(String 10) Modified string table slightly to avoid indicating extra chars.

$GPNVS,10,0,1,0,+0,+0,2,100,0.5,4,0,10,1,+0,1.0,20*46

const char id10[] = "s1,s1,s1,s3,s4,s1,s3,";
*/

void parseId10(char *str)
{
	char strarray[7][MAX_STR_WIDTH];
	char fmtarray[7][MAX_STR_WIDTH];

    if (strSplit(id10, ',', 7, &fmtarray[0]) == 7)
    {
    	if (strSplit(str, ',', 7, &strarray[0]) == 7)
    	{
    		strcpy(radio.nsPPSStability, &strarray[0][0]);
			strcpy(radio.nsPPSDiscGPS, &strarray[1][0]);
			strcpy(radio.nsPPSOutputType, &strarray[2][0]);
			strcpy(radio.nsPPSDifference, &strarray[3][0]);
			strcpy(radio.nsPPSCalFactor, &strarray[4][0]);
			strcpy(radio.nsPPSTimeCalFactor, &strarray[5][0]);
			strcpy(radio.nsPPSFreqVar, &strarray[6][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S10: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S10: Format err\n");
		}
	}
}




/*
	1. Identifier $GPNVS
	2. String ID 11
	3. Warmup remaining 0-99999
	4. Warmup time met 0=warmup not complete, 1= warmup complete
	5. Holdover Time elapsed 0-999999
	6. Holdover valid, 0=not valid, 1=valid
	7. Frequency valid, 0=not valid, 1=valid
	8. Holdover temperature -40.0 - 99.0
	9. NMEA Checksum *XX (xor�d value of bytes between $ and *)

	#define id11 "s5,s1,s6,s1,s1,s1,s1,s5"
*/

void parseId11(char *str)
{
	char strarray[8][MAX_STR_WIDTH];
	char fmtarray[8][MAX_STR_WIDTH];

    if (strSplit(id11, ',', 8, &fmtarray[0]) == 8)
    {
    	if (strSplit(str, ',', 8, &strarray[0]) == 8)
    	{
    		strcpy(radio.nsWarmupRemaining, &strarray[0][0]);
			strcpy(radio.nsWarmupComplete, &strarray[1][0]);
			strcpy(radio.nsHoldoverElapsed, &strarray[2][0]);
			strcpy(radio.nsHoldoverValid, &strarray[3][0]);
			strcpy(radio.nsFrequencyValid, &strarray[4][0]);
			// strcpy(radio., &strarray[5][0]);
			// strcpy(radio., &strarray[6][0]);
			strcpy(radio.nsHoldoverTemp, &strarray[7][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S11: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S11: Format err\n");
		}
	}
}

/*
1. Identifier						$GPNVS
2. String ID						13
3. Priority Discipline Source		0 = GNSS, 1 = 10MHz input, 2 = Optical input
4. Current Discipline Source		0 = GNSS, 1 = 10MHz, 2 = Optical, 3 = Holdover
5. GNSS Lock						0 to 3, 0 = Unlocked, 3 = Fully Locked
6. RF Present						0 = No RF source, 1 = RF Source found
7. Opto Present						0 = No Optical source, 1 = Optical Source Found
8. Loop Lock						1 = Lock, 0 = Loop acquiring lock
9. Reserved
10. NMEA Checksum					*XX (xor’d value of bytes between $ and *)
*/

void parseId13(char *str)
{
	char strarray[6][MAX_STR_WIDTH];
	char fmtarray[6][MAX_STR_WIDTH];

    if (strSplit(id13, ',', 6, &fmtarray[0]) == 6)
    {
    	if (strSplit(str, ',', 6, &strarray[0]) == 6)
    	{
    		strcpy(radio.nsDiscPrioritySource, &strarray[0][0]);
			strcpy(radio.nsDiscCurrentSource, &strarray[1][0]);
			strcpy(radio.nsDiscGNSSLock, &strarray[2][0]);
			strcpy(radio.nsDiscRFPresent, &strarray[3][0]);
			strcpy(radio.nsDiscOpticalPresent, &strarray[4][0]);
			strcpy(radio.nsDiscLoopLock, &strarray[5][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S13: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S13: Format err\n");
		}
	}
}




/*
1. Identifier						$GPNVS
2. String ID						14
3. Rb Status						8=init,5=laserlock,0=full lock.
4. Rb Alarm							0=ok, 1=mwsignal, 4=lctemp, 5=abctemp, 8=lcabc, 9=allalarm
5. Rb Mode							0x%04
6. Rb steer							%04d
7. Rb discipline status				0=Disc Wait, 1=Disc Good, 2=Holdover
8. Rb last update second			%03u
9. Offset avg						s10
10. Rb used as holdover				0=Using GNSS, 1=Changing State, 2=Using Rb
11. NMEA Checksum					*XX (xor’d value of bytes between $ and *)


#define id14 "s1,s1,s6,s4,s1,s3,s10,s1"
*/

void parseId14(char *str)
{
	char strarray[8][MAX_STR_WIDTH];
	char fmtarray[8][MAX_STR_WIDTH];

    if (strSplit(id14, ',', 8, &fmtarray[0]) == 8)
    {
    	if (strSplit(str, ',', 8, &strarray[0]) == 8)
    	{
    		strcpy(radio.nsRbStatus, &strarray[0][0]);
			strcpy(radio.nsRbAlarm, &strarray[1][0]);
			strcpy(radio.nsRbMode, &strarray[2][0]);
			// strcpy(radio., &strarray[3][0]);		//steer not used
			strcpy(radio.nsRbDiscStatus, &strarray[4][0]);
			// strcpy(radio., &strarray[5][0]);		//last update
			// strcpy(radio., &strarray[6][0]);		//offset phase
			strcpy(radio.nsRbHoldoverSource, &strarray[7][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S14: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S14: Format err\n");
		}
	}
}




void parseId15(char *str)
{
	char strarray[6][MAX_STR_WIDTH];
	char fmtarray[6][MAX_STR_WIDTH];

    if (strSplit(id15, ',', 6, &fmtarray[0]) == 6)
    {
    	if (strSplit(str, ',', 6, &strarray[0]) == 6)
    	{
    		strcpy(radio.nsRb2Lock, &strarray[0][0]);
			strcpy(radio.nsRb2Status, &strarray[2][0]);
			strcpy(radio.nsRb2Steer, &strarray[3][0]);

    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S15: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S15: Format err\n");
		}
	}
}



/*
1. Identifier $GPNVS
2. String ID 16
3. Time (UTC) hhmmss
4. Date mmddyy
5. Channel 1 Vrms 0.00 to 3.30 [V]
6. Channel 2 Vrms 0.00 to 3.30 [V]
7. Channel 3 Vrms 0.00 to 3.30 [V]
8. Channel 4 Vrms 0.00 to 3.30 [V]
9. Channel 5 Vrms 0.00 to 3.30 [V]
10. Channel 6 Vrms 0.00 to 3.30 [V]
11. Channel 7 Vrms 0.00 to 3.30 [V]
12. Channel 8 Vrms 0.00 to 3.30 [V]
13. NMEA Checksum *XX (xor�d value of bytes between $ and *)

const char id2[] = "%6s,%6s,%4s,%4s,%4s,%4s,%4s,%4s,%4s,%4s";
034852,050218,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20

*/
void parseId16(char *str)
{
	char strarray[10][MAX_STR_WIDTH];
	char fmtarray[10][MAX_STR_WIDTH];

    if (strSplit(id16, ',', 10, &fmtarray[0]) == 10)
    {
    	if (strSplit(str, ',', 10, &strarray[0]) == 10)
    	{
			strcpy(radio.nsChannel17Vrms, &strarray[2][0]);
			strcpy(radio.nsChannel18Vrms, &strarray[3][0]);
			strcpy(radio.nsChannel19Vrms, &strarray[4][0]);
			strcpy(radio.nsChannel20Vrms, &strarray[5][0]);
			strcpy(radio.nsChannel21Vrms, &strarray[6][0]);
			strcpy(radio.nsChannel22Vrms, &strarray[7][0]);
			strcpy(radio.nsChannel23Vrms, &strarray[8][0]);
			strcpy(radio.nsChannel24Vrms, &strarray[9][0]);
    	} else {
    		if (dbg)
    		{
    			syslog(LOG_INFO, "S16: Malformed\n");
    		}
    	}
	} else {
		if (dbg)
		{
			syslog(LOG_INFO, "S16: Format err\n");
		}
	}
}









#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/route.h>
#include <unistd.h>
#include <sys/reboot.h>

void getIPInfoStr(char* dstBuff);


/* Move pointer forward to the first position after the specified number of commas*/
char *commaParse(char *tempLoc, uint8_t numCommas)
{
	uint8_t index;
	uint8_t maxChars = 0;

	for (index = 0; index < numCommas; index++)
	{
		while(*tempLoc++ != ',' && *tempLoc != '*' && maxChars++ < 100);
	}
	return tempLoc;
}

int copyUntilComma(char *dest, char *src, int num)
{
	int count = 0;
	char* loc = src;
	while(count < num && *loc != ',' && *loc != 0)
	{
		*(dest + count) = *loc;
		count++;
		loc++;
	}
	return count;

}


void parseId99(char *str)
{

	if(strncmp(str,"A5A5",4)==0)
	{
		writeRadio("RECEIVED_IP_CHANGE.\r\n");
		char ipStr[24] = {0};
		char mskStr[24] = {0};
		char gtwStr[24] = {0};

		//get incoming string to strings
		char *loc = str;
		loc = commaParse(loc,1);
		int ipLen = copyUntilComma(ipStr,loc,20);
		loc = commaParse(loc,1);
		int mskLen = copyUntilComma(mskStr,loc,20);
		loc = commaParse(loc,1);
		int gtwLen = copyUntilComma(gtwStr,loc,20);

		char addrCheck[24] = {0};
		memcpy(addrCheck,ipStr,20);
		writeRadio(addrCheck);
		writeRadio("\r\n");
		memcpy(addrCheck,mskStr,20);
		writeRadio(addrCheck);
		writeRadio("\r\n");
		memcpy(addrCheck,gtwStr,20);
		writeRadio(addrCheck);
		writeRadio("\r\n");

		char buffNum[24] = {0};
		snprintf(buffNum,20,"%i,%i,%i\r\n",ipLen,mskLen,gtwLen);
		writeRadio(buffNum);

		memset(&interfaceStr[4][8],0,25);
		memset(&interfaceStr[5][8],0,25);
		memset(&interfaceStr[6][8],0,25);

		snprintf(&interfaceStr[4][8],30,"%s\r\n",ipStr);
		snprintf(&interfaceStr[5][8],30,"%s\r\n",mskStr);
		snprintf(&interfaceStr[6][8],30,"%s\r\n",gtwStr);

		int i = 0;
		while(i < 11)
		{
			writeRadio(&interfaceStr[i][0]);
			i++;
		}

		FILE* fp  = fopen("etc/network/interfaces",  "w+");

		if(fp)
		{
			i = 0;
			while(i < 11)
			{
				fprintf(fp,&interfaceStr[i][0]);
				i++;
			}
			fclose(fp);
		}

		sync();
		reboot(RB_AUTOBOOT);


	}
	else
	{
		char buff[100] = {0};
		getIPInfoStr(buff);
		writeRadio(buff);
	}
}


void getIPInfoStr(char* dstBuff)
{



    struct ifaddrs *ifaddr, *ifa;
    int s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1)
    {
        return;
    }


    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if((strcmp(ifa->ifa_name,"eth0")==0)&&(ifa->ifa_addr->sa_family==AF_INET))
        {
            if (s != 0)
            {
                return;
            }
            char mask[INET_ADDRSTRLEN];
            void* mask_ptr = &((struct sockaddr_in*) ifa->ifa_netmask)->sin_addr;
            inet_ntop(AF_INET, mask_ptr, mask, INET_ADDRSTRLEN);
		char buffTemp[24] = {0};
		memcpy(buffTemp,host,19);
                snprintf(dstBuff,29,"$IP:%s,",buffTemp);
                strncat(dstBuff,mask,28);
                strncat(dstBuff,",",4);
        }
    }

    freeifaddrs(ifaddr);

    FILE *f;
    char line[100] , *p , *c, *d;

    f = fopen("/proc/net/route" , "r");
    	if(f != NULL)
	{

    	while(fgets(line , 100 , f))
    	{
        	p = strtok(line , " \t");
        	c = strtok(NULL , " \t");
        	d = strtok(NULL, " \t");

        	if(p!=NULL && c!=NULL && d != NULL)
        	{
                	if(strcmp(c , "00000000") == 0)
                	{
                        char *gw;
                        char g[16] = {0};
                        memcpy(g,d + 6,2);
                        memcpy(g+2,d+4,2);
                        memcpy(g+4,d+2,2);
                        memcpy(g+6,d,2);

                        struct in_addr addr;
                        addr.s_addr = htonl(strtoul(g,NULL,16));
                        gw = inet_ntoa(addr);
			if(gw != NULL)
				strncat(dstBuff,gw,20);
                	}
        	}
	}

    	}
        strncat(dstBuff,"\r\n\0",4);

fclose(f);
}






