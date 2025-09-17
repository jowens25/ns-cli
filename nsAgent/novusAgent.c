/*****************************************************************************
 * Project			: nsAgent
 * Author			: Bryan Wilcutt bwilcutt@yahoo.com
 * Date				: 5-13-18
 * System			: Nano PI
 * File				: novusAgent.c
 *
 * Description		:
 *
 * This file contains the primary definition of the NOVUS-SECURE-MIB.mib.
 * The initialization process adds individual handlers to the SNMP engine
 * as call back functions for GET operations.  Other operations, such as SET,
 * can be supported by must be added.
 *
 * Written for Novus Power.
 *
 * Copyright (c) Novus Power All Rights Reserved
 *****************************************************************************/

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "novusAgent.h"
#include "nsStartup.h"

radioBlock_t radio;
NOVUS_WEB_DATA_T nvwData;

const oid nsFaultGPS1Lock_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,1,1 };
const oid nsFaultGPS2Lock_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,1,2 };
const oid nsFaultSatView1_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,1,3 };
const oid nsFaultSatView2_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,1,4 };
const oid nsFaultChannelBytes_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,1,5 };
const oid nsFaultPowerSupplyByte_oid[] = 	{ 1,3,6,1,4,1,9183,1,1,1,6 };
const oid nsFaultErrMsgByte_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,1,7 };
const oid nsFaultAnt1Stat_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,1,8 };
const oid nsFaultAnt2Stat_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,1,9 };

const oid nsTrapMsg_oid[]    =         		{ 1,3,6,1,4,1,9183,1,1,1,10 }; // No handler necessary

const oid nsChannel1Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,1 };
const oid nsChannel2Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,2 };
const oid nsChannel3Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,3 };
const oid nsChannel4Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,4 };
const oid nsChannel5Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,5 };
const oid nsChannel6Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,6 };
const oid nsChannel7Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,7 };
const oid nsChannel8Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,8 };
const oid nsChannel9Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,9 };
const oid nsChannel10Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,10 };
const oid nsChannel11Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,11 };
const oid nsChannel12Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,12 };
const oid nsChannel13Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,13 };
const oid nsChannel14Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,14 };
const oid nsChannel15Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,15 };
const oid nsChannel16Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,16 };
const oid nsChannel17Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,17 };
const oid nsChannel18Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,18 };
const oid nsChannel19Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,19 };
const oid nsChannel20Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,20 };
const oid nsChannel21Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,21 };
const oid nsChannel22Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,22 };
const oid nsChannel23Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,23 };
const oid nsChannel24Vrms_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,2,24 };




const oid nsPS1Status_oid[] =				{ 1,3,6,1,4,1,9183,1,1,3,1 };
const oid nsPS2Status_oid[] =	 			{ 1,3,6,1,4,1,9183,1,1,3,2 };
const oid nsPS3Status_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,3,3 };
const oid nsPS4Status_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,3,4 };
const oid nsPS5Status_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,3,5 };
const oid nsPS6Status_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,3,6 };
const oid nsPS7Status_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,3,7 };
const oid nsPS8Status_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,3,8 };
const oid nsBITStatus_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,3,9 };
const oid nsPSTemp_oid[] = 					{ 1,3,6,1,4,1,9183,1,1,3,10 };

const oid nsSensorPotentiometer_oid[] = 	{ 1,3,6,1,4,1,9183,1,1,4,1 };
const oid nsSensorFanPWM_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,4,2 };
const oid nsSensorTemperature_oid[] =		{ 1,3,6,1,4,1,9183,1,1,4,3 };

const oid nsSysIdentifier_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,5,1 };
const oid nsSysActivePCBAssy_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,5,2 };
const oid nsSysGNSSLock_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,5,3 };
const oid nsSysInputErr_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,5,4 };
const oid nsSysChanStatusWord_oid[] =		{ 1,3,6,1,4,1,9183,1,1,5,5 };
const oid nsSysPriPSStatus_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,5,6 };
const oid nsSysSecPSStatus_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,5,7 };
const oid nsSysActivePCBStatus_oid[] =		{ 1,3,6,1,4,1,9183,1,1,5,8 };
const oid nsSysChksumStatus_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,5,9 };
const oid nsSysChanFaultBin_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,5,10 };
const oid nsSysPriPCBAmpStatus_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,5,11 };
const oid nsSysBkupPCBAmpStatus_oid[] = 	{ 1,3,6,1,4,1,9183,1,1,5,12 };
const oid nsSysGPSLock_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,5,13 };
const oid nsSysSatView_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,5,14 };
const oid nsSysErrorByte_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,5,15 };
const oid nsSysFreqDiff_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,5,16 };
const oid nsSysPPSDiff_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,5,17 };
const oid nsSysFreqCorSlice_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,5,18 };
const oid nsSysDACValue_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,5,19 };
const oid nsSysPS1VDC_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,5,20 };
const oid nsSysPS2VDC_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,5,21 };

const oid nsEventDiscCounter_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,6,1 };
const oid nsEventUserEnabled_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,6,2 };
const oid nsEventSysEnabled_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,6,3 };
const oid nsEventGPSLock_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,6,4 };
const oid nsEventRAMIndex_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,6,5 };
const oid nsEventTimeAlignment_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,6,6 };
const oid nsEventEstAccuracy_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,6,7 };
const oid nsEventEdgeDetDir_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,6,8 };

const oid nsMeasureFreq_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,7,1 };
const oid nsMeasureDAC_oid[] = 			    { 1,3,6,1,4,1,9183,1,1,7,2 };
const oid nsMeasureAnt_oid[] = 			    { 1,3,6,1,4,1,9183,1,1,7,3 };
const oid nsMeasureRMSOutput_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,7,4 };
const oid nsMeasureTemp_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,7,5 };
const oid nsMeasureHeaterTemp_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,7,6 };





