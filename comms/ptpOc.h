
#ifndef PTP_OC_H

#define PTP_OC_H

#define Ucm_PtpOc_ControlReg 0x00000000
#define Ucm_PtpOc_StatusReg 0x00000004
#define Ucm_PtpOc_VersionReg 0x0000000C
#define Ucm_PtpOc_NrOfUnicastEntriesReg 0x00000010
#define Ucm_PtpOc_ConfigControlReg 0x00000080
#define Ucm_PtpOc_ConfigProfileReg 0x00000084
#define Ucm_PtpOc_ConfigVlanReg 0x00000088
#define Ucm_PtpOc_ConfigIpReg 0x0000008C
#define Ucm_PtpOc_ConfigIpv61Reg 0x00000090
#define Ucm_PtpOc_ConfigIpv62Reg 0x00000094
#define Ucm_PtpOc_ConfigIpv63Reg 0x00000098
#define Ucm_PtpOc_DefaultDsControlReg 0x00000100
#define Ucm_PtpOc_DefaultDs1Reg 0x00000104
#define Ucm_PtpOc_DefaultDs2Reg 0x00000108
#define Ucm_PtpOc_DefaultDs3Reg 0x0000010C
#define Ucm_PtpOc_DefaultDs4Reg 0x00000110
#define Ucm_PtpOc_DefaultDs5Reg 0x00000114
#define Ucm_PtpOc_DefaultDs6Reg 0x00000118
#define Ucm_PtpOc_DefaultDs7Reg 0x0000011C
#define Ucm_PtpOc_PortDsControlReg 0x00000200
#define Ucm_PtpOc_PortDs1Reg 0x00000204
#define Ucm_PtpOc_PortDs2Reg 0x00000208
#define Ucm_PtpOc_PortDs3Reg 0x0000020C
#define Ucm_PtpOc_PortDs4Reg 0x00000210
#define Ucm_PtpOc_PortDs5Reg 0x00000214
#define Ucm_PtpOc_PortDs6Reg 0x00000218
#define Ucm_PtpOc_PortDs7Reg 0x0000021C
#define Ucm_PtpOc_PortDs8Reg 0x00000220
#define Ucm_PtpOc_CurrentDsControlReg 0x00000300
#define Ucm_PtpOc_CurrentDs1Reg 0x00000304
#define Ucm_PtpOc_CurrentDs2Reg 0x00000308
#define Ucm_PtpOc_CurrentDs3Reg 0x0000030C
#define Ucm_PtpOc_CurrentDs4Reg 0x00000310
#define Ucm_PtpOc_CurrentDs5Reg 0x00000314
#define Ucm_PtpOc_ParentDsControlReg 0x00000400
#define Ucm_PtpOc_ParentDs1Reg 0x00000404
#define Ucm_PtpOc_ParentDs2Reg 0x00000408
#define Ucm_PtpOc_ParentDs3Reg 0x0000040C
#define Ucm_PtpOc_ParentDs4Reg 0x00000410
#define Ucm_PtpOc_ParentDs5Reg 0x00000414
#define Ucm_PtpOc_ParentDs6Reg 0x00000418
#define Ucm_PtpOc_ParentDs7Reg 0x0000041C
#define Ucm_PtpOc_ParentDs8Reg 0x00000420
#define Ucm_PtpOc_ParentDs9Reg 0x00000424
#define Ucm_PtpOc_TimePropertiesDsControlReg 0x00000500
#define Ucm_PtpOc_TimePropertiesDs1Reg 0x00000504
#define Ucm_PtpOc_TimePropertiesDs2Reg 0x00000508
#define Ucm_PtpOc_TimePropertiesDs3Reg 0x0000050C
#define Ucm_PtpOc_TimePropertiesDs4Reg 0x00000510
#define Ucm_PtpOc_TimePropertiesDs5Reg 0x00000514
#define Ucm_PtpOc_TimePropertiesDs6Reg 0x00000518
#define Ucm_PtpOc_TimePropertiesDs7Reg 0x0000051C
#define Ucm_PtpOc_TimePropertiesDs8Reg 0x00000520
#define Ucm_PtpOc_TimePropertiesDs9Reg 0x00000524
#define Ucm_PtpOc_UnicastDsControlReg 0x00000600
#define Ucm_PtpOc_UnicastDs1Reg 0x00000604
#define Ucm_PtpOc_UnicastDs2Reg 0x00000608
#define Ucm_PtpOc_UnicastDs3Reg 0x0000060C
#define Ucm_PtpOc_UnicastDs4Reg 0x00000610
#define Ucm_PtpOc_UnicastDs5Reg 0x00000614
#define Ucm_PtpOc_UnicastDs6Reg 0x00000618
#define Ucm_PtpOc_UnicastDs7Reg 0x0000061C
#define Ucm_PtpOc_UnicastDs8Reg 0x00000620
#define Ucm_PtpOc_UnicastDs9Reg 0x00000624
#define Ucm_PtpOc_UnicastDs10Reg 0x00000628
#define Ucm_PtpOc_UnicastDs11Reg 0x0000062C
#define Ucm_PtpOc_UnicastDs12Reg 0x00000630

