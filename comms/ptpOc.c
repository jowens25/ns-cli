#include "axi.h"
#include "coreConfig.h"
#include "ptpOc.h"

int hasPtpOc(char *in, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = 0x00000000;
    if (Ucm_CoreConfig_PtpOrdinaryClockCoreType != cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].core_type)
    {

        return -1;
    }

    return 0;
}

int readPtpOcStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;

    // enabled
    if (0 != readRegister(temp_addr + Ucm_PtpOc_ControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");

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

// vlan status
int readPtpOcVlanStatus(char *vlanStatus, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigVlanReg, &temp_data))
    {
        snprintf(vlanStatus, size, "%s", "NA");
        return -1;
    }
    if ((temp_data & 0x00010000) == 0)
    {
        snprintf(vlanStatus, size, "%s", "disabled");
    }
    else
    {
        snprintf(vlanStatus, size, "%s", "enabled");
    }
    temp_data &= 0x0000FFFF;
    return 0;
}

// vlan addr
int readPtpOcVlanAddress(char *vlanAddr, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigVlanReg, &temp_data))
    {
        snprintf(vlanAddr, size, "%s", "NA");
        return -1;
    }
    temp_data &= 0x0000FFFF;
    snprintf(vlanAddr, size, "0x%04lx", temp_data);
    return 0;
}

int readPtpOcProfile(char *profile, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        snprintf(profile, size, "%s", "NA");
        return -1;
    }

    switch (temp_data & 0x00000007)
    {
    case 0:
        snprintf(profile, size, "%s", "Default");
        break;
    case 1:
        snprintf(profile, size, "%s", "Power");
        break;
    case 2:
        snprintf(profile, size, "%s", "Utility");
        break;
    case 3:
        snprintf(profile, size, "%s", "TSN");
        break;
    case 4:
        snprintf(profile, size, "%s", "ITUG8265.1");
        break;
    case 5:
        snprintf(profile, size, "%s", "ITUG8275.1");
        break;
    case 6:
        snprintf(profile, size, "%s", "ITUG8275.2");
        break;
    default:
        snprintf(profile, size, "%s", "NA");
        break;
    }

    return 0;
}

int readPtpOcDefaultDsTwoStepStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        snprintf(status, size, "%s", "NA");
        return -1;
    }
    switch ((temp_data >> 8) & 0x00000001)
    {
    case 0:
        // ui->PtpOcDefaultDsTwoStepCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");
        break;
    case 1:
        // ui->PtpOcDefaultDsTwoStepCheckBox->setChecked(true);
        snprintf(status, size, "%s", "enabled");

        break;
    default:
        // ui->PtpOcDefaultDsTwoStepCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");

        break;
    }

    return 0;
}

int readPtpOcDefaultDsSignalingStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        snprintf(status, size, "%s", "NA");
        return -1;
    }
    switch ((temp_data >> 9) & 0x00000001)
    {
    case 0:
        // ui->PtpOcDefaultDsTwoStepCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");
        break;
    case 1:
        // ui->PtpOcDefaultDsTwoStepCheckBox->setChecked(true);
        snprintf(status, size, "%s", "enabled");

        break;
    default:
        // ui->PtpOcDefaultDsTwoStepCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");

        break;
    }
    return 0;
}

int readPtpOcLayer(char *layer, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        snprintf(layer, size, "%s", "NA");
        return -1;
    }
    switch ((temp_data >> 16) & 0x00000003)
    {
    case 0:
        // ui->PtpOcLayerValue->setCurrentText("Layer 2");
        snprintf(layer, size, "%s", "Layer 2");

        break;
    case 1:
        // ui->PtpOcLayerValue->setCurrentText("Layer 3v4");
        snprintf(layer, size, "%s", "Layer 3v4");

        break;
    case 2:
        // ui->PtpOcLayerValue->setCurrentText("Layer 3v6");
        snprintf(layer, size, "%s", "Layer 3v6");

        break;
    default:
        // ui->PtpOcLayerValue->setCurrentText("NA");
        snprintf(layer, size, "%s", "NA");

        break;
    }
    return 0;
}

int readPtpOcSlaveOnlyStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        snprintf(status, size, "%s", "NA");
        return -1;
    }
    switch ((temp_data >> 20) & 0x00000003)
    {
    case 0:
        //    ui->PtpOcDefaultDsSlaveOnlyCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");

        // ui->PtpOcDefaultDsMasterOnlyCheckBox->setChecked(false);
        break;
    case 1:
        //    ui->PtpOcDefaultDsSlaveOnlyCheckBox->setChecked(true);
        snprintf(status, size, "%s", "enabled");

        // ui->PtpOcDefaultDsMasterOnlyCheckBox->setChecked(false);
        break;
    case 2:
        //    ui->PtpOcDefaultDsSlaveOnlyCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");

        // ui->PtpOcDefaultDsMasterOnlyCheckBox->setChecked(true);
        break;
    default:
        //    ui->PtpOcDefaultDsSlaveOnlyCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");

        // ui->PtpOcDefaultDsMasterOnlyCheckBox->setChecked(false);
        break;
    }
    return 0;
}

int readPtpOcMasterOnlyStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        snprintf(status, size, "%s", "NA");
        return -1;
    }
    switch ((temp_data >> 20) & 0x00000003)
    {
    case 0:
        //    ui->PtpOcDefaultDsSlaveOnlyCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");

        // ui->PtpOcDefaultDsMasterOnlyCheckBox->setChecked(false);
        break;
    case 1:
        //    ui->PtpOcDefaultDsSlaveOnlyCheckBox->setChecked(true);
        snprintf(status, size, "%s", "disabled");

        // ui->PtpOcDefaultDsMasterOnlyCheckBox->setChecked(false);
        break;
    case 2:
        //    ui->PtpOcDefaultDsSlaveOnlyCheckBox->setChecked(false);
        snprintf(status, size, "%s", "enabled");

        // ui->PtpOcDefaultDsMasterOnlyCheckBox->setChecked(true);
        break;
    default:
        //    ui->PtpOcDefaultDsSlaveOnlyCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");

        // ui->PtpOcDefaultDsMasterOnlyCheckBox->setChecked(false);
        break;
    }
    return 0;
}