const oid nsPPSStability_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,8,1 };
const oid nsPPSDiscGPS_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,8,2 };
const oid nsPPSOutputType_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,8,3 };
const oid nsPPSDifference_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,8,4 };
const oid nsPPSCalFactor_oid[] = 			{ 1,3,6,1,4,1,9183,1,1,8,5 };
const oid nsPPSTimeCalFactor_oid[] = 		{ 1,3,6,1,4,1,9183,1,1,8,6 };
const oid nsPPSFreqVar_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,8,7 };

const oid nsCommand_oid[] = 				{ 1,3,6,1,4,1,9183,1,1,9,1 };
const oid nsResult_oid[] = 					{ 1,3,6,1,4,1,9183,1,1,9,2 };

const oid nsDiscPrioritySource_oid[] =           { 1,3,6,1,4,1,9183,1,1,10,1 };
const oid nsDiscCurrentSource_oid[] =            { 1,3,6,1,4,1,9183,1,1,10,2 };
const oid nsDiscGNSSLock_oid[]  =                { 1,3,6,1,4,1,9183,1,1,10,3 };
const oid nsDiscRFPresent_oid[]  =               { 1,3,6,1,4,1,9183,1,1,10,4 };
const oid nsDiscOpticalPresent_oid[]   =         { 1,3,6,1,4,1,9183,1,1,10,5 };
const oid nsDiscLoopLock_oid[]  =                { 1,3,6,1,4,1,9183,1,1,10,6 };

const oid nsWarmupRemaining_oid[]  =         { 1,3,6,1,4,1,9183,1,1,11,1 };
const oid nsWarmupComplete_oid[]  =          { 1,3,6,1,4,1,9183,1,1,11,2 };
const oid nsHoldoverElapsed_oid[] =          { 1,3,6,1,4,1,9183,1,1,11,3 };
const oid nsHoldoverValid_oid[]   =          { 1,3,6,1,4,1,9183,1,1,11,4 };
const oid nsFrequencyValid_oid[]  =          { 1,3,6,1,4,1,9183,1,1,11,5 };
const oid nsHoldoverTemp_oid[]  =            { 1,3,6,1,4,1,9183,1,1,11,6 };

const oid nsRbStatus_oid[]   =               { 1,3,6,1,4,1,9183,1,1,12,1 };
const oid nsRbAlarm_oid[]   =                { 1,3,6,1,4,1,9183,1,1,12,2 };
const oid nsRbMode_oid[]    =                { 1,3,6,1,4,1,9183,1,1,12,3 };
const oid nsRbDiscStatus_oid[]  =            { 1,3,6,1,4,1,9183,1,1,12,4 };
const oid nsRbHoldoverSource_oid[]   =       { 1,3,6,1,4,1,9183,1,1,12,5 };

const oid nsRb2Lock_oid[]   =               { 1,3,6,1,4,1,9183,1,1,13,1 };
const oid nsRb2Status_oid[]   =              { 1,3,6,1,4,1,9183,1,1,13,2 };
const oid nsRb2Steer_oid[]   =               { 1,3,6,1,4,1,9183,1,1,13,3 };



 /************************************************
  * Function       : init_novus
  * Input          : void
  * Output         : void
  * Description    : Initializes the SNMP handlers.
  ************************************************/
