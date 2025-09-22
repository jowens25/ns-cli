
#include "cores.h"

#include "clkClock.h"
#include "ptpOc.h"
#include "ntpServer.h"
#include "ppsSlave.h"
#include "todSlave.h"
#include "axi.h"
#include <stdint.h>

//   read the core configuration

int getCores(void)
{

    int err = 0;

    // Ucm_CoreConfig temp_config;
    temp_data = 0;
    int64_t type = 0;

    for (int i = 0; i < 256; i++)
    {
        if (0 == readRegister((0x00000000 + ((i * Ucm_Config_BlockSize) + Ucm_Config_TypeInstanceReg)), &temp_data))
        {
            // printf("temp data: %d \n", temp_data);
            if ((i == 0) && ((((temp_data >> 16) & 0x0000FFFF) != Ucm_CoreConfig_ConfSlaveCoreType) || (((temp_data >> 0) & 0x0000FFFF) != 1)))
            {

                printf("ERROR: not a conf block at the address expected: %d\n", i);
                err = -1;
                break;
            }
            else if (temp_data == 0)
            {
                // printf("ERROR 2 \n");
                // err = -2;
                break;
            }
            else
            {

                // printf("ERROR 3 \n");
                // temp_config.core_type = ((temp_data >> 16) & 0x0000FFFF);
                type = ((temp_data >> 16) & 0x0000FFFF);

                switch (type)
                {
                case Ucm_CoreConfig_PhyConfigurationCoreType: // 30
                    type = 30;
                    break;
                case Ucm_CoreConfig_I2cConfigurationCoreType: // 31
                    type = 31;
                    break;
                case Ucm_CoreConfig_IoConfigurationCoreType: // 32
                    type = 32;
                    break;
                case Ucm_CoreConfig_EthernetTestplatformType: // 33
                    type = 33;
                    break;
                case Ucm_CoreConfig_MinSwitchCoreType: // 34
                    type = 34;
                    break;
                case Ucm_CoreConfig_ConfExtCoreType: // 35
                    type = 35;
                    break;
                default:
                    break;
                }

                cores[type].name = coreNames[type];

                cores[type].properties = coreProperties[type];

                cores[type].core_type = type;

                // temp_config.core_instance_nr = ((temp_data >> 0) & 0x0000FFFF);
                cores[type].core_instance_nr = ((temp_data >> 0) & 0x0000FFFF);

                printf("core type: %d ... core name: %s\n", cores[type].core_type, cores[type].name);
            }
        }
        else
        {
            // printf("ERROR 4 \n");
            err = -3;

            break;
        }

        if (0 == readRegister((0x00000000 + ((i * Ucm_Config_BlockSize) + Ucm_Config_BaseAddrLReg)), &temp_data))
        {
            // temp_config.address_range_low = temp_data;
            // cores[i].address_range_low = temp_data;

            cores[type].address_range_low = temp_data;
            // printf("low addr %d \n", temp_data);
        }
        else
        {
            // p/rintf("ERROR 5 \n");
            err = -4;
            break;
        }

        if (0 == readRegister((0x00000000 + ((i * Ucm_Config_BlockSize) + Ucm_Config_BaseAddrHReg)), &temp_data))
        {
            // temp_config.address_range_high = temp_data;
            cores[type].address_range_high = temp_data;
        }
        else
        {
            err = -5;
            // printf("ERROR 6 \n");

            break;
        }

        if (0 == readRegister((0x00000000 + ((i * Ucm_Config_BlockSize) + Ucm_Config_IrqMaskReg)), &temp_data))
        {
            // temp_config.interrupt_mask = temp_data;
            cores[type].interrupt_mask = temp_data;
        }
        else
        {

            // printf("ERROR 7 \n");
            // cores[i] = temp_config;
            err = -6;
            break;

            // ucm->core_config.append(temp_config);
        }
    }

    return err;
}

Ucm_CoreConfig cores[64];

