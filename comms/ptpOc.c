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

int readPtpOcDefaultDsDisableOffsetCorrectionStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        snprintf(status, size, "%s", "NA");
        return -1;
    }
    switch ((temp_data >> 22) & 0x00000001)
    {
    case 0:
        // ui->PtpOcDefaultDsDisableOffsetCorCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");
        break;
    case 1:
        // ui->PtpOcDefaultDsDisableOffsetCorCheckBox->setChecked(true);
        snprintf(status, size, "%s", "enabled"); // offset correction disabled... I think

        break;
    default:
        // ui->PtpOcDefaultDsDisableOffsetCorCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");

        break;
    }
    return 0;
}

// default dataset use listed unicast slaves only (y/n)   f me, this is a long function name
int readPtpOcDefaultDsListedUnicastSlavesOnlyStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        snprintf(status, size, "%s", "NA");
        return -1;
    }
    switch ((temp_data >> 23) & 0x00000001)
    {
    case 0:
        // ui->PtpOcDefaultDsListedUnicastSlavesOnlyCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");
        break;
    case 1:
        // ui->PtpOcDefaultDsListedUnicastSlavesOnlyCheckBox->setChecked(true);
        snprintf(status, size, "%s", "enabled"); // offset correction disabled... I think

        break;
    default:
        // ui->PtpOcDefaultDsListedUnicastSlavesOnlyCheckBox->setChecked(false);
        snprintf(status, size, "%s", "disabled");

        break;
    }
    return 0;
}

int readPtpOcDelayMechanismValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");
        return -1;
    }
    switch ((temp_data >> 24) & 0x00000001)
    {
    case 0:
        snprintf(value, size, "%s", "P2P");
        break;
    case 1:
        if ((temp_data & 0x02000000) == 0)
        {
            snprintf(value, size, "%s", "E2E");
        }
        else
        {
            snprintf(value, size, "%s", "E2E Unicast");
        }
        break;
    default:
        snprintf(value, size, "%s", "NA");
        break;
    }
    return 0;
}

int readPtpOcIpAddress(char *ipAddr, size_t size)
{

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    int64_t temp_ip = 0;
    char layer[size];

    int err = readPtpOcLayer(layer, size);

    if (err != 0)
    {
        snprintf(ipAddr, size, "%s", "mode err");
        return -1;
    }

    if (0 == strncmp(layer, "Layer 2", size))
    {
        snprintf(ipAddr, size, "%s", "NA");
        return 0;
    }

    if (0 == strncmp(layer, "Layer 3v4", size))
    {
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigIpReg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err");
            return -1;
        }
        temp_ip = 0x00000000;
        temp_ip |= (temp_data >> 0) & 0x000000FF;
        temp_ip = temp_ip << 8;
        temp_ip |= (temp_data >> 8) & 0x000000FF;
        temp_ip = temp_ip << 8;
        temp_ip |= (temp_data >> 16) & 0x000000FF;
        temp_ip = temp_ip << 8;
        temp_ip |= (temp_data >> 24) & 0x000000FF;

        unsigned char ip_bytes[4];
        ip_bytes[0] = temp_ip & 0xFF;
        ip_bytes[1] = (temp_ip >> 8) & 0xFF;
        ip_bytes[2] = (temp_ip >> 16) & 0xFF;
        ip_bytes[3] = (temp_ip >> 24) & 0xFF;

        snprintf(ipAddr, size, "%d.%d.%d.%d", ip_bytes[3], ip_bytes[2], ip_bytes[1], ip_bytes[0]);
    }
    else if (0 == strncmp(layer, "Layer 3v6", size))
    {
        unsigned char temp_ip6[16];
        // temp_string.clear();
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigIpReg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err0-3");
            return -1;
        }
        temp_ip6[0] = (temp_data >> 0) & 0x000000FF;
        temp_ip6[1] = (temp_data >> 8) & 0x000000FF;
        temp_ip6[2] = (temp_data >> 16) & 0x000000FF;
        temp_ip6[3] = (temp_data >> 24) & 0x000000FF;

        if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigIpv61Reg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err4-7");
            return -1;
        }
        temp_ip6[4] = (temp_data >> 0) & 0x000000FF;
        temp_ip6[5] = (temp_data >> 8) & 0x000000FF;
        temp_ip6[6] = (temp_data >> 16) & 0x000000FF;
        temp_ip6[7] = (temp_data >> 24) & 0x000000FF;

        if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigIpv62Reg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err8-11");
            return -1;
        }
        temp_ip6[8] = (temp_data >> 0) & 0x000000FF;
        temp_ip6[9] = (temp_data >> 8) & 0x000000FF;
        temp_ip6[10] = (temp_data >> 16) & 0x000000FF;
        temp_ip6[11] = (temp_data >> 24) & 0x000000FF;

        if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigIpv63Reg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err12-15");
            return -1;
        }
        temp_ip6[12] = (temp_data >> 0) & 0x000000FF;
        temp_ip6[13] = (temp_data >> 8) & 0x000000FF;
        temp_ip6[14] = (temp_data >> 16) & 0x000000FF;
        temp_ip6[15] = (temp_data >> 24) & 0x000000FF;

        snprintf(ipAddr, size, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                 temp_ip6[0],
                 temp_ip6[1],
                 temp_ip6[2],
                 temp_ip6[3],
                 temp_ip6[4],
                 temp_ip6[5],
                 temp_ip6[6],
                 temp_ip6[7],
                 temp_ip6[8],
                 temp_ip6[9],
                 temp_ip6[10],
                 temp_ip6[11],
                 temp_ip6[12],
                 temp_ip6[13],
                 temp_ip6[14],
                 temp_ip6[15]);
    }
    else
    {
        snprintf(ipAddr, size, "%s", "NA");
        return -1;
    }

    return 0;
}

