#include "axi.h"
#include "coreConfig.h"
#include "clkClock.h"

int readClkClockVersion(char *value, size_t size)
{
    temp_data = 0x00000000;

    temp_addr = cores[Ucm_CoreConfig_ClkClockCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_ClkClock_ControlReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");

        return -1;
    }
    snprintf(value, size, "0x%lx", temp_data);

    return 0;
}