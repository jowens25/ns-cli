
#include "axi.h"
#include "coreConfig.h"
#include "ntpServer.h"
#include "config.h"

int64_t ntplow;
int64_t temp_data = 0x00000000;

int readStatus(char *status, size_t size)
{
    /// readConfig();

    // enabled
    if (0 == readRegister(cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low + Ucm_NtpServer_ControlReg, &temp_data))

    {
        if ((temp_data & 0x00000001) == 0)
        {
            strncpy(status, "disabled", size);
            return 0;
            ////ui->NtpServerEnableCheckBox->setChecked(false);
        }
        else
        {
            strncpy(status, "enabled", size);
            return 1;
            // ui->NtpServerEnableCheckBox->setChecked(true);
        }
    }
    else
    {
        strncpy(status, "disabled", size);
        return 0;
        // ui->NtpServerEnableCheckBox->setChecked(false);
    }
}

int writeStatus(char *status, size_t size)
{
    // readConfig();
    //  printf("NTP STATUS SET TO: %s|\n", status);

    if (0 == strncmp(status, "enable", size))
    {
        temp_data = 0x00000001;
    }

    else if (0 == strncmp(status, "disable", size))
    {
        temp_data = 0x00000000;
    }

    else
    {
        printf("PLEASE ENTER A VALID STATUS\n");
        temp_data = 0x00000000;
    }

    if (0 == writeRegister(cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low + Ucm_NtpServer_ControlReg, &temp_data))
    {
        printf("write reg success\n");

        // showNtpServerSTATUS()
    }
    else
    {
        // log.Fatal(" VERBOSE ERROR WRITING NTP")
    }

    return 0;
}