// ptp oc
int hasPtpOc(char *in, size_t size);
int readPtpOcVersion(char *value, size_t size);
int readPtpOcInstanceNumber(char *instanceNumber, size_t size);
int readPtpOcVlanAddress(char *vlanAddr, size_t size);
int readPtpOcVlanStatus(char *vlanStatus, size_t size);
int readPtpOcProfile(char *profile, size_t size);
int readPtpOcLayer(char *layer, size_t size);
int readPtpOcDelayMechanismValue(char *value, size_t size);
int readPtpOcIpAddress(char *ipAddr, size_t size);
int readPtpOcStatus(char *status, size_t size);
// default
int readPtpOcDefaultDsClockId(char *clockId, size_t size);
int readPtpOcDefaultDsDomain(char *domain, size_t size);
int readPtpOcDefaultDsPriority1(char *priority1, size_t size);
int readPtpOcDefaultDsPriority2(char *priority2, size_t size);
int readPtpOcDefaultDsAccuracy(char *accuracy, size_t size);
int readPtpOcDefaultDsClass(char *class, size_t size);
int readPtpOcDefaultDsVariance(char *variance, size_t size);
int readPtpOcDefaultDsShortId(char *id, size_t size);
int readPtpOcDefaultDsInaccuracy(char *inaccuracy, size_t size);
int readPtpOcDefaultDsNumberOfPorts(char *numPorts, size_t size);
int readPtpOcDefaultDsTwoStepStatus(char *status, size_t size);
int readPtpOcDefaultDsSignalingStatus(char *status, size_t size);
int readPtpOcDefaultDsMasterOnlyStatus(char *status, size_t size);
int readPtpOcDefaultDsSlaveOnlyStatus(char *status, size_t size);
int readPtpOcDefaultDsListedUnicastSlavesOnlyStatus(char *status, size_t size);
int readPtpOcDefaultDsDisableOffsetCorrectionStatus(char *status, size_t size);
// port
int readPtpOcPortDsPeerDelayValue(char *delay, size_t size);
int readPtpOcPortDsState(char *state, size_t size);
int readPtpOcPortDsAsymmetryValue(char *asymmetry, size_t size);
int readPtpOcPortDsMaxPeerDelayValue(char *delay, size_t size);
int readPtpOcPortDsPDelayReqLogMsgIntervalValue(char *interval, size_t size);
int readPtpOcPortDsDelayReqLogMsgIntervalValue(char *interval, size_t size);
int readPtpOcPortDsDelayReceiptTimeoutValue(char *timeout, size_t size);
int readPtpOcPortDsAnnounceLogMsgIntervalValue(char *interval, size_t size);
int readPtpOcPortDsAnnounceReceiptTimeoutValue(char *timeout, size_t size);
int readPtpOcPortDsSyncLogMsgIntervalValue(char *interval, size_t size);
int readPtpOcPortDsSyncReceiptTimeoutValue(char *timeout, size_t size);
// current - RO
int readPtpOcCurrentDsStepsRemovedValue(char *steps, size_t size);
int readPtpOcCurrentDsOffsetValue(char *offset, size_t size);
int readPtpOcCurrentDsDelayValue(char *delay, size_t size);
// parent - RO
int readPtpOcParentDsParentClockIdValue(char *clockId, size_t size);
int readPtpOcParentDsGmClockIdValue(char *clockId, size_t size);
int readPtpOcParentDsGmPriority1Value(char *priority, size_t size);
int readPtpOcParentDsGmPriority2Value(char *priority, size_t size);
int readPtpOcParentDsGmVarianceValue(char *variance, size_t size);
int readPtpOcParentDsGmAccuracyValue(char *accuracy, size_t size);
int readPtpOcParentDsGmClassValue(char *class, size_t size);
int readPtpOcParentDsGmShortIdValue(char *id, size_t size);
int readPtpOcParentDsGmInaccuracyValue(char *inaccuracy, size_t size);
int readPtpOcParentDsNwInaccuracyValue(char *inaccuracy, size_t size);
// time properties
int readPtpOcTimePropertiesDsTimeSourceValue(char *source, size_t size);
int readPtpOcTimePropertiesDsPtpTimescaleStatus(char *status, size_t size);
int readPtpOcTimePropertiesDsFreqTraceableStatus(char *status, size_t size);
int readPtpOcTimePropertiesDsTimeTraceableStatus(char *status, size_t size);
int readPtpOcTimePropertiesDsLeap61Status(char *status, size_t size);
int readPtpOcTimePropertiesDsLeap59Status(char *status, size_t size);
int readPtpOcTimePropertiesDsUtcOffsetValStatus(char *status, size_t size);
int readPtpOcTimePropertiesDsUtcOffsetValue(char *offset, size_t size);
int readPtpOcTimePropertiesDsCurrentOffsetValue(char *offset, size_t size);
int readPtpOcTimePropertiesDsJumpSecondsValue(char *seconds, size_t size);
int readPtpOcTimePropertiesDsNextJumpValue(char *seconds, size_t size);
int readPtpOcTimePropertiesDsDisplayNameValue(char *seconds, size_t size);

