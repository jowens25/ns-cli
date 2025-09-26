/*****************************************************************************
 * Project			: nsAgent
 * Author			: Bryan Wilcutt bwilcutt@yahoo.com
 * Date				: 5-13-18
 * System			: Nano PI
 * File				: novusagent.h
 *
 * Description		:
 *
 * This file contains support constructs for most other source files.  It defines
 * the SNMP MIB and handlers.
 *
 * Written for Novus Power.
 *
 * Copyright (c) Novus Power All Rights Reserved
 *****************************************************************************/
#ifndef NOVUSAGENT_H
#define NOVUSAGENT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <semaphore.h>

/* function declarations */

void init_novus(void);

Netsnmp_Node_Handler handle_nsFaultGPS1Lock;
Netsnmp_Node_Handler handle_nsFaultGPS2Lock;
Netsnmp_Node_Handler handle_nsFaultSatView1;
Netsnmp_Node_Handler handle_nsFaultSatView2;
Netsnmp_Node_Handler handle_nsFaultChannelBytes;
Netsnmp_Node_Handler handle_nsFaultPowerSupplyByte;
Netsnmp_Node_Handler handle_nsFaultErrMsgByte;
Netsnmp_Node_Handler handle_nsFaultAnt1Stat;
Netsnmp_Node_Handler handle_nsFaultAnt2Stat;
Netsnmp_Node_Handler handle_nsChannel1Vrms;
Netsnmp_Node_Handler handle_nsChannel2Vrms;
Netsnmp_Node_Handler handle_nsChannel3Vrms;
Netsnmp_Node_Handler handle_nsChannel4Vrms;
Netsnmp_Node_Handler handle_nsChannel5Vrms;
Netsnmp_Node_Handler handle_nsChannel6Vrms;
Netsnmp_Node_Handler handle_nsChannel7Vrms;
Netsnmp_Node_Handler handle_nsChannel8Vrms;
Netsnmp_Node_Handler handle_nsChannel9Vrms;
Netsnmp_Node_Handler handle_nsChannel10Vrms;
Netsnmp_Node_Handler handle_nsChannel11Vrms;
Netsnmp_Node_Handler handle_nsChannel12Vrms;
Netsnmp_Node_Handler handle_nsChannel13Vrms;
Netsnmp_Node_Handler handle_nsChannel14Vrms;
Netsnmp_Node_Handler handle_nsChannel15Vrms;
Netsnmp_Node_Handler handle_nsChannel16Vrms;
Netsnmp_Node_Handler handle_nsChannel17Vrms;
Netsnmp_Node_Handler handle_nsChannel18Vrms;
Netsnmp_Node_Handler handle_nsChannel19Vrms;
Netsnmp_Node_Handler handle_nsChannel20Vrms;
Netsnmp_Node_Handler handle_nsChannel21Vrms;
Netsnmp_Node_Handler handle_nsChannel22Vrms;
Netsnmp_Node_Handler handle_nsChannel23Vrms;
Netsnmp_Node_Handler handle_nsChannel24Vrms;

Netsnmp_Node_Handler handle_nsPS1Status;
Netsnmp_Node_Handler handle_nsPS2Status;
Netsnmp_Node_Handler handle_nsPS3Status;
Netsnmp_Node_Handler handle_nsPS4Status;
Netsnmp_Node_Handler handle_nsPS5Status;
Netsnmp_Node_Handler handle_nsPS6Status;
Netsnmp_Node_Handler handle_nsPS7Status;
Netsnmp_Node_Handler handle_nsPS8Status;
Netsnmp_Node_Handler handle_nsBITStatus;
Netsnmp_Node_Handler handle_nsPSTemp;
Netsnmp_Node_Handler handle_nsSensorPotentiometer;
Netsnmp_Node_Handler handle_nsSensorFanPWM;
Netsnmp_Node_Handler handle_nsSensorTemperature;
Netsnmp_Node_Handler handle_nsSysIdentifier;
Netsnmp_Node_Handler handle_nsSysActivePCBAssy;
Netsnmp_Node_Handler handle_nsSysGNSSLock;
Netsnmp_Node_Handler handle_nsSysInputErr;
Netsnmp_Node_Handler handle_nsSysChanStatusWord;
Netsnmp_Node_Handler handle_nsSysPriPSStatus;
Netsnmp_Node_Handler handle_nsSysSecPSStatus;
Netsnmp_Node_Handler handle_nsSysActivePCBStatus;
Netsnmp_Node_Handler handle_nsSysChksumStatus;
Netsnmp_Node_Handler handle_nsSysChanFaultBin;
Netsnmp_Node_Handler handle_nsSysPriPCBAmpStatus;
Netsnmp_Node_Handler handle_nsSysBkupPCBAmpStatus;
Netsnmp_Node_Handler handle_nsSysGPSLock;
Netsnmp_Node_Handler handle_nsSysSatView;
Netsnmp_Node_Handler handle_nsSysErrorByte;
Netsnmp_Node_Handler handle_nsSysFreqDiff;
Netsnmp_Node_Handler handle_nsSysPPSDiff;
Netsnmp_Node_Handler handle_nsSysFreqCorSlice;
Netsnmp_Node_Handler handle_nsSysDACValue;
Netsnmp_Node_Handler handle_nsSysPS1VDC;
Netsnmp_Node_Handler handle_nsSysPS2VDC;
Netsnmp_Node_Handler handle_nsEventDiscCounter;
Netsnmp_Node_Handler handle_nsEventUserEnabled;
Netsnmp_Node_Handler handle_nsEventSysEnabled;
Netsnmp_Node_Handler handle_nsEventGPSLock;
Netsnmp_Node_Handler handle_nsEventRAMIndex;
Netsnmp_Node_Handler handle_nsEventTimeAlignment;
Netsnmp_Node_Handler handle_nsEventEstAccuracy;
Netsnmp_Node_Handler handle_nsEventEdgeDetDir;

