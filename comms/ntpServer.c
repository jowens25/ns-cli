
#include "axi.h"
#include "coreConfig.h"
#include "ntpServer.h"

int64_t ntplow;
int64_t temp_data = 0x00000000;

int readStatus(char *status)
{

    // enabled
    if (0 == readRegister(cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low + Ucm_NtpServer_ControlReg, &temp_data))
    {
        if ((temp_data & 0x00000001) == 0)
        {
            status = "disabled";
            return 0;
            ////ui->NtpServerEnableCheckBox->setChecked(false);
        }
        else
        {
            status = "enabled";
            return 1;
            // ui->NtpServerEnableCheckBox->setChecked(true);
        }
    }
    else
    {
        status = "this is my status disabled";
        return 0;
        // ui->NtpServerEnableCheckBox->setChecked(false);
    }
}

int writeStatus(char *status)
{
    readConfig();
    printf("ntp status set to: %s\n", status);

    if (status == "enable\0")
    {
        temp_data = 0x00000001;
    }
    if (status == "disable\0")
    {
        temp_data = 0x00000000;
    }
    else
    {
        // log.Fatal("Please enter a valid status (enabled or disabled)")
    }
    // temp_data = 0x00000000;

    if (writeRegister(cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low + Ucm_NtpServer_ControlReg, &temp_data) == 0)
    {
        // showNtpServerSTATUS()
    }
    else
    {
        // log.Fatal(" VERBOSE ERROR WRITING NTP")
    }

    return 0;
}