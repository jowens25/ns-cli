
#include "axi.h"
#include "ntpServer.h"

int64_t ntplow = 0x00000000;
int64_t temp_data = 0x00000000;

int readStatus(void)
{

    // enabled
    if (0 == readRegister(ntplow + Ucm_NtpServer_ControlReg, &temp_data))
    {
        if ((temp_data & 0x00000001) == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}