Netsnmp_Node_Handler handle_nsMeasureFreq;
Netsnmp_Node_Handler handle_nsMeasureDAC;
Netsnmp_Node_Handler handle_nsMeasureAnt;
Netsnmp_Node_Handler handle_nsMeasureRMSOutput;
Netsnmp_Node_Handler handle_nsMeasureTemp;
Netsnmp_Node_Handler handle_nsMeasureHeaterTemp;

Netsnmp_Node_Handler handle_nsPPSStability;
Netsnmp_Node_Handler handle_nsPPSDiscGPS;
Netsnmp_Node_Handler handle_nsPPSOutputType;
Netsnmp_Node_Handler handle_nsPPSDifference;
Netsnmp_Node_Handler handle_nsPPSCalFactor;
Netsnmp_Node_Handler handle_nsPPSTimeCalFactor;
Netsnmp_Node_Handler handle_nsPPSFreqVar;
Netsnmp_Node_Handler handle_nsCommand;
Netsnmp_Node_Handler handle_nsResult;

Netsnmp_Node_Handler handle_nsDiscPrioritySource;
Netsnmp_Node_Handler handle_nsDiscCurrentSource;
Netsnmp_Node_Handler handle_nsDiscGNSSLock;
Netsnmp_Node_Handler handle_nsDiscRFPresent;
Netsnmp_Node_Handler handle_nsDiscOpticalPresent;
Netsnmp_Node_Handler handle_nsDiscLoopLock;

Netsnmp_Node_Handler handle_nsWarmupRemaining;
Netsnmp_Node_Handler handle_nsWarmupComplete;
Netsnmp_Node_Handler handle_nsHoldoverElapsed;
Netsnmp_Node_Handler handle_nsHoldoverValid;
Netsnmp_Node_Handler handle_nsFrequencyValid;
Netsnmp_Node_Handler handle_nsHoldoverTemp;

Netsnmp_Node_Handler handle_nsRbStatus;
Netsnmp_Node_Handler handle_nsRbAlarm;
Netsnmp_Node_Handler handle_nsRbMode;
Netsnmp_Node_Handler handle_nsRbDiscStatus;
Netsnmp_Node_Handler handle_nsRbHoldoverSource;

Netsnmp_Node_Handler handle_nsRb2Lock;
Netsnmp_Node_Handler handle_nsRb2Status;
Netsnmp_Node_Handler handle_nsRb2Steer;

#define PLEN 14
#define NS_OID_LEN 11