int writePtpOcProfile(char *profile, size_t size);
int writePtpOcDefaultDsTwoStepStatus(char *status, size_t size);
int writePtpOcDefaultDsSignalingStatus(char *status, size_t size);
int writePtpOcDefaultDsSlaveOnlyStatus(char *status, size_t size);
int writePtpOcDefaultDsMasterOnlyStatus(char *status, size_t size);
int writePtpOcDefaultDsDisableOffsetCorrectionStatus(char *status, size_t size);
int writePtpOcDefaultDsListedUnicastSlavesOnlyStatus(char *status, size_t size);
int writePtpOcLayer(char *layer, size_t size);
int writePtpOcDelayMechanismValue(char *layer, size_t size);

int writePtpOcVlanAddress(char *address, size_t size);
int writePtpOcVlanStatus(char *status, size_t size);
int writePtpOcIpAddress(char *ipAddress, size_t size);

int ptp_ipv4_addr_to_register_value(char *ipAddress, size_t size);
int ptp_ipv6_addr_to_register_value(char *ipAddress, size_t size);

int writePtpOcDefaultDsClockIdValue(char *clockid, size_t size);
int writePtpOcDefaultDsDomainValue(char *domain, size_t size);
int writePtpOcDefaultDsPriority1Value(char *priority1, size_t size);
int writePtpOcDefaultDsPriority2Value(char *priority2, size_t size);
int writePtpOcDefaultDsClassValue(char *class, size_t size);
int writePtpOcDefaultDsAccuracyValue(char *accuracy, size_t size);
int writePtpOcDefaultDsVarianceValue(char *variance, size_t size);
int writePtpOcDefaultDsShortIdValue(char *shortid, size_t size);
int writePtpOcDefaultDsInaccuracyValue(char *inaccuracy, size_t size);

int writePtpOcPortDsDelayReceiptTimeoutValue(char *timeout, size_t size);      //
int writePtpOcPortDsDelayReqLogMsgIntervalValue(char *interval, size_t size);  //
int writePtpOcPortDsPDelayReqLogMsgIntervalValue(char *interval, size_t size); //
int writePtpOcPortDsAnnounceReceiptTimeoutValue(char *timeout, size_t size);   //
int writePtpOcPortDsAnnounceLogMsgIntervalValue(char *interval, size_t size);  //
int writePtpOcPortDsSyncReceiptTimeoutValue(char *timeout, size_t size);       //
int writePtpOcPortDsSyncLogMsgIntervalValue(char *interval, size_t size);
int writePtpOcPortDsAsymmetryValue(char *asymmetry, size_t size);
int writePtpOcPortDsMaxPeerDelayValue(char *delay, size_t size);

int readPtpOcPortDsSetCustomIntervalsStatus(char *status, size_t size);
int writePtpOcPortDsSetCustomIntervalsStatus(char *status, size_t size);
int writePtpOcTimePropertiesDsTimeSourceValue(char *source, size_t size);
int writePtpOcTimePropertiesDsPtpTimescaleStatus(char *status, size_t size);
int writePtpOcTimePropertiesDsFreqTraceableStatus(char *status, size_t size);
#endif // PTP_OC_H