#include "coreConfig.h"
#include "ntpServer.h"
#include "ptpOc.h"
#include "axi.h"

Ucm_CoreConfig cores[64];

char *coreNames[64] = {

    [Ucm_CoreConfig_ConfSlaveCoreType] = "confslave",
    [Ucm_CoreConfig_ClkClockCoreType] = "clkclock",
    [Ucm_CoreConfig_ClkSignalGeneratorCoreType] = "clksignalgenerator",
    [Ucm_CoreConfig_ClkSignalTimestamperCoreType] = "clksignaltimestamper",
    [Ucm_CoreConfig_IrigSlaveCoreType] = "irigslave",
    [Ucm_CoreConfig_IrigMasterCoreType] = "irigmaster",
    [Ucm_CoreConfig_PpsSlaveCoreType] = "ppsslave",
    [Ucm_CoreConfig_PpsMasterCoreType] = "ppsmaster",
    [Ucm_CoreConfig_PtpOrdinaryClockCoreType] = "ptpordinaryclock",
    [Ucm_CoreConfig_PtpTransparentClockCoreType] = "ptptransparentclock",
    [Ucm_CoreConfig_PtpHybridClockCoreType] = "ptphybridclock",
    [Ucm_CoreConfig_RedHsrPrpCoreType] = "redhsrprp",
    [Ucm_CoreConfig_RtcSlaveCoreType] = "rtcslave",
    [Ucm_CoreConfig_RtcMasterCoreType] = "rtcmaster",
    [Ucm_CoreConfig_TodSlaveCoreType] = "todslave",
    [Ucm_CoreConfig_TodMasterCoreType] = "todmaster",
    [Ucm_CoreConfig_TapSlaveCoreType] = "tapslave",
    [Ucm_CoreConfig_DcfSlaveCoreType] = "dcfslave",
    [Ucm_CoreConfig_DcfMasterCoreType] = "dcfmaster",
    [Ucm_CoreConfig_RedTsnCoreType] = "redtsn",
    [Ucm_CoreConfig_TsnIicCoreType] = "tsniic",
    [Ucm_CoreConfig_NtpServerCoreType] = "ntp-server",
    [Ucm_CoreConfig_NtpClientCoreType] = "ntpclient",
    [Ucm_CoreConfig_ClkFrequencyGeneratorCoreType] = "clkfrequencygenerator",
    [Ucm_CoreConfig_SynceNodeCoreType] = "syncenode",
    [Ucm_CoreConfig_PpsClkToPpsCoreType] = "ppsclktopps",
    [Ucm_CoreConfig_PtpServerCoreType] = "ptpserver",
    [Ucm_CoreConfig_PtpClientCoreType] = "ptpclient",
    [30] = "PhyConfiguration",     // these were adjusted so we dont have a size 20000 array
    [31] = "I2cConfiguration",     // these were adjusted so we dont have a size 20000 array
    [32] = "IoConfiguration",      // these were adjusted so we dont have a size 20000 array
    [33] = "EthernetTestplatform", // these were adjusted so we dont have a size 20000 array
    [34] = "MinSwitch",            // these were adjusted so we dont have a size 20000 array
    [35] = "ConfExt",              // these were adjusted so we dont have a size 20000 array

};

char **coreProperties[64] = {

    [Ucm_CoreConfig_ConfSlaveCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_ClkClockCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_ClkSignalGeneratorCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_ClkSignalTimestamperCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_IrigSlaveCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_IrigMasterCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_PpsSlaveCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_PpsMasterCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_PtpOrdinaryClockCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_PtpTransparentClockCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_PtpHybridClockCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_RedHsrPrpCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_RtcSlaveCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_RtcMasterCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_TodSlaveCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_TodMasterCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_TapSlaveCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_DcfSlaveCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_DcfMasterCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_RedTsnCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_TsnIicCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_NtpServerCoreType] = NtpServerProperties,
    [Ucm_CoreConfig_NtpClientCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_ClkFrequencyGeneratorCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_SynceNodeCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_PpsClkToPpsCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_PtpServerCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_PtpClientCoreType] = PtpOcProperties,
    [30] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array
    [31] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array
    [32] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array
    [33] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array
    [34] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array
    [35] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array

};

int getCoreId(char *name)
{

    for (int i = 0; i < 63; i++)
    {
        if (cores[i].name != NULL)
        {
            if (strcmp(cores[i].name, name) == 0)
            {
                return i;
            }
        }
    }

    return -1; // not found
}

int getPropertyId(int core_id, char *name)
{
    // cores[core_id].properties[1]
    for (int i = 0; i < 63; i++)
    {
        if (cores[core_id].properties[i] != NULL)
        {

            if (strcmp(cores[core_id].properties[i], name) == 0)
            {
                return i;
            }
        }
    }
    return -1; // not found
}

int getOperationId(char *name)
{
    if (strcmp("read", name) == 0)
    {
        return 0;
    }

    if (strcmp("write", name) == 0)
    {
        return 1;
    }

    return -1;
}