typedef struct
{
	char nsFaultGPS1Lock[PLEN];
	char nsFaultGPS2Lock[PLEN];
	char nsFaultSatView1[PLEN];
	char nsFaultSatView2[PLEN];
	char nsFaultChannelBytes[PLEN];
	char nsFaultPowerSupplyByte[PLEN];
	char nsFaultErrMsgByte[PLEN];
	char nsFaultAnt1Stat[PLEN];
	char nsFaultAnt2Stat[PLEN];
	char nsChannel1Vrms[PLEN];
	char nsChannel2Vrms[PLEN];
	char nsChannel3Vrms[PLEN];
	char nsChannel4Vrms[PLEN];
	char nsChannel5Vrms[PLEN];
	char nsChannel6Vrms[PLEN];
	char nsChannel7Vrms[PLEN];
	char nsChannel8Vrms[PLEN];
	char nsChannel9Vrms[PLEN];
	char nsChannel10Vrms[PLEN];
	char nsChannel11Vrms[PLEN];
	char nsChannel12Vrms[PLEN];
	char nsChannel13Vrms[PLEN];
	char nsChannel14Vrms[PLEN];
	char nsChannel15Vrms[PLEN];
	char nsChannel16Vrms[PLEN];
	char nsChannel17Vrms[PLEN];
	char nsChannel18Vrms[PLEN];
	char nsChannel19Vrms[PLEN];
	char nsChannel20Vrms[PLEN];
	char nsChannel21Vrms[PLEN];
	char nsChannel22Vrms[PLEN];
	char nsChannel23Vrms[PLEN];
	char nsChannel24Vrms[PLEN];

	char nsPS1Status[PLEN];
	char nsPS2Status[PLEN];
	char nsPS3Status[PLEN];
	char nsPS4Status[PLEN];
	char nsPS5Status[PLEN];
	char nsPS6Status[PLEN];
	char nsPS7Status[PLEN];
	char nsPS8Status[PLEN];
	char nsBITStatus[PLEN];
	char nsPSTemp[PLEN];
	char nsSensorPotentiometer[PLEN];
	char nsSensorFanPWM[PLEN];
	char nsSensorTemperature[PLEN];
	char nsSysIdentifier[80];
	char nsSysActivePCBAssy[PLEN];
	char nsSysGNSSLock[PLEN];
	char nsSysInputErr[PLEN];
	char nsSysChanStatusWord[PLEN];
	char nsSysPriPSStatus[PLEN];
	char nsSysSecPSStatus[PLEN];
	char nsSysActivePCBStatus[PLEN];
	char nsSysChksumStatus[PLEN];
	char nsSysChanFaultBin[PLEN];
	char nsSysPriPCBAmpStatus[PLEN];
	char nsSysBkupPCBAmpStatus[PLEN];
	char nsSysGPSLock[PLEN];
	char nsSysSatView[PLEN];
	char nsSysErrorByte[PLEN];
	char nsSysFreqDiff[PLEN];
	char nsSysPPSDiff[PLEN];
	char nsSysFreqCorSlice[PLEN];
	char nsSysDACValue[PLEN];
	char nsSysPS1VDC[PLEN];
	char nsSysPS2VDC[PLEN];
	char nsEventDiscCounter[PLEN];
	char nsEventUserEnabled[PLEN];
	char nsEventSysEnabled[PLEN];
	char nsEventGPSLock[PLEN];
	char nsEventRAMIndex[PLEN];
	char nsEventTimeAlignment[PLEN];
	char nsEventEstAccuracy[PLEN];
	char nsEventEdgeDetDir[PLEN];

	char nsMeasureFreq[PLEN];
	char nsMeasureDAC[PLEN];
	char nsMeasureAnt[PLEN];
	char nsMeasureRMSOutput[PLEN];
	char nsMeasureTemp[PLEN];
	char nsMeasureHeaterTemp[PLEN];

	char nsPPSStability[PLEN];
	char nsPPSDiscGPS[PLEN];
	char nsPPSOutputType[PLEN];
	char nsPPSDifference[PLEN];
	char nsPPSCalFactor[PLEN];
	char nsPPSTimeCalFactor[PLEN];
	char nsPPSFreqVar[PLEN];

	char nsDiscPrioritySource[PLEN];
	char nsDiscCurrentSource[PLEN];
	char nsDiscGNSSLock[PLEN];
	char nsDiscRFPresent[PLEN];
	char nsDiscOpticalPresent[PLEN];
	char nsDiscLoopLock[PLEN];

	char nsWarmupRemaining[PLEN];
	char nsWarmupComplete[PLEN];
	char nsHoldoverElapsed[PLEN];
	char nsHoldoverValid[PLEN];
	char nsFrequencyValid[PLEN];
	char nsHoldoverTemp[PLEN];

	char nsRbStatus[PLEN];
	char nsRbAlarm[PLEN];
	char nsRbMode[PLEN];
	char nsRbDiscStatus[PLEN];
	char nsRbHoldoverSource[PLEN];

	char nsRb2Lock[PLEN];
	char nsRb2Status[PLEN];
	char nsRb2Steer[PLEN];

} radioBlock_t;

extern radioBlock_t radio;

// Added for server page
#define WEB_DATA_LEN 16
typedef struct
{
	/* data */
	char nsTime[WEB_DATA_LEN];
	char nsDate[WEB_DATA_LEN];
} NOVUS_WEB_DATA_T;

