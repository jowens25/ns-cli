
#ifndef CORES_H
#define CORES_H
#include <stdint.h>
// CORES STUFF
#define Ucm_Config_BlockSize 16
#define Ucm_Config_TypeInstanceReg 0x00000000
#define Ucm_Config_BaseAddrLReg 0x00000004
#define Ucm_Config_BaseAddrHReg 0x00000008
#define Ucm_Config_IrqMaskReg 0x0000000C
int getCores(void);

// CORE CONFIG STUFF

#define NumberPtpOcProperties 64
#define NumberNtpServerProperties 32

#define ConfSlave 1
#define ClkClock 2
#define ClkSignalGenerator 3
#define ClkSignalTimestamper 4
#define IrigSlave 5
#define IrigMaster 6
#define PpsSlave 7
#define PpsMaster 8
#define PtpOrdinaryClock 9
#define PtpTransparentClock 10
#define PtpHybridClock 11
#define RedHsrPrp 12
#define RtcSlave 13
#define RtcMaster 14
#define TodSlave 15
#define TodMaster 16
#define TapSlave 17
#define DcfSlave 18
#define DcfMaster 19
#define RedTsn 20
#define TsnIic 21
#define NtpServer 22
#define NtpClient 23
#define ClkFrequencyGenerator 25
#define SynceNode 26
#define PpsClkToPps 27
#define PtpServer 28
#define PtpClient 29
#define PhyConfiguration 10000 // 30
#define I2cConfiguration 10001 // 31
#define IoConfiguration 10002  // 32
#define EthernetTestplat 10003 // 33
#define MinSwitch 10004        // 34
#define ConfExt 20000          // 35

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
    char *name;
    char **properties;
    int64_t core_type;
    int64_t core_instance_nr;
    int64_t address_range_low;
    int64_t address_range_high;
    int64_t interrupt_mask;
} Ucm_CoreConfig;

extern Ucm_CoreConfig cores[];

extern char **coreProperties[];

extern char *coreNames[];

int getCoreId(char *name);

int getPropertyId(int core, char *name);

int getOperationId(char *name);

#endif