char *coreNames[64] = {

    [Ucm_CoreConfig_ConfSlaveCoreType] = "confslave",
    [Ucm_CoreConfig_ClkClockCoreType] = "clk",
    //[Ucm_CoreConfig_ClkSignalGeneratorCoreType] = "clksignalgenerator",
    //[Ucm_CoreConfig_ClkSignalTimestamperCoreType] = "clksignaltimestamper",
    //[Ucm_CoreConfig_IrigSlaveCoreType] = "irigslave",
    //[Ucm_CoreConfig_IrigMasterCoreType] = "irigmaster",
    [Ucm_CoreConfig_PpsSlaveCoreType] = "pps",
    //[Ucm_CoreConfig_PpsMasterCoreType] = "ppsmaster",
    [Ucm_CoreConfig_PtpOrdinaryClockCoreType] = "ptp",
    //[Ucm_CoreConfig_PtpTransparentClockCoreType] = "ptptransparentclock",
    //[Ucm_CoreConfig_PtpHybridClockCoreType] = "ptphybridclock",
    //[Ucm_CoreConfig_RedHsrPrpCoreType] = "redhsrprp",
    //[Ucm_CoreConfig_RtcSlaveCoreType] = "rtcslave",
    //[Ucm_CoreConfig_RtcMasterCoreType] = "rtcmaster",
    [Ucm_CoreConfig_TodSlaveCoreType] = "tod",
    //[Ucm_CoreConfig_TodMasterCoreType] = "todmaster",
    //[Ucm_CoreConfig_TapSlaveCoreType] = "tapslave",
    //[Ucm_CoreConfig_DcfSlaveCoreType] = "dcfslave",
    //[Ucm_CoreConfig_DcfMasterCoreType] = "dcfmaster",
    //[Ucm_CoreConfig_RedTsnCoreType] = "redtsn",
    //[Ucm_CoreConfig_TsnIicCoreType] = "tsniic",
    [Ucm_CoreConfig_NtpServerCoreType] = "ntp",
    //[Ucm_CoreConfig_NtpClientCoreType] = "ntpclient",
    //[Ucm_CoreConfig_ClkFrequencyGeneratorCoreType] = "clkfrequencygenerator",
    //[Ucm_CoreConfig_SynceNodeCoreType] = "syncenode",
    //[Ucm_CoreConfig_PpsClkToPpsCoreType] = "ppsclktopps",
    //[Ucm_CoreConfig_PtpServerCoreType] = "ptpserver",
    //[Ucm_CoreConfig_PtpClientCoreType] = "ptpclient",
    [30] = "PhyConfiguration", // these were adjusted so we dont have a size 20000 array
    //[31] = "I2cConfiguration",     // these were adjusted so we dont have a size 20000 array
    //[32] = "IoConfiguration",      // these were adjusted so we dont have a size 20000 array
    //[33] = "EthernetTestplatform", // these were adjusted so we dont have a size 20000 array
    //[34] = "MinSwitch",            // these were adjusted so we dont have a size 20000 array
    //[35] = "ConfExt",              // these were adjusted so we dont have a size 20000 array

};

char **coreProperties[64] = {

    [Ucm_CoreConfig_ConfSlaveCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_ClkClockCoreType] = ClkClockProperties,
    //[Ucm_CoreConfig_ClkSignalGeneratorCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_ClkSignalTimestamperCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_IrigSlaveCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_IrigMasterCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_PpsSlaveCoreType] = PpsSlaveProperties,
    //[Ucm_CoreConfig_PpsMasterCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_PtpOrdinaryClockCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_PtpTransparentClockCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_PtpHybridClockCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_RedHsrPrpCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_RtcSlaveCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_RtcMasterCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_TodSlaveCoreType] = TodSlaveProperties,
    //[Ucm_CoreConfig_TodMasterCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_TapSlaveCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_DcfSlaveCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_DcfMasterCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_RedTsnCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_TsnIicCoreType] = PtpOcProperties,
    [Ucm_CoreConfig_NtpServerCoreType] = NtpServerProperties,
    //[Ucm_CoreConfig_NtpClientCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_ClkFrequencyGeneratorCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_SynceNodeCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_PpsClkToPpsCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_PtpServerCoreType] = PtpOcProperties,
    //[Ucm_CoreConfig_PtpClientCoreType] = PtpOcProperties,
    [30] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array
    //[31] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array
    //[32] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array
    //[33] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array
    //[34] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array
    //[35] = PtpOcProperties, // these were adjusted so we dont have a size 20000 array

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

        if (0 == strcmp(cores[core_id].properties[i], "NULL"))
        {
            return -1; // end of properties
        }

        if (cores[core_id].properties[i] != NULL)
        {
            if (strcmp(cores[core_id].properties[i], name) == 0)
            {
                return i;
            }
        }
    }
    return -1;
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