extern NOVUS_WEB_DATA_T nvwData;

#define WEB_INPUT_MSG_LEN 255
typedef struct
{
	char msg[WEB_INPUT_MSG_LEN];
	char status;
} NOVUS_WEB_INPUT_CMD_T;
extern NOVUS_WEB_INPUT_CMD_T webCmdInput;

typedef struct
{
	char buff[sizeof(radioBlock_t) + sizeof(NOVUS_WEB_DATA_T) + sizeof(NOVUS_WEB_INPUT_CMD_T)]; /* Data being transferred */
	sem_t sem1;																					/* POSIX unnamed semaphore */
	sem_t sem2;																					/* POSIX unnamed semaphore */
	size_t cnt;																					/* Number of bytes used in 'buf' */
} SHMBUF_T;

// #define SHM_ID "/SHM_TEST"
#define SHM_STORAGE_SIZE (sizeof(SHMBUF_T))

extern const oid nsFaultGPS1Lock_oid[NS_OID_LEN];
extern const oid nsFaultGPS2Lock_oid[NS_OID_LEN];
extern const oid nsFaultSatView1_oid[NS_OID_LEN];
extern const oid nsFaultSatView2_oid[NS_OID_LEN];
extern const oid nsFaultChannelBytes_oid[NS_OID_LEN];
extern const oid nsFaultPowerSupplyByte_oid[NS_OID_LEN];
extern const oid nsFaultErrMsgByte_oid[NS_OID_LEN];
extern const oid nsFaultAnt1Stat_oid[NS_OID_LEN];
extern const oid nsFaultAnt2Stat_oid[NS_OID_LEN];
extern const oid nsTrapMsg_oid[NS_OID_LEN];
extern const oid nsChannel1Vrms_oid[NS_OID_LEN];
extern const oid nsChannel2Vrms_oid[NS_OID_LEN];
extern const oid nsChannel3Vrms_oid[NS_OID_LEN];
extern const oid nsChannel4Vrms_oid[NS_OID_LEN];
extern const oid nsChannel5Vrms_oid[NS_OID_LEN];
extern const oid nsChannel6Vrms_oid[NS_OID_LEN];
extern const oid nsChannel7Vrms_oid[NS_OID_LEN];
extern const oid nsChannel8Vrms_oid[NS_OID_LEN];
extern const oid nsChannel9Vrms_oid[NS_OID_LEN];
extern const oid nsChannel10Vrms_oid[NS_OID_LEN];
extern const oid nsChannel11Vrms_oid[NS_OID_LEN];
extern const oid nsChannel12Vrms_oid[NS_OID_LEN];
extern const oid nsChannel13Vrms_oid[NS_OID_LEN];
extern const oid nsChannel14Vrms_oid[NS_OID_LEN];
extern const oid nsChannel15Vrms_oid[NS_OID_LEN];
extern const oid nsChannel16Vrms_oid[NS_OID_LEN];
extern const oid nsChannel17Vrms_oid[NS_OID_LEN];
extern const oid nsChannel18Vrms_oid[NS_OID_LEN];
extern const oid nsChannel19Vrms_oid[NS_OID_LEN];
extern const oid nsChannel20Vrms_oid[NS_OID_LEN];
extern const oid nsChannel21Vrms_oid[NS_OID_LEN];
extern const oid nsChannel22Vrms_oid[NS_OID_LEN];
extern const oid nsChannel23Vrms_oid[NS_OID_LEN];
extern const oid nsChannel24Vrms_oid[NS_OID_LEN];