int readPtpOcDefaultDsClockId(char *clockId, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        snprintf(clockId, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(clockId, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs1Reg, &temp_data))
            {
                snprintf(clockId, size, "%s", "NA");

                return -3;
            }

            temp_clock_id[0] = ((temp_data >> 0) & 0x000000FF);
            temp_clock_id[1] = ((temp_data >> 8) & 0x000000FF);
            temp_clock_id[2] = ((temp_data >> 16) & 0x000000FF);
            temp_clock_id[3] = ((temp_data >> 24) & 0x000000FF);

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs2Reg, &temp_data))
            {
                snprintf(clockId, size, "%s", "NA");

                return -2;
            }

            temp_clock_id[4] = ((temp_data >> 0) & 0x000000FF);
            temp_clock_id[5] = ((temp_data >> 8) & 0x000000FF);
            temp_clock_id[6] = ((temp_data >> 0) & 0x000000FF);
            temp_clock_id[7] = ((temp_data >> 8) & 0x000000FF);

            snprintf(clockId, size, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
                     temp_clock_id[0],
                     temp_clock_id[1],
                     temp_clock_id[2],
                     temp_clock_id[3],
                     temp_clock_id[4],
                     temp_clock_id[5],
                     temp_clock_id[6],
                     temp_clock_id[7]);

            break;

            // ui->NtpServerMacValue->setText(temp_string);
        }
    }
    return 0;
}

int readPtpOcDefaultDsDomain(char *domain, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        snprintf(domain, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(domain, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs3Reg, &temp_data))
            {
                snprintf(domain, size, "%s", "NA");

                return -3;
            }

            snprintf(domain, size, "0x%02lx", ((temp_data >> 0) & 0x000000FF));
            break;
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}

int readPtpOcDefaultDsPriority1(char *priority1, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        snprintf(priority1, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(priority1, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs3Reg, &temp_data))
            {
                snprintf(priority1, size, "%s", "NA");

                return -3;
            }

            snprintf(priority1, size, "0x%02lx", ((temp_data >> 24) & 0x000000FF));
            break;
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}

// priority 2 from >> 16
int readPtpOcDefaultDsPriority2(char *priority2, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        snprintf(priority2, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(priority2, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs3Reg, &temp_data))
            {
                snprintf(priority2, size, "%s", "NA");

                return -3;
            }

            snprintf(priority2, size, "0x%02lx", ((temp_data >> 16) & 0x000000FF));
            break;
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}

int readPtpOcDefaultDsVariance(char *variance, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        snprintf(variance, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(variance, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs4Reg, &temp_data))
            {
                snprintf(variance, size, "%s", "NA");

                return -3;
            }

            snprintf(variance, size, "0x%04lx", ((temp_data >> 0) & 0x0000FFFF));
            break;
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}

int readPtpOcDefaultDsAccuracy(char *accuracy, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        snprintf(accuracy, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(accuracy, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs4Reg, &temp_data))
            {
                snprintf(accuracy, size, "%s", "NA");

                return -3;
            }

            snprintf(accuracy, size, "%ld", ((temp_data >> 16) & 0x000000FF));

            break;
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}

int readPtpOcDefaultDsClass(char *class, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        snprintf(class, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(class, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs4Reg, &temp_data))
            {
                snprintf(class, size, "%s", "NA");

                return -3;
            }

            snprintf(class, size, "0x%02lx", ((temp_data >> 24) & 0x000000FF));
            break;
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}

int readPtpOcDefaultDsShortId(char *id, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        snprintf(id, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(id, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs5Reg, &temp_data))
            {
                snprintf(id, size, "%s", "NA");

                return -3;
            }

            snprintf(id, size, "0x%04lx", temp_data);
            break;
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}
int readPtpOcDefaultDsInaccuracy(char *inaccuracy, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        snprintf(inaccuracy, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(inaccuracy, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs6Reg, &temp_data))
            {
                snprintf(inaccuracy, size, "%s", "NA");

                return -3;
            }

            snprintf(inaccuracy, size, "%ld", temp_data);
            break;
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}
int readPtpOcDefaultDsNumberOfPorts(char *numPorts, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        snprintf(numPorts, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(numPorts, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs7Reg, &temp_data))
            {
                snprintf(numPorts, size, "%s", "NA");

                return -3;
            }

            snprintf(numPorts, size, "%ld", temp_data);
            break;
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}