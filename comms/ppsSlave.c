#include "axi.h"
#include "coreConfig.h"
#include "ppsSlave.h"

int hasPpsSlave(char *in, size_t size)
{
    if (Ucm_CoreConfig_PpsSlaveCoreType != cores[Ucm_CoreConfig_PpsSlaveCoreType].core_type)
    {
        return -1;
    }
    return 0;
}

int readPpsSlaveVersion(char *value, size_t size)
{
    temp_data = 0x00000000;

    temp_addr = cores[Ucm_CoreConfig_PpsSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_PpsSlave_VersionReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");

        return -1;
    }
    snprintf(value, size, "0x%lx", temp_data);

    return 0;
}

int readPpsSlaveInstanceNumber(char *instanceNumber, size_t size)
{
    snprintf(instanceNumber, size, "%ld", cores[Ucm_CoreConfig_PpsSlaveCoreType].core_instance_nr);
    return 0;
}

int readPpsSlaveEnableStatus(char *status, size_t size)
{
    temp_data = 0x00000000;

    temp_addr = cores[Ucm_CoreConfig_PpsSlaveCoreType].address_range_low;

    // enabled
    if (0 != readRegister(temp_addr + Ucm_PpsSlave_ControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x00000001) == 0)
    {
        snprintf(status, size, "%s", "disabled");
    }

    else
    {
        snprintf(status, size, "%s", "enabled");
    }

    return 0;
}

int readPpsSlaveInvertedStatus(char *status, size_t size)
{
    temp_data = 0x00000000;

    temp_addr = cores[Ucm_CoreConfig_PpsSlaveCoreType].address_range_low;

    // enabled
    if (0 != readRegister(temp_addr + Ucm_PpsSlave_PolarityReg, &temp_data))
    {
        snprintf(status, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x00000001) == 0)
    {
        snprintf(status, size, "%s", "enabled");
    }

    else
    {
        snprintf(status, size, "%s", "disabled");
    }

    return 0;
}

int readPpsSlaveInputOkStatus(char *status, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_PpsSlaveCoreType].address_range_low;

    // enabled
    if (0 != readRegister(temp_addr + Ucm_PpsSlave_StatusReg, &temp_data))
    {
        snprintf(status, size, "%s", "disabled"); // not ok

        return -1;
    }

    if (temp_data == 0)
    {
        snprintf(status, size, "%s", "enabled"); // ok
    }

    else
    {
        snprintf(status, size, "%s", "disabled");
    }
    // clear
    writeRegister(temp_addr + Ucm_PpsSlave_StatusReg, temp_data);

    return 0;
}

int readPpsSlavePulseWidthValue(char *value, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_PpsSlaveCoreType].address_range_low;
    snprintf(value, size, "%s", "NA"); // not ok

    // enabled
    if (0 != readRegister(temp_addr + Ucm_PpsSlave_PulseWidthReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA"); // not ok

        return -1;
    }

    snprintf(value, size, "%ld", temp_data);

    return 0;
}

int readPpsSlaveCableDelayValue(char *value, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_PpsSlaveCoreType].address_range_low;
    int64_t temp_delay;
    snprintf(value, size, "%s", "NA"); // not ok

    // enabled
    if (0 != readRegister(temp_addr + Ucm_PpsSlave_CableDelayReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA"); // not ok

        return -1;
    }
    temp_delay = (int)(temp_data & 0x3FFFFFFF);
    if ((temp_data & 0x80000000) != 0)
    {
        temp_delay = -1 * temp_delay;
    }
    snprintf(value, size, "%ld", temp_delay);

    return 0;
}