extern const oid nsPS1Status_oid[NS_OID_LEN];
extern const oid nsPS2Status_oid[NS_OID_LEN];
extern const oid nsPS3Status_oid[NS_OID_LEN];
extern const oid nsPS4Status_oid[NS_OID_LEN];
extern const oid nsPS5Status_oid[NS_OID_LEN];
extern const oid nsPS6Status_oid[NS_OID_LEN];
extern const oid nsPS7Status_oid[NS_OID_LEN];
extern const oid nsPS8Status_oid[NS_OID_LEN];
extern const oid nsBITStatus_oid[NS_OID_LEN];
extern const oid nsPSTemp_oid[NS_OID_LEN];
extern const oid nsSensorPotentiometer_oid[NS_OID_LEN];
extern const oid nsSensorFanPWM_oid[NS_OID_LEN];
extern const oid nsSensorTemperature_oid[NS_OID_LEN];
extern const oid nsSysIdentifier_oid[NS_OID_LEN];
extern const oid nsSysActivePCBAssy_oid[NS_OID_LEN];
extern const oid nsSysGNSSLock_oid[NS_OID_LEN];
extern const oid nsSysInputErr_oid[NS_OID_LEN];
extern const oid nsSysChanStatusWord_oid[NS_OID_LEN];
extern const oid nsSysPriPSStatus_oid[NS_OID_LEN];
extern const oid nsSysSecPSStatus_oid[NS_OID_LEN];
extern const oid nsSysActivePCBStatus_oid[NS_OID_LEN];
extern const oid nsSysChksumStatus_oid[NS_OID_LEN];
extern const oid nsSysChanFaultBin_oid[NS_OID_LEN];
extern const oid nsSysPriPCBAmpStatus_oid[NS_OID_LEN];
extern const oid nsSysBkupPCBAmpStatus_oid[NS_OID_LEN];
extern const oid nsSysGPSLock_oid[NS_OID_LEN];
extern const oid nsSysSatView_oid[NS_OID_LEN];
extern const oid nsSysErrorByte_oid[NS_OID_LEN];
extern const oid nsSysFreqDiff_oid[NS_OID_LEN];
extern const oid nsSysPPSDiff_oid[NS_OID_LEN];
extern const oid nsSysFreqCorSlice_oid[NS_OID_LEN];
extern const oid nsSysDACValue_oid[NS_OID_LEN];
extern const oid nsSysPS1VDC_oid[NS_OID_LEN];
extern const oid nsSysPS2VDC_oid[NS_OID_LEN];
extern const oid nsEventDiscCounter_oid[NS_OID_LEN];
extern const oid nsEventUserEnabled_oid[NS_OID_LEN];
extern const oid nsEventSysEnabled_oid[NS_OID_LEN];
extern const oid nsEventGPSLock_oid[NS_OID_LEN];
extern const oid nsEventRAMIndex_oid[NS_OID_LEN];
extern const oid nsEventTimeAlignment_oid[NS_OID_LEN];
extern const oid nsEventEstAccuracy_oid[NS_OID_LEN];
extern const oid nsEventEdgeDetDir_oid[NS_OID_LEN];

extern const oid nsMeasureFreq_oid[NS_OID_LEN];
extern const oid nsMeasureDAC_oid[NS_OID_LEN];
extern const oid nsMeasureAnt_oid[NS_OID_LEN];
extern const oid nsMeasureRMSOutput_oid[NS_OID_LEN];
extern const oid nsMeasureTemp_oid[NS_OID_LEN];
extern const oid nsMeasureHeaterTemp_oid[NS_OID_LEN];

extern const oid nsPPSStability_oid[NS_OID_LEN];
extern const oid nsPPSDiscGPS_oid[NS_OID_LEN];
extern const oid nsPPSOutputType_oid[NS_OID_LEN];
extern const oid nsPPSDifference_oid[NS_OID_LEN];
extern const oid nsPPSCalFactor_oid[NS_OID_LEN];
extern const oid nsPPSTimeCalFactor_oid[NS_OID_LEN];
extern const oid nsPPSFreqVar_oid[NS_OID_LEN];
extern const oid nsCommand_oid[NS_OID_LEN];

extern const oid nsDiscPrioritySource_oid[NS_OID_LEN];
extern const oid nsDiscCurrentSource_oid[NS_OID_LEN];
extern const oid nsDiscGNSSLock_oid[NS_OID_LEN];
extern const oid nsDiscRFPresent_oid[NS_OID_LEN];
extern const oid nsDiscOpticalPresent_oid[NS_OID_LEN];
extern const oid nsDiscLoopLock_oid[NS_OID_LEN];

extern const oid nsWarmupRemaining_oid[NS_OID_LEN];
extern const oid nsWarmupComplete_oid[NS_OID_LEN];
extern const oid nsHoldoverElapsed_oid[NS_OID_LEN];
extern const oid nsHoldoverValid_oid[NS_OID_LEN];
extern const oid nsFrequencyValid_oid[NS_OID_LEN];
extern const oid nsHoldoverTemp_oid[NS_OID_LEN];

extern const oid nsRbStatus_oid[NS_OID_LEN];
extern const oid nsRbAlarm_oid[NS_OID_LEN];
extern const oid nsRbMode_oid[NS_OID_LEN];
extern const oid nsRbDiscStatus_oid[NS_OID_LEN];
extern const oid nsRbHoldoverSource_oid[NS_OID_LEN];

extern const oid nsRb2Lock_oid[NS_OID_LEN];
extern const oid nsRb2Status_oid[NS_OID_LEN];
extern const oid nsRb2Steer_oid[NS_OID_LEN];

#endif /* NOVUS_H */