void init_novus(void)
{
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsFaultGPS1Lock", handle_nsFaultGPS1Lock,
                               nsFaultGPS1Lock_oid, OID_LENGTH(nsFaultGPS1Lock_oid),
                               HANDLER_CAN_RONLY
        ));

    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsFaultGPS2Lock", handle_nsFaultGPS2Lock,
                               nsFaultGPS2Lock_oid, OID_LENGTH(nsFaultGPS2Lock_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsFaultSatView1", handle_nsFaultSatView1,
                               nsFaultSatView1_oid, OID_LENGTH(nsFaultSatView1_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsFaultSatView2", handle_nsFaultSatView2,
                               nsFaultSatView2_oid, OID_LENGTH(nsFaultSatView2_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsFaultChannelBytes", handle_nsFaultChannelBytes,
                               nsFaultChannelBytes_oid, OID_LENGTH(nsFaultChannelBytes_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsFaultPowerSupplyByte", handle_nsFaultPowerSupplyByte,
                               nsFaultPowerSupplyByte_oid, OID_LENGTH(nsFaultPowerSupplyByte_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsFaultErrMsgByte", handle_nsFaultErrMsgByte,
                               nsFaultErrMsgByte_oid, OID_LENGTH(nsFaultErrMsgByte_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsFaultAnt1Stat", handle_nsFaultAnt1Stat,
                               nsFaultAnt1Stat_oid, OID_LENGTH(nsFaultAnt1Stat_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsFaultAnt2Stat", handle_nsFaultAnt2Stat,
                               nsFaultAnt2Stat_oid, OID_LENGTH(nsFaultAnt2Stat_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel1Vrms", handle_nsChannel1Vrms,
                               nsChannel1Vrms_oid, OID_LENGTH(nsChannel1Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel2Vrms", handle_nsChannel2Vrms,
                               nsChannel2Vrms_oid, OID_LENGTH(nsChannel2Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel3Vrms", handle_nsChannel3Vrms,
                               nsChannel3Vrms_oid, OID_LENGTH(nsChannel3Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel4Vrms", handle_nsChannel4Vrms,
                               nsChannel4Vrms_oid, OID_LENGTH(nsChannel4Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel5Vrms", handle_nsChannel5Vrms,
                               nsChannel5Vrms_oid, OID_LENGTH(nsChannel5Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel6Vrms", handle_nsChannel6Vrms,
                               nsChannel6Vrms_oid, OID_LENGTH(nsChannel6Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel7Vrms", handle_nsChannel7Vrms,
                               nsChannel7Vrms_oid, OID_LENGTH(nsChannel7Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel8Vrms", handle_nsChannel8Vrms,
                               nsChannel8Vrms_oid, OID_LENGTH(nsChannel8Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel9Vrms", handle_nsChannel9Vrms,
                               nsChannel9Vrms_oid, OID_LENGTH(nsChannel9Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel10Vrms", handle_nsChannel10Vrms,
                               nsChannel10Vrms_oid, OID_LENGTH(nsChannel10Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel11Vrms", handle_nsChannel11Vrms,
                               nsChannel11Vrms_oid, OID_LENGTH(nsChannel11Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel12Vrms", handle_nsChannel12Vrms,
                               nsChannel12Vrms_oid, OID_LENGTH(nsChannel12Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel13Vrms", handle_nsChannel13Vrms,
                               nsChannel13Vrms_oid, OID_LENGTH(nsChannel13Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel14Vrms", handle_nsChannel14Vrms,
                               nsChannel14Vrms_oid, OID_LENGTH(nsChannel14Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel15Vrms", handle_nsChannel15Vrms,
                               nsChannel15Vrms_oid, OID_LENGTH(nsChannel15Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel16Vrms", handle_nsChannel16Vrms,
                               nsChannel16Vrms_oid, OID_LENGTH(nsChannel16Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel17Vrms", handle_nsChannel17Vrms,
                               nsChannel17Vrms_oid, OID_LENGTH(nsChannel17Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel18Vrms", handle_nsChannel18Vrms,
                               nsChannel18Vrms_oid, OID_LENGTH(nsChannel18Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel19Vrms", handle_nsChannel19Vrms,
                               nsChannel19Vrms_oid, OID_LENGTH(nsChannel19Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel20Vrms", handle_nsChannel20Vrms,
                               nsChannel20Vrms_oid, OID_LENGTH(nsChannel20Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel21Vrms", handle_nsChannel21Vrms,
                               nsChannel21Vrms_oid, OID_LENGTH(nsChannel21Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel22Vrms", handle_nsChannel22Vrms,
                               nsChannel22Vrms_oid, OID_LENGTH(nsChannel22Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel23Vrms", handle_nsChannel23Vrms,
                               nsChannel23Vrms_oid, OID_LENGTH(nsChannel23Vrms_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsChannel24Vrms", handle_nsChannel24Vrms,
                               nsChannel24Vrms_oid, OID_LENGTH(nsChannel24Vrms_oid),
                               HANDLER_CAN_RONLY
        ));



    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPS1Status", handle_nsPS1Status,
                               nsPS1Status_oid, OID_LENGTH(nsPS1Status_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPS2Status", handle_nsPS2Status,
                               nsPS2Status_oid, OID_LENGTH(nsPS2Status_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPS3Status", handle_nsPS3Status,
                               nsPS3Status_oid, OID_LENGTH(nsPS3Status_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPS4Status", handle_nsPS4Status,
                               nsPS4Status_oid, OID_LENGTH(nsPS4Status_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPS5Status", handle_nsPS5Status,
                               nsPS5Status_oid, OID_LENGTH(nsPS5Status_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPS6Status", handle_nsPS6Status,
                               nsPS6Status_oid, OID_LENGTH(nsPS6Status_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPS7Status", handle_nsPS7Status,
                               nsPS7Status_oid, OID_LENGTH(nsPS7Status_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPS8Status", handle_nsPS8Status,
                               nsPS8Status_oid, OID_LENGTH(nsPS8Status_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsBITStatus", handle_nsBITStatus,
                               nsBITStatus_oid, OID_LENGTH(nsBITStatus_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPSTemp", handle_nsPSTemp,
                               nsPSTemp_oid, OID_LENGTH(nsPSTemp_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSensorPotentiometer", handle_nsSensorPotentiometer,
                               nsSensorPotentiometer_oid, OID_LENGTH(nsSensorPotentiometer_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSensorFanPWM", handle_nsSensorFanPWM,
                               nsSensorFanPWM_oid, OID_LENGTH(nsSensorFanPWM_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSensorTemperature", handle_nsSensorTemperature,
                               nsSensorTemperature_oid, OID_LENGTH(nsSensorTemperature_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysIdentifier", handle_nsSysIdentifier,
                               nsSysIdentifier_oid, OID_LENGTH(nsSysIdentifier_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysActivePCBAssy", handle_nsSysActivePCBAssy,
                               nsSysActivePCBAssy_oid, OID_LENGTH(nsSysActivePCBAssy_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysGNSSLock", handle_nsSysGNSSLock,
                               nsSysGNSSLock_oid, OID_LENGTH(nsSysGNSSLock_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysInputErr", handle_nsSysInputErr,
                               nsSysInputErr_oid, OID_LENGTH(nsSysInputErr_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysChanStatusWord", handle_nsSysChanStatusWord,
                               nsSysChanStatusWord_oid, OID_LENGTH(nsSysChanStatusWord_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysPriPSStatus", handle_nsSysPriPSStatus,
                               nsSysPriPSStatus_oid, OID_LENGTH(nsSysPriPSStatus_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysSecPSStatus", handle_nsSysSecPSStatus,
                               nsSysSecPSStatus_oid, OID_LENGTH(nsSysSecPSStatus_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysActivePCBStatus", handle_nsSysActivePCBStatus,
                               nsSysActivePCBStatus_oid, OID_LENGTH(nsSysActivePCBStatus_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysChksumStatus", handle_nsSysChksumStatus,
                               nsSysChksumStatus_oid, OID_LENGTH(nsSysChksumStatus_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysChanFaultBin", handle_nsSysChanFaultBin,
                               nsSysChanFaultBin_oid, OID_LENGTH(nsSysChanFaultBin_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysPriPCBAmpStatus", handle_nsSysPriPCBAmpStatus,
                               nsSysPriPCBAmpStatus_oid, OID_LENGTH(nsSysPriPCBAmpStatus_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysBkupPCBAmpStatus", handle_nsSysBkupPCBAmpStatus,
                               nsSysBkupPCBAmpStatus_oid, OID_LENGTH(nsSysBkupPCBAmpStatus_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysGPSLock", handle_nsSysGPSLock,
                               nsSysGPSLock_oid, OID_LENGTH(nsSysGPSLock_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysSatView", handle_nsSysSatView,
                               nsSysSatView_oid, OID_LENGTH(nsSysSatView_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysErrorByte", handle_nsSysErrorByte,
                               nsSysErrorByte_oid, OID_LENGTH(nsSysErrorByte_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysFreqDiff", handle_nsSysFreqDiff,
                               nsSysFreqDiff_oid, OID_LENGTH(nsSysFreqDiff_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysPPSDiff", handle_nsSysPPSDiff,
                               nsSysPPSDiff_oid, OID_LENGTH(nsSysPPSDiff_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysFreqCorSlice", handle_nsSysFreqCorSlice,
                               nsSysFreqCorSlice_oid, OID_LENGTH(nsSysFreqCorSlice_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysDACValue", handle_nsSysDACValue,
                               nsSysDACValue_oid, OID_LENGTH(nsSysDACValue_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysPS1VDC", handle_nsSysPS1VDC,
                               nsSysPS1VDC_oid, OID_LENGTH(nsSysPS1VDC_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsSysPS2VDC", handle_nsSysPS2VDC,
                               nsSysPS2VDC_oid, OID_LENGTH(nsSysPS2VDC_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsEventDiscCounter", handle_nsEventDiscCounter,
                               nsEventDiscCounter_oid, OID_LENGTH(nsEventDiscCounter_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsEventUserEnabled", handle_nsEventUserEnabled,
                               nsEventUserEnabled_oid, OID_LENGTH(nsEventUserEnabled_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsEventSysEnabled", handle_nsEventSysEnabled,
                               nsEventSysEnabled_oid, OID_LENGTH(nsEventSysEnabled_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsEventGPSLock", handle_nsEventGPSLock,
                               nsEventGPSLock_oid, OID_LENGTH(nsEventGPSLock_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsEventRAMIndex", handle_nsEventRAMIndex,
                               nsEventRAMIndex_oid, OID_LENGTH(nsEventRAMIndex_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsEventTimeAlignment", handle_nsEventTimeAlignment,
                               nsEventTimeAlignment_oid, OID_LENGTH(nsEventTimeAlignment_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsEventEstAccuracy", handle_nsEventEstAccuracy,
                               nsEventEstAccuracy_oid, OID_LENGTH(nsEventEstAccuracy_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsEventEdgeDetDir", handle_nsEventEdgeDetDir,
                               nsEventEdgeDetDir_oid, OID_LENGTH(nsEventEdgeDetDir_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsMeasureFreq", handle_nsMeasureFreq,
                               nsMeasureFreq_oid, OID_LENGTH(nsMeasureFreq_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsMeasureDAC", handle_nsMeasureDAC,
                               nsMeasureDAC_oid, OID_LENGTH(nsMeasureDAC_oid),
                               HANDLER_CAN_RONLY
        ));

    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsMeasureAnt", handle_nsMeasureAnt,
                               nsMeasureAnt_oid, OID_LENGTH(nsMeasureAnt_oid),
                               HANDLER_CAN_RONLY
        ));

    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsMeasureRMSOutput", handle_nsMeasureRMSOutput,
                               nsMeasureRMSOutput_oid, OID_LENGTH(nsMeasureRMSOutput_oid),
                               HANDLER_CAN_RONLY
        ));

    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsMeasureTemp", handle_nsMeasureTemp,
                               nsMeasureTemp_oid, OID_LENGTH(nsMeasureTemp_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsMeasureHeaterTemp", handle_nsMeasureHeaterTemp,
                               nsMeasureHeaterTemp_oid, OID_LENGTH(nsMeasureHeaterTemp_oid),
                               HANDLER_CAN_RONLY
        ));


    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPPSStability", handle_nsPPSStability,
                               nsPPSStability_oid, OID_LENGTH(nsPPSStability_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPPSDiscGPS", handle_nsPPSDiscGPS,
                               nsPPSDiscGPS_oid, OID_LENGTH(nsPPSDiscGPS_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPPSOutputType", handle_nsPPSOutputType,
                               nsPPSOutputType_oid, OID_LENGTH(nsPPSOutputType_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPPSDifference", handle_nsPPSDifference,
                               nsPPSDifference_oid, OID_LENGTH(nsPPSDifference_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPPSCalFactor", handle_nsPPSCalFactor,
                               nsPPSCalFactor_oid, OID_LENGTH(nsPPSCalFactor_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPPSTimeCalFactor", handle_nsPPSTimeCalFactor,
                               nsPPSTimeCalFactor_oid, OID_LENGTH(nsPPSTimeCalFactor_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsPPSFreqVar", handle_nsPPSFreqVar,
                               nsPPSFreqVar_oid, OID_LENGTH(nsPPSFreqVar_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
    	netsnmp_create_handler_registration("nsCommand", handle_nsCommand,
								nsCommand_oid, OID_LENGTH(nsCommand_oid),
								HANDLER_CAN_SET_ONLY
		));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsResult", handle_nsResult,
                               nsResult_oid, OID_LENGTH(nsResult_oid),
                               HANDLER_CAN_RONLY
        ));




    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsDiscPrioritySource", handle_nsDiscPrioritySource,
                               nsDiscPrioritySource_oid, OID_LENGTH(nsDiscPrioritySource_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsDiscCurrentSource", handle_nsDiscCurrentSource,
                               nsDiscCurrentSource_oid, OID_LENGTH(nsDiscCurrentSource_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsDiscGNSSLock", handle_nsDiscGNSSLock,
                               nsDiscGNSSLock_oid, OID_LENGTH(nsDiscGNSSLock_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsDiscRFPresent", handle_nsDiscRFPresent,
                               nsDiscRFPresent_oid, OID_LENGTH(nsDiscRFPresent_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsDiscOpticalPresent", handle_nsDiscOpticalPresent,
                               nsDiscOpticalPresent_oid, OID_LENGTH(nsDiscOpticalPresent_oid),
                               HANDLER_CAN_RONLY
        ));
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsDiscLoopLock", handle_nsDiscLoopLock,
                               nsDiscLoopLock_oid, OID_LENGTH(nsDiscLoopLock_oid),
                               HANDLER_CAN_RONLY
        ));



    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsWarmupRemaining", handle_nsWarmupRemaining,
                               nsWarmupRemaining_oid, OID_LENGTH(nsWarmupRemaining_oid),
                               HANDLER_CAN_RONLY
        ));    
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsWarmupComplete", handle_nsWarmupComplete,
                               nsWarmupComplete_oid, OID_LENGTH(nsWarmupComplete_oid),
                               HANDLER_CAN_RONLY
        ));    
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsHoldoverElapsed", handle_nsHoldoverElapsed,
                               nsHoldoverElapsed_oid, OID_LENGTH(nsHoldoverElapsed_oid),
                               HANDLER_CAN_RONLY
        ));    
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsHoldoverValid", handle_nsHoldoverValid,
                               nsHoldoverValid_oid, OID_LENGTH(nsHoldoverValid_oid),
                               HANDLER_CAN_RONLY
        ));  
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsFrequencyValid", handle_nsFrequencyValid,
                               nsFrequencyValid_oid, OID_LENGTH(nsFrequencyValid_oid),
                               HANDLER_CAN_RONLY
        ));  
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsHoldoverTemp", handle_nsHoldoverTemp,
                               nsHoldoverTemp_oid, OID_LENGTH(nsHoldoverTemp_oid),
                               HANDLER_CAN_RONLY
        ));  


    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsRbStatus", handle_nsRbStatus,
                               nsRbStatus_oid, OID_LENGTH(nsRbStatus_oid),
                               HANDLER_CAN_RONLY
        ));  
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsRbAlarm", handle_nsRbAlarm,
                               nsRbAlarm_oid, OID_LENGTH(nsRbAlarm_oid),
                               HANDLER_CAN_RONLY
        ));  
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsRbMode", handle_nsRbMode,
                               nsRbMode_oid, OID_LENGTH(nsRbMode_oid),
                               HANDLER_CAN_RONLY
        ));  
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsRbDiscStatus", handle_nsRbDiscStatus,
                               nsRbDiscStatus_oid, OID_LENGTH(nsRbDiscStatus_oid),
                               HANDLER_CAN_RONLY
        ));  
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsRbHoldoverSource", handle_nsRbHoldoverSource,
                               nsRbHoldoverSource_oid, OID_LENGTH(nsRbHoldoverSource_oid),
                               HANDLER_CAN_RONLY
        ));  



    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsRb2Lock", handle_nsRb2Lock,
                               nsRb2Lock_oid, OID_LENGTH(nsRb2Lock_oid),
                               HANDLER_CAN_RONLY
        ));  
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsRb2Status", handle_nsRb2Status,
                               nsRb2Status_oid, OID_LENGTH(nsRb2Status_oid),
                               HANDLER_CAN_RONLY
        ));  
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("nsRb2Steer", handle_nsRb2Steer,
                               nsRb2Steer_oid, OID_LENGTH(nsRb2Steer_oid),
                               HANDLER_CAN_RONLY
        ));  

}

/************************************************
 * Function       : handle_nsFaultGPS1Lock
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsFaultGPS1Lock(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
  
    switch(reqinfo->mode) {

        case MODE_GET:
        	// A = Valid, V = Not Valid, N = N/A

        	if (radio.nsFaultGPS1Lock[0] == 'A')
        		val = 0;
        	else if (radio.nsFaultGPS1Lock[1] == 'V')
        		val = 1;
        	else
        		val = 2;

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsFaultGPS1Lock\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsFaultGPS2Lock
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsFaultGPS2Lock(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	// A = Valid, V = Not Valid, N = N/A

        	if (radio.nsFaultGPS2Lock[0] == 'A')
        		val = 0;
        	else if (radio.nsFaultGPS2Lock[1] == 'V')
        		val = 1;
        	else
        		val = 2;

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsFaultGPS2Lock\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
 /************************************************
  * Function       : handle_nsFaultSatView1
  * Input          : netsnmp_mib_handler ptr to handler
  * 					netsnmp_handler_registration ptr to reg
  * 					netsnmp_agent_request_info ptr to agent info
  * 					netsnmp_request_info ptr to current request
  * Output         : SNMP_ERR_xxxxxxx
  * Description    : Handler for OID.
  ************************************************/
int handle_nsFaultSatView1(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsFaultSatView1);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsFaultSatView1\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsFaultSatView2
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsFaultSatView2(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsFaultSatView2);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsFaultSatView2\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsFaultChannelBytes
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsFaultChannelBytes(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */

    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, &radio.nsFaultChannelBytes, strlen(radio.nsFaultChannelBytes));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsFaultChannelBytes\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsFaultPowerSupplyByte
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsFaultPowerSupplyByte(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, &radio.nsFaultPowerSupplyByte, strlen(radio.nsFaultPowerSupplyByte));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsFaultPowerSupplyByte\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsFaultErrMsgByte
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsFaultErrMsgByte(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, &radio.nsFaultErrMsgByte, strlen(radio.nsFaultErrMsgByte));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsFaultErrMsgByte\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsFaultAnt1Stat
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsFaultAnt1Stat(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val = 0;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	if (radio.nsFaultAnt1Stat[0] != 'N')
        		val = atoi(radio.nsFaultAnt1Stat);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsFaultAnt1Stat\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsFaultAnt2Stat
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsFaultAnt2Stat(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val = 0;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	if (radio.nsFaultAnt1Stat[0] != 'N')
        		val = atoi(radio.nsFaultAnt1Stat);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsFaultAnt2Stat\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel1Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel1Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel1Vrms,
					strlen(radio.nsChannel1Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel1Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel2Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel2Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel2Vrms,
					strlen(radio.nsChannel2Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel2Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel3Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel3Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel3Vrms,
					strlen(radio.nsChannel3Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel3Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel4Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel4Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel4Vrms,
					strlen(radio.nsChannel4Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel4Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel5Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel5Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel5Vrms,
					strlen(radio.nsChannel5Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel5Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel6Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel6Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel6Vrms,
					strlen(radio.nsChannel6Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel6Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel7Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel7Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel7Vrms,
					strlen(radio.nsChannel7Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel7Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel8Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel8Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel8Vrms,
					strlen(radio.nsChannel8Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel8Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel9Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel9Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel9Vrms,
					strlen(radio.nsChannel9Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel9Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel10Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel10Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel10Vrms,
					strlen(radio.nsChannel10Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel10Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel11Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel11Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel11Vrms,
					strlen(radio.nsChannel11Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel11Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel12Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel12Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel12Vrms,
					strlen(radio.nsChannel12Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel12Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel13Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel13Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel13Vrms,
					strlen(radio.nsChannel13Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel13Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel14Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel14Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel14Vrms,
					strlen(radio.nsChannel14Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel14Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel15Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel15Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel15Vrms,
					strlen(radio.nsChannel15Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel15Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsChannel16Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel16Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel16Vrms,
					strlen(radio.nsChannel16Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel16Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}


/************************************************
 * Function       : handle_nsChannel17Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel17Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel17Vrms,
					strlen(radio.nsChannel17Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel17Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}





/************************************************
 * Function       : handle_nsChannel18Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel18Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel18Vrms,
					strlen(radio.nsChannel18Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel18Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}








/************************************************
 * Function       : handle_nsChannel19Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel19Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel19Vrms,
					strlen(radio.nsChannel19Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel19Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}







/************************************************
 * Function       : handle_nsChannel20Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel20Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel20Vrms,
					strlen(radio.nsChannel20Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel20Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}








/************************************************
 * Function       : handle_nsChannel21Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel21Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel21Vrms,
					strlen(radio.nsChannel21Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel21Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}









/************************************************
 * Function       : handle_nsChannel22Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel22Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel22Vrms,
					strlen(radio.nsChannel22Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel22Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}









/************************************************
 * Function       : handle_nsChannel23Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel23Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel23Vrms,
					strlen(radio.nsChannel23Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel23Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}










/************************************************
 * Function       : handle_nsChannel24Vrms
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsChannel24Vrms(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsChannel24Vrms,
					strlen(radio.nsChannel24Vrms));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsChannel24Vrms\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}






/************************************************
 * Function       : handle_nsPS1Status
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPS1Status(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPS1Status,
					strlen(radio.nsPS1Status));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPS1Status\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPS2Status
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPS2Status(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPS2Status,
					strlen(radio.nsPS2Status));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPS2Status\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPS3Status
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPS3Status(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPS3Status,
					strlen(radio.nsPS3Status));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPS3Status\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPS4Status
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPS4Status(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPS4Status,
					strlen(radio.nsPS4Status));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPS4Status\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPS5Status
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPS5Status(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPS5Status,
					strlen(radio.nsPS5Status));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPS5Status\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPS6Status
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPS6Status(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPS6Status,
					strlen(radio.nsPS6Status));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPS6Status\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPS7Status
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPS7Status(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPS7Status,
					strlen(radio.nsPS7Status));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPS7Status\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPS8Status
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPS8Status(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPS8Status,
					strlen(radio.nsPS8Status));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPS8Status\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsBITStatus
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsBITStatus(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val = 0;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsBITStatus);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsBITStatus\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPSTemp
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPSTemp(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPSTemp,
					strlen(radio.nsPSTemp));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPSTemp\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSensorPotentiometer
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSensorPotentiometer(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsSensorPotentiometer);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSensorPotentiometer\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSensorFanPWM
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSensorFanPWM(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsSensorFanPWM);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSensorFanPWM\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSensorTemperature
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSensorTemperature(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsSensorTemperature,
					strlen(radio.nsSensorTemperature));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSensorTemperature\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysIdentifier
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysIdentifier(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsSysIdentifier,
					strlen(radio.nsSysIdentifier));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysIdentifier\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysActivePCBAssy
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysActivePCBAssy(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsSysActivePCBAssy);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysActivePCBAssy\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysGNSSLock
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysGNSSLock(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val = 1;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	if (radio.nsSysGNSSLock[0] == 'A')
        		val = 0;
        	else if (radio.nsSysGNSSLock[1] == 'V')
        		val = 1;

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysGNSSLock\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysInputErr
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysInputErr(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsSysInputErr);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysInputErr\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysChanStatusWord
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysChanStatusWord(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, &radio.nsSysChanStatusWord, strlen(radio.nsSysChanStatusWord));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysChanStatusWord\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysPriPSStatus
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysPriPSStatus(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, &radio.nsSysPriPSStatus, strlen(radio.nsSysPriPSStatus));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysPriPSStatus\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysSecPSStatus
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysSecPSStatus(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, &radio.nsSysSecPSStatus, strlen(radio.nsSysSecPSStatus));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysSecPSStatus\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysActivePCBStatus
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysActivePCBStatus(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:

            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsSysActivePCBStatus,
					strlen(radio.nsSysActivePCBStatus));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysActivePCBStatus\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysChksumStatus
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysChksumStatus(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsSysChksumStatus);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysChksumStatus\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysChanFaultBin
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysChanFaultBin(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsSysChanFaultBin);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysChanFaultBin\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysPriPCBAmpStatus
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysPriPCBAmpStatus(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsSysPriPCBAmpStatus,
					strlen(radio.nsSysPriPCBAmpStatus));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysPriPCBAmpStatus\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysBkupPCBAmpStatus
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysBkupPCBAmpStatus(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsSysBkupPCBAmpStatus,
					strlen(radio.nsSysBkupPCBAmpStatus));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysBkupPCBAmpStatus\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysGPSLock
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysGPSLock(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val = 1;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	if (radio.nsSysGPSLock[0] == 'A')
        		val = 0;
        	else if (radio.nsSysGPSLock[0] == 'V')
        		val = 1;

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysGPSLock\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysSatView
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysSatView(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	if (radio.nsSysSatView[0] != 'N')
        		val = atoi(radio.nsSysSatView);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysSatView\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysErrorByte
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysErrorByte(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsSysErrorByte,
					strlen(radio.nsSysErrorByte));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysErrorByte\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysFreqDiff
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysFreqDiff(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsSysFreqDiff,
					strlen(radio.nsSysFreqDiff));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysFreqDiff\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysPPSDiff
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysPPSDiff(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsSysPPSDiff,
					strlen(radio.nsSysPPSDiff));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysPPSDiff\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysFreqCorSlice
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysFreqCorSlice(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsSysFreqCorSlice,
					strlen(radio.nsSysFreqCorSlice));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysFreqCorSlice\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysDACValue
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysDACValue(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsSysDACValue);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysDACValue\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysPS1VDC
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysPS1VDC(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, &radio.nsSysPS1VDC, strlen(radio.nsSysPS1VDC));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysPS1VDC\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsSysPS2VDC
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsSysPS2VDC(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, &radio.nsSysPS2VDC, strlen(radio.nsSysPS2VDC));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsSysPS2VDC\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsEventDiscCounter
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsEventDiscCounter(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsEventDiscCounter);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsEventDiscCounter\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsEventUserEnabled
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsEventUserEnabled(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsEventUserEnabled);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsEventUserEnabled\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsEventSysEnabled
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsEventSysEnabled(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsEventSysEnabled);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsEventSysEnabled\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsEventGPSLock
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsEventGPSLock(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsEventGPSLock);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsEventGPSLock\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsEventRAMIndex
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsEventRAMIndex(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsEventRAMIndex);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsEventRAMIndex\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsEventTimeAlignment
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsEventTimeAlignment(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsEventTimeAlignment);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsEventTimeAlignment\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsEventEstAccuracy
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsEventEstAccuracy(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsEventEstAccuracy);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED,	&val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsEventEstAccuracy\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsEventEdgeDetDir
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsEventEdgeDetDir(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsEventEdgeDetDir);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsEventEdgeDetDir\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsMeasureFreq
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsMeasureFreq(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsMeasureFreq,
					strlen(radio.nsMeasureFreq));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsMeasureFreq\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsMeasureDAC
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsMeasureDAC(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsMeasureDAC,
					strlen(radio.nsMeasureDAC));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsMeasureDAC\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}



/************************************************
 * Function       : handle_nsMeasureDAC
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsMeasureAnt(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsMeasureAnt,
					strlen(radio.nsMeasureAnt));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsMeasureAnt\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}




/************************************************
 * Function       : handle_nsMeasureRMSOutput
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsMeasureRMSOutput(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsMeasureRMSOutput,
					strlen(radio.nsMeasureRMSOutput));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsMeasureRMSOutput\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}




/************************************************
 * Function       : handle_nsMeasureTemp
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
// uint8_t nsMeasureDAC[10];
// uint8_t nsMeasureTemp[7];

int handle_nsMeasureTemp(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsMeasureTemp,
					strlen(radio.nsMeasureTemp));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsMeasureTemp\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}


/************************************************
 * Function       : handle_nsMeasureHeaterTemp
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/


int handle_nsMeasureHeaterTemp(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsMeasureHeaterTemp,
					strlen(radio.nsMeasureHeaterTemp));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsMeasureHeaterTemp\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}




/************************************************
 * Function       : handle_nsPPSStability
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPPSStability(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsPPSStability);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPPSStability\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPPSDiscGPS
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPPSDiscGPS(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsPPSDiscGPS);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPPSDiscGPS\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPPSOutputType
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPPSOutputType(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsPPSOutputType);

            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPPSOutputType\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPPSDifference
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPPSDifference(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPPSDifference,
					strlen(radio.nsPPSDifference));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPPSDifference\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPPSCalFactor
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPPSCalFactor(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsPPSCalFactor,
					strlen(radio.nsPPSCalFactor));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPPSCalFactor\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPPSTimeCalFactor
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPPSTimeCalFactor(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsPPSTimeCalFactor);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED,	&val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPPSTimeCalFactor\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsPPSFreqVar
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsPPSFreqVar(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
	unsigned int val;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
        	val = atoi(radio.nsPPSFreqVar);

            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED, &val, sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsPPSFreqVar\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
/************************************************
 * Function       : handle_nsCommand
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsCommand(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */

    switch(reqinfo->mode) {

        case MODE_GET:
        	netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_NOACCESS);
            break;

        case MODE_SET_RESERVE1:
        case MODE_SET_RESERVE2:
        case MODE_SET_ACTION:
            /*
             * Send this command string to the Ns2316 radio.
             */
            if (dbg)
            {
    			syslog(LOG_INFO, "Command sent to ns2316: %s\n", requests->requestvb->val.string);
            }

            writeRadio((char *) requests->requestvb->val.string);
	    writeRadio("\r\n");
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_NOERROR);
            break;

        case MODE_SET_COMMIT:
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
            break;

        case MODE_SET_UNDO:
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_UNDOFAILED);
             break;

        case MODE_SET_FREE:
        	// Do nothing
        	netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_NOERROR);
        	break;

        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsCommand\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

/************************************************
 * Function       : handle_nsResult
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsResult(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */

    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&commandResultStr,
					strlen(commandResultStr));

            // Can only read the result once.

            commandResultStr[0] = 0;
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsResult\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}


/************************************************
 * Function       : handle_nsDiscPrioritySource
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsDiscPrioritySource(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
       int val = 0;
    
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsDiscPrioritySource);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsDiscPrioritySource\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}





/************************************************
 * Function       : handle_nsDiscCurrentSource
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsDiscCurrentSource(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsDiscCurrentSource);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsDiscCurrentSource\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}





/************************************************
 * Function       : handle_nsDiscGNSSLock
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsDiscGNSSLock(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    unsigned int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsDiscGNSSLock);
            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsDiscGNSSLock\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}







/************************************************
 * Function       : handle_nsDiscRFPresent
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsDiscRFPresent(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsDiscRFPresent);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsDiscRFPresent\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}








/************************************************
 * Function       : handle_nsDiscOpticalPresent
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsDiscOpticalPresent(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsDiscOpticalPresent);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsDiscOpticalPresent\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}








/************************************************
 * Function       : handle_nsDiscLoopLock
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsDiscLoopLock(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsDiscLoopLock);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsDiscLoopLock\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}








/************************************************
 * Function       : handle_nsWarmupRemaining
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsWarmupRemaining(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    unsigned int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsWarmupRemaining);
            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsWarmupRemaining\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}









/************************************************
 * Function       : handle_nsWarmupComplete
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsWarmupComplete(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsWarmupComplete);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsWarmupComplete\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}










/************************************************
 * Function       : handle_nsHoldoverElapsed
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsHoldoverElapsed(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    unsigned int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsHoldoverElapsed);
            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsHoldoverElapsed\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}




/************************************************
 * Function       : handle_nsHoldoverValid
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsHoldoverValid(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsHoldoverValid);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsHoldoverValid\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}







/************************************************
 * Function       : handle_nsFrequencyValid
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsFrequencyValid(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsFrequencyValid);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsFrequencyValid\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}








/************************************************
 * Function       : handle_nsHoldoverTemp
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsHoldoverTemp(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsHoldoverTemp,
					strlen(radio.nsHoldoverTemp));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsHoldoverTemp\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}







/************************************************
 * Function       : handle_nsRbStatus
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsRbStatus(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsRbStatus);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsRbStatus\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}









/************************************************
 * Function       : handle_nsRbAlarm
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsRbAlarm(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsRbAlarm);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsRbAlarm\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}








/************************************************
 * Function       : handle_nsRbMode
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsRbMode(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsRbMode,
					strlen(radio.nsRbMode));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsRbMode\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}










/************************************************
 * Function       : handle_nsRbDiscStatus
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsRbDiscStatus(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsRbDiscStatus,
					strlen(radio.nsRbDiscStatus));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsRbDiscStatus\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}













/************************************************
 * Function       : handle_nsRbHoldoverSource
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsRbHoldoverSource(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsRbHoldoverSource);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsRbHoldoverSource\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}







/************************************************
 * Function       : handle_nsRb2Lock
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsRb2Lock(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsRb2Lock);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsRb2Lock\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}






/************************************************
 * Function       : handle_nsRb2Status
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsRb2Status(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
            		&radio.nsRb2Status,
					strlen(radio.nsRb2Status));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsRb2Status\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}





/************************************************
 * Function       : handle_nsRb2Steer
 * Input          : netsnmp_mib_handler ptr to handler
 * 					netsnmp_handler_registration ptr to reg
 * 					netsnmp_agent_request_info ptr to agent info
 * 					netsnmp_request_info ptr to current request
 * Output         : SNMP_ERR_xxxxxxx
 * Description    : Handler for OID.
 ************************************************/
int handle_nsRb2Steer(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    int val = 0;
    switch(reqinfo->mode) {

        case MODE_GET:
            val = atoi(radio.nsRb2Steer);
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            		&val,
					sizeof(val));
            break;


        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nsRb2Lock\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

