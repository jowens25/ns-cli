#ifndef UCM_CORECONFIG_H
#define UCM_CORECONFIG_H

#define Ucm_CoreConfig_ConfSlaveCoreType 1
#define Ucm_CoreConfig_ClkClockCoreType 2
#define Ucm_CoreConfig_ClkSignalGeneratorCoreType 3
#define Ucm_CoreConfig_ClkSignalTimestamperCoreType 4
#define Ucm_CoreConfig_IrigSlaveCoreType 5
#define Ucm_CoreConfig_IrigMasterCoreType 6
#define Ucm_CoreConfig_PpsSlaveCoreType 7
#define Ucm_CoreConfig_PpsMasterCoreType 8
#define Ucm_CoreConfig_PtpOrdinaryClockCoreType 9
#define Ucm_CoreConfig_PtpTransparentClockCoreType 10
#define Ucm_CoreConfig_PtpHybridClockCoreType 11
#define Ucm_CoreConfig_RedHsrPrpCoreType 12
#define Ucm_CoreConfig_RtcSlaveCoreType 13
#define Ucm_CoreConfig_RtcMasterCoreType 14
#define Ucm_CoreConfig_TodSlaveCoreType 15
#define Ucm_CoreConfig_TodMasterCoreType 16
#define Ucm_CoreConfig_TapSlaveCoreType 17
#define Ucm_CoreConfig_DcfSlaveCoreType 18
#define Ucm_CoreConfig_DcfMasterCoreType 19
#define Ucm_CoreConfig_RedTsnCoreType 20
#define Ucm_CoreConfig_TsnIicCoreType 21
#define Ucm_CoreConfig_NtpServerCoreType 22
#define Ucm_CoreConfig_NtpClientCoreType 23
#define Ucm_CoreConfig_ClkFrequencyGeneratorCoreType 25
#define Ucm_CoreConfig_SynceNodeCoreType 26
#define Ucm_CoreConfig_PpsClkToPpsCoreType 27
#define Ucm_CoreConfig_PtpServerCoreType 28
#define Ucm_CoreConfig_PtpClientCoreType 29
#define Ucm_CoreConfig_PhyConfigurationCoreType 10000 // 30
#define Ucm_CoreConfig_I2cConfigurationCoreType 10001 // 31
#define Ucm_CoreConfig_IoConfigurationCoreType 10002  // 32
#define Ucm_CoreConfig_EthernetTestplatformType 10003 // 33
#define Ucm_CoreConfig_MinSwitchCoreType 10004        // 34
#define Ucm_CoreConfig_ConfExtCoreType 20000          // 35

typedef struct
{
    long core_type;
    long core_instance_nr;
    long address_range_low;
    long address_range_high;
    long interrupt_mask;
} Ucm_CoreConfig;

extern Ucm_CoreConfig cores[64];

#endif // Ucm_CoreConfig_H
