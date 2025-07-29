
#include "axi.h"
#include "config.h"
#include "coreConfig.h"
//   read the core configuration

int readConfig(void)
{

    int err = 0;

    Ucm_CoreConfig temp_config;
    temp_data = 0;
    long type = 0;

    for (int i = 0; i < 256; i++)
    {
        if (0 == readRegister((0x00000000 + ((i * Ucm_Config_BlockSize) + Ucm_Config_TypeInstanceReg)), &temp_data))
        {
            // printf("temp data: %ld \n", temp_data);
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

                printf("core type: %ld ... core name: %s\n", cores[type].core_type, cores[type].name);
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
            // printf("low addr %ld \n", temp_data);
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
