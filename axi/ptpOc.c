#include "axi.h"
#include "coreConfig.h"
#include "ptpOc.h"
#include <stddef.h>
#define MIN(a, b) ((a) < (b) ? (a) : (b))

char *PtpOcProperties[] = {

    [PtpOcVersion] = "version",
    [PtpOcInstanceNumber] = "instance-number",
    [PtpOcVlanAddress] = "vlan-address",
    [PtpOcVlanStatus] = "vlan-status",
    [PtpOcProfile] = "profile",
    [PtpOcLayer] = "layer",
    [PtpOcDelayMechanismValue] = "delay-mechanism-value",
    [PtpOcIpAddress] = "ip-address",
    [PtpOcStatus] = "status",
    [PtpOcDefaultDsClockId] = "default-ds-clock-id",
    [PtpOcDefaultDsDomain] = "default-ds-domain",
    [PtpOcDefaultDsPriority1] = "default-ds-priority1",
    [PtpOcDefaultDsPriority2] = "default-ds-priority2",
    [PtpOcDefaultDsAccuracy] = "default-ds-accuracy",
    [PtpOcDefaultDsClass] = "default-ds-class",
    [PtpOcDefaultDsVariance] = "default-ds-variance",
    [PtpOcDefaultDsShortId] = "default-ds-short-id",
    [PtpOcDefaultDsInaccuracy] = "default-ds-inaccuracy",
    [PtpOcDefaultDsNumberOfPorts] = "default-ds-number-of-ports", // read only,
    [PtpOcDefaultDsTwoStepStatus] = "default-ds-two-step-status",
    [PtpOcDefaultDsSignalingStatus] = "default-ds-signaling-status",
    [PtpOcDefaultDsMasterOnlyStatus] = "default-ds-master-only-status",
    [PtpOcDefaultDsSlaveOnlyStatus] = "default-ds-slave-only-status",
    [PtpOcDefaultDsListedUnicastSlavesOnlyStatus] = "default-ds-listed-unicast-slaves-only-status",
    [PtpOcDefaultDsDisableOffsetCorrectionStatus] = "default-ds-disable-offset-correction-status",
    [PtpOcPortDsPeerDelayValue] = "port-ds-peer-delay-value", // read only
    [PtpOcPortDsState] = "port-ds-state",                     // read only
    [PtpOcPortDsAsymmetryValue] = "port-ds-asymmetry-value",
    [PtpOcPortDsMaxPeerDelayValue] = "port-ds-maxpeerdelay-value",
    [PtpOcPortDsPDelayReqLogMsgIntervalValue] = "port-ds-p-delay-req-log-msg-interval-value",
    [PtpOcPortDsDelayReqLogMsgIntervalValue] = "port-ds-delay-req-log-msg-interval-value",
    [PtpOcPortDsDelayReceiptTimeoutValue] = "port-ds-delay-receipt-timeout-value",
    [PtpOcPortDsAnnounceLogMsgIntervalValue] = "port-ds-announce-log-msg-interval-value",
    [PtpOcPortDsAnnounceReceiptTimeoutValue] = "port-ds-announce-receipt-timeout-value", // read only
    [PtpOcPortDsSyncLogMsgIntervalValue] = "port-ds-synclogmsgintervalvalue",
    [PtpOcPortDsSyncReceiptTimeoutValue] = "port-ds-syncreceipttimeoutvalue",
    [PtpOcCurrentDsStepsRemovedValue] = "current-ds-stepsremovedvalue",    // read only
    [PtpOcCurrentDsOffsetValue] = "current-ds-offsetvalue",                // read only
    [PtpOcCurrentDsDelayValue] = "current-ds-delayvalue",                  // read only
    [PtpOcParentDsParentClockIdValue] = "parent-ds-parent-clock-id-value", // read only
    [PtpOcParentDsGmClockIdValue] = "parent-ds-gm-clock-id-value",         // read only
    [PtpOcParentDsGmPriority1Value] = "parent-ds-gm-priority1-value",      // read only
    [PtpOcParentDsGmPriority2Value] = "parent-ds-gm-priority2-value",      // read only
    [PtpOcParentDsGmVarianceValue] = "parent-ds-gm-variance-value",        // read only
    [PtpOcParentDsGmAccuracyValue] = "parent-ds-gm-accuracy-value",        // read only
    [PtpOcParentDsGmClassValue] = "parent-ds-gm-class-value",              // read only
    [PtpOcParentDsGmShortIdValue] = "parent-ds-gm-shortid-value",          // read only
    [PtpOcParentDsGmInaccuracyValue] = "parent-ds-gm-inaccuracy-value",    // read only
    [PtpOcParentDsNwInaccuracyValue] = "parent-ds-nw-inaccuracy-value",    // read onl;
    [PtpOcTimePropertiesDsTimeSourceValue] = "time-properties-ds-time-source-value",
    [PtpOcTimePropertiesDsPtpTimescaleStatus] = "time-properties-ds-ptptimescale-status",
    [PtpOcTimePropertiesDsFreqTraceableStatus] = "time-properties-ds-freqtraceable-status",
    [PtpOcTimePropertiesDsTimeTraceableStatus] = "time-properties-ds-timetraceable-status",
    [PtpOcTimePropertiesDsLeap61Status] = "time-properties-ds-leap61-status",
    [PtpOcTimePropertiesDsLeap59Status] = "time-properties-ds-leap59-status",
    [PtpOcTimePropertiesDsUtcOffsetValStatus] = "time-properties-ds-utc-offset-val-status",
    [PtpOcTimePropertiesDsUtcOffsetValue] = "time-properties-ds-utc-offset-value",
    [PtpOcTimePropertiesDsCurrentOffsetValue] = "time-properties-ds-current-offset-value",
    [PtpOcTimePropertiesDsJumpSecondsValue] = "time-properties-ds-jump-seconds-value",
    [PtpOcTimePropertiesDsNextJumpValue] = "time-properties-ds-next-jump-value",
    [PtpOcTimePropertiesDsDisplayNameValue] = "time-properties-ds-display-name-value",
    [61] = NULL,
};

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
// Version
int readPtpOcVersion(char *value, size_t size)
{

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_VersionReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");

        return -1;
    }
    snprintf(value, size, "0x%lx", temp_data);

    return 0;
}
// InstanceNumber
int readPtpOcInstanceNumber(char *instanceNumber, size_t size)
{
    snprintf(instanceNumber, size, "%ld", cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].core_instance_nr);
    return 0;
}
// VlanAddress
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
// VlanStatus
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
// Profile
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
// Layer
int readPtpOcLayer(char *layer, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        snprintf(layer, size, "%s", "NA");
        return -1234;
    }
    printf("The value for this switch is: %ld\r\n", (temp_data >> 16) & 0x00000003);
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
        snprintf(layer, size, "%s", "NA-Default");

        break;
    }
    return 0;
}
// DelayMechanismValue
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
// IpAddress
int readPtpOcIpAddress(char *ipAddr, size_t size)
{

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    int64_t temp_ip = 0;
    char layer[size];

    if (0 != readPtpOcLayer(layer, size))
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
            return -2;
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
            return -3;
        }
        temp_ip6[0] = (temp_data >> 0) & 0x000000FF;
        temp_ip6[1] = (temp_data >> 8) & 0x000000FF;
        temp_ip6[2] = (temp_data >> 16) & 0x000000FF;
        temp_ip6[3] = (temp_data >> 24) & 0x000000FF;

        if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigIpv61Reg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err4-7");
            return -4;
        }
        temp_ip6[4] = (temp_data >> 0) & 0x000000FF;
        temp_ip6[5] = (temp_data >> 8) & 0x000000FF;
        temp_ip6[6] = (temp_data >> 16) & 0x000000FF;
        temp_ip6[7] = (temp_data >> 24) & 0x000000FF;

        if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigIpv62Reg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err8-11");
            return -5;
        }
        temp_ip6[8] = (temp_data >> 0) & 0x000000FF;
        temp_ip6[9] = (temp_data >> 8) & 0x000000FF;
        temp_ip6[10] = (temp_data >> 16) & 0x000000FF;
        temp_ip6[11] = (temp_data >> 24) & 0x000000FF;

        if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigIpv63Reg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err12-15");
            return -6;
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
        return -7;
    }

    return 0;
}
// Status
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

//********************************
// default dataset
//********************************

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
            temp_clock_id[6] = ((temp_data >> 16) & 0x000000FF);
            temp_clock_id[7] = ((temp_data >> 24) & 0x000000FF);

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

int readPtpOcDefaultDsMasterOnlyStatus(char *status, size_t size)
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

int readPtpOcDefaultDsSlaveOnlyStatus(char *status, size_t size)
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
// use listed unicast slaves only (y/n)   f me, this is a long function name
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

//********************************
// port dataset
//********************************
int readPtpOcPortDsPeerDelayValue(char *delay, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    char delayMechanism[size];
    int64_t temp_delay = 0;
    int64_t temp_signed_delay = 0;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(delay, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(delay, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            readPtpOcDelayMechanismValue(delayMechanism, size);

            if (strncmp("E2E", delayMechanism, size) == 0 || strncmp("E2E Unicast", delayMechanism, size) == 0)
            {
                snprintf(delay, size, "%s", "NA");
                return -3;
            }

            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs1Reg, &temp_data))
            {
                return -4;
            }
            // peer delay
            temp_delay = temp_data;
            temp_delay = temp_delay << 32;

            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs2Reg, &temp_data))
            {
                return -5;
            }

            temp_delay |= temp_data;
            temp_signed_delay = (long long)temp_delay;
            temp_signed_delay = temp_signed_delay >> 16;
            // ui->PtpOcPortDsPeerDelayValue->setText(QString::number(temp_signed_delay));
            snprintf(delay, size, "%ld", temp_signed_delay);

            break;
            // snprintf(numPorts, size, "%ld", temp_data);
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}

int readPtpOcPortDsState(char *state, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(state, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(state, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs3Reg, &temp_data))
            {
                return -3;
            }

            switch (temp_data)
            {
            case 0x0000001:
                snprintf(state, size, "%s", "INITIALIZING");
                break;
            case 0x00000002:
                snprintf(state, size, "%s", "FAULTY");
                break;
            case 0x00000003:
                snprintf(state, size, "%s", "DISABLED");
                break;
            case 0x00000004:
                snprintf(state, size, "%s", "LISTENING");
                break;
            case 0x00000005:
                snprintf(state, size, "%s", "PREMASTER");
                break;
            case 0x00000006:
                snprintf(state, size, "%s", "MASTER");
                break;
            case 0x00000007:
                snprintf(state, size, "%s", "PASSIVE");
                break;
            case 0x00000008:
                snprintf(state, size, "%s", "UNCALIBRATED");
                break;
            case 0x00000009:
                snprintf(state, size, "%s", "SLAVE");
                break;
            default:
                snprintf(state, size, "%s", "NA");
                break;
            }
            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcPortDsAsymmetryValue(char *asymmetry, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(asymmetry, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(asymmetry, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs7Reg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcPortDsPDelayReqLogMsgIntervalValue->setText(QString::number((signed char)(temp_data & 0x000000FF)));

            snprintf(asymmetry, size, "%d", ((signed int)temp_data));

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcPortDsMaxPeerDelayValue(char *delay, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(delay, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(delay, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs8Reg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcPortDsPDelayReqLogMsgIntervalValue->setText(QString::number((signed char)(temp_data & 0x000000FF)));

            snprintf(delay, size, "%ld", temp_data);

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcPortDsPDelayReqLogMsgIntervalValue(char *interval, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(interval, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(interval, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs4Reg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcPortDsPDelayReqLogMsgIntervalValue->setText(QString::number((signed char)(temp_data & 0x000000FF)));

            snprintf(interval, size, "%d", ((signed char)(temp_data & 0x000000FF)));

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcPortDsDelayReqLogMsgIntervalValue(char *interval, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(interval, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(interval, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs4Reg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcPortDsPDelayReqLogMsgIntervalValue->setText(QString::number((signed char)(temp_data & 0x000000FF)));

            snprintf(interval, size, "%d", ((signed char)((temp_data >> 8) & 0x000000FF)));

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcPortDsDelayReceiptTimeoutValue(char *timeout, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(timeout, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(timeout, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs4Reg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcPortDsPDelayReqLogMsgIntervalValue->setText(QString::number((signed char)(temp_data & 0x000000FF)));

            snprintf(timeout, size, "%ld", ((temp_data >> 16) & 0x000000FF));

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcPortDsAnnounceLogMsgIntervalValue(char *interval, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(interval, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(interval, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs5Reg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcPortDsPDelayReqLogMsgIntervalValue->setText(QString::number((signed char)(temp_data & 0x000000FF)));

            snprintf(interval, size, "%d", ((signed char)(temp_data & 0x000000FF)));

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcPortDsAnnounceReceiptTimeoutValue(char *timeout, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(timeout, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(timeout, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs5Reg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcPortDsPDelayReqLogMsgIntervalValue->setText(QString::number((signed char)(temp_data & 0x000000FF)));

            snprintf(timeout, size, "%ld", (((temp_data >> 8) & 0x000000FF)));

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcPortDsSyncLogMsgIntervalValue(char *interval, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(interval, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(interval, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs6Reg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcPortDsPDelayReqLogMsgIntervalValue->setText(QString::number((signed char)(temp_data & 0x000000FF)));

            snprintf(interval, size, "%d", ((signed char)(temp_data & 0x000000FF)));

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcPortDsSyncReceiptTimeoutValue(char *timeout, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(timeout, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(timeout, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs6Reg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcPortDsPDelayReqLogMsgIntervalValue->setText(QString::number((signed char)(temp_data & 0x000000FF)));

            snprintf(timeout, size, "%ld", (((temp_data >> 8) & 0x000000FF)));

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

//********************************
// current dataset
//********************************

int readPtpOcCurrentDsStepsRemovedValue(char *steps, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(steps, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(steps, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // state
            if (0 != readRegister(temp_addr + Ucm_PtpOc_CurrentDs1Reg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcPortDsPDelayReqLogMsgIntervalValue->setText(QString::number((signed char)(temp_data & 0x000000FF)));

            snprintf(steps, size, "%ld", temp_data & 0xFFFF);

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcCurrentDsOffsetValue(char *offset, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_offset, temp_signed_offset = 0;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(offset, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(offset, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // offset
            if (0 != readRegister(temp_addr + Ucm_PtpOc_CurrentDs2Reg, &temp_data))
            {
                return -3;
            }
            temp_offset = temp_data;
            temp_offset = temp_offset << 32;

            if (0 != readRegister(temp_addr + Ucm_PtpOc_CurrentDs3Reg, &temp_data))
            {
                return -4;
            }

            temp_offset |= temp_data;

            if ((temp_offset & 0x8000000000000000) != 0)
            {
                temp_offset = (0xFFFF000000000000 | (temp_offset >> 16));
                temp_signed_offset = (long long)temp_offset;
            }
            else
            {
                temp_offset = (0x0000FFFFFFFFFFFF & (temp_offset >> 16));
                temp_signed_offset = (long long)temp_offset;
            }

            if (temp_signed_offset == -4294967296) // negativ 0
            {
                temp_signed_offset = 0;
            }

            // limit to one second in display
            if (temp_signed_offset >= 100000)
            {
                temp_signed_offset = 100000;
            }
            else if (temp_signed_offset <= -100000)
            {
                temp_signed_offset = -100000;
            }

            snprintf(offset, size, "%ld", temp_signed_offset);

            break; // success get out...
        }
        // snprintf(numPorts, size, "%ld", temp_data);
        // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
    }

    return 0;
}

int readPtpOcCurrentDsDelayValue(char *delay, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    char delayMechanism[size];
    int64_t temp_delay = 0;
    int64_t temp_signed_delay = 0;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        snprintf(delay, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(delay, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            readPtpOcDelayMechanismValue(delayMechanism, size);

            if (strncmp("P2P", delayMechanism, size) == 0)
            {
                snprintf(delay, size, "%s", "NA");
                return 0;
            }

            else if (0 != readRegister(temp_addr + Ucm_PtpOc_CurrentDs4Reg, &temp_data))
            {
                return -4;
            }
            // peer delay
            temp_delay = temp_data;
            temp_delay = temp_delay << 32;

            if (0 != readRegister(temp_addr + Ucm_PtpOc_CurrentDs5Reg, &temp_data))
            {
                return -5;
            }

            temp_delay |= temp_data;
            temp_signed_delay = (long long)temp_delay;
            temp_signed_delay = temp_signed_delay >> 16;
            // ui->PtpOcPortDsPeerDelayValue->setText(QString::number(temp_signed_delay));
            snprintf(delay, size, "%ld", temp_signed_delay);

            break;
            // snprintf(numPorts, size, "%ld", temp_data);
            // ui->PtpOcDefaultDsDomainValue->setText(QString("0x%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        }
    }
    return 0;
}

//********************************
// parent dataset
//********************************

int readPtpOcParentDsParentClockIdValue(char *clockId, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[16];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
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
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs1Reg, &temp_data))
            {
                snprintf(clockId, size, "%s", "NA");

                return -3;
            }

            temp_clock_id[0] = ((temp_data >> 0) & 0x000000FF);
            temp_clock_id[1] = ((temp_data >> 8) & 0x000000FF);
            temp_clock_id[2] = ((temp_data >> 16) & 0x000000FF);
            temp_clock_id[3] = ((temp_data >> 24) & 0x000000FF);

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs2Reg, &temp_data))
            {
                snprintf(clockId, size, "%s", "NA");

                return -2;
            }

            temp_clock_id[4] = ((temp_data >> 0) & 0x000000FF);
            temp_clock_id[5] = ((temp_data >> 8) & 0x000000FF);
            temp_clock_id[6] = ((temp_data >> 16) & 0x000000FF);
            temp_clock_id[7] = ((temp_data >> 24) & 0x000000FF);

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs3Reg, &temp_data))
            {

                snprintf(clockId, size, "%s", "NA");

                return -3;
            }

            // port
            temp_clock_id[8] = ((temp_data >> 0) & 0x0000FFFF);

            snprintf(clockId, size, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x.%04d",
                     temp_clock_id[0],
                     temp_clock_id[1],
                     temp_clock_id[2],
                     temp_clock_id[3],
                     temp_clock_id[4],
                     temp_clock_id[5],
                     temp_clock_id[6],
                     temp_clock_id[7],
                     temp_clock_id[8]);

            break;

            // ui->NtpServerMacValue->setText(temp_string);
        }
    }
    return 0;
}

int readPtpOcParentDsGmClockIdValue(char *clockId, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    unsigned char temp_clock_id[8];

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
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
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs4Reg, &temp_data))
            {
                snprintf(clockId, size, "%s", "NA");

                return -3;
            }

            temp_clock_id[0] = ((temp_data >> 0) & 0x000000FF);
            temp_clock_id[1] = ((temp_data >> 8) & 0x000000FF);
            temp_clock_id[2] = ((temp_data >> 16) & 0x000000FF);
            temp_clock_id[3] = ((temp_data >> 24) & 0x000000FF);

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs5Reg, &temp_data))
            {
                snprintf(clockId, size, "%s", "NA");

                return -2;
            }

            temp_clock_id[4] = ((temp_data >> 0) & 0x000000FF);
            temp_clock_id[5] = ((temp_data >> 8) & 0x000000FF);
            temp_clock_id[6] = ((temp_data >> 16) & 0x000000FF);
            temp_clock_id[7] = ((temp_data >> 24) & 0x000000FF);

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

int readPtpOcParentDsGmPriority1Value(char *priority, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_priority;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
    {
        snprintf(priority, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(priority, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs3Reg, &temp_data))
            {
                snprintf(priority, size, "%s", "NA");

                return -3;
            }

            temp_priority = ((temp_data >> 24) & 0x000000FF);

            snprintf(priority, size, "0x%02lx", temp_priority);

            break;

            // ui->NtpServerMacValue->setText(temp_string);
        }
    }
    return 0;
}

int readPtpOcParentDsGmPriority2Value(char *priority, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_priority;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
    {
        snprintf(priority, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(priority, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs3Reg, &temp_data))
            {
                snprintf(priority, size, "%s", "NA");

                return -3;
            }

            temp_priority = ((temp_data >> 16) & 0x000000FF);

            snprintf(priority, size, "0x%02lx", temp_priority);

            break;

            // ui->NtpServerMacValue->setText(temp_string);
        }
    }
    return 0;
}

int readPtpOcParentDsGmVarianceValue(char *variance, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_variance;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
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
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs6Reg, &temp_data))
            {
                snprintf(variance, size, "%s", "NA");

                return -3;
            }

            temp_variance = ((temp_data >> 0) & 0x0000FFFF);

            snprintf(variance, size, "0x%02lx", temp_variance);

            break;
        }
    }
    return 0;
}

int readPtpOcParentDsGmAccuracyValue(char *accuracy, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_accuracy;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
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
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs6Reg, &temp_data))
            {
                snprintf(accuracy, size, "%s", "NA");

                return -3;
            }

            temp_accuracy = ((temp_data >> 16) & 0x000000FF);

            snprintf(accuracy, size, "%02ld", temp_accuracy);

            break;
        }
    }
    return 0;
}

int readPtpOcParentDsGmClassValue(char *class, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_class;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
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
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs6Reg, &temp_data))
            {
                snprintf(class, size, "%s", "NA");

                return -3;
            }

            temp_class = ((temp_data >> 24) & 0x000000FF);

            snprintf(class, size, "0x%02lx", temp_class);

            break;
        }
    }
    return 0;
}

int readPtpOcParentDsGmShortIdValue(char *id, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_id;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
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
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs7Reg, &temp_data))
            {
                snprintf(id, size, "%s", "NA");

                return -3;
            }

            temp_id = temp_data;

            snprintf(id, size, "0x%04lx", temp_id);

            break;
        }
    }
    return 0;
}

int readPtpOcParentDsGmInaccuracyValue(char *inaccuracy, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_inaccuracy;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
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
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs8Reg, &temp_data))
            {
                snprintf(inaccuracy, size, "%s", "NA");

                return -3;
            }

            temp_inaccuracy = temp_data;

            snprintf(inaccuracy, size, "%ld", temp_inaccuracy);

            break;
        }
    }
    return 0;
}

int readPtpOcParentDsNwInaccuracyValue(char *inaccuracy, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_inaccuracy;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
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
        if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDsControlReg, &temp_data))
        {
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_ParentDs9Reg, &temp_data))
            {
                snprintf(inaccuracy, size, "%s", "NA");

                return -3;
            }

            temp_inaccuracy = temp_data;

            snprintf(inaccuracy, size, "%ld", temp_inaccuracy);

            break;
        }
    }
    return 0;
}

//********************************
// time properties dataset
//********************************

int readPtpOcTimePropertiesDsTimeSourceValue(char *source, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_source;
    snprintf(source, size, "%s", "NA");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(source, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(source, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(source, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
            {
                snprintf(source, size, "%s", "err3");
                return -3;
            }

            temp_source = (temp_data >> 0) & 0x000000FF;

            snprintf(source, size, "0x%02lx", temp_source);

            break;
        }
    }
    return 0;
}

int readPtpOcTimePropertiesDsPtpTimescaleStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    snprintf(status, size, "%s", "disabled");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(status, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(status, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
            {
                snprintf(status, size, "%s", "err3");
                return -3;
            }

            if ((temp_data & 0x00000100) != 0)
            {
                snprintf(status, size, "%s", "enabled");
            }
            else
            {
                snprintf(status, size, "%s", "disabled");
            }

            break;
        }
    }
    return 0;
}

int readPtpOcTimePropertiesDsFreqTraceableStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    snprintf(status, size, "%s", "disabled");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(status, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(status, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
            {
                snprintf(status, size, "%s", "err3");
                return -3;
            }

            if ((temp_data & 0x00000200) != 0)
            {
                snprintf(status, size, "%s", "enabled");
            }
            else
            {
                snprintf(status, size, "%s", "disabled");
            }

            break;
        }
    }
    return 0;
}

int readPtpOcTimePropertiesDsTimeTraceableStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    snprintf(status, size, "%s", "disabled");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(status, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(status, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
            {
                snprintf(status, size, "%s", "err3");
                return -3;
            }

            if ((temp_data & 0x00000400) != 0)
            {
                snprintf(status, size, "%s", "enabled");
            }
            else
            {
                snprintf(status, size, "%s", "disabled");
            }

            break;
        }
    }
    return 0;
}

int readPtpOcTimePropertiesDsLeap61Status(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    snprintf(status, size, "%s", "disabled");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(status, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(status, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
            {
                snprintf(status, size, "%s", "err3");
                return -3;
            }

            if ((temp_data & 0x00000800) != 0)
            {
                snprintf(status, size, "%s", "enabled");
            }
            else
            {
                snprintf(status, size, "%s", "disabled");
            }

            break;
        }
    }
    return 0;
}

int readPtpOcTimePropertiesDsLeap59Status(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    snprintf(status, size, "%s", "disabled");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(status, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(status, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
            {
                snprintf(status, size, "%s", "err3");
                return -3;
            }

            if ((temp_data & 0x00001000) != 0)
            {
                snprintf(status, size, "%s", "enabled");
            }
            else
            {
                snprintf(status, size, "%s", "disabled");
            }

            break;
        }
    }
    return 0;
}

int readPtpOcTimePropertiesDsUtcOffsetValStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    snprintf(status, size, "%s", "disabled");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(status, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(status, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
            {
                snprintf(status, size, "%s", "err3");
                return -3;
            }

            if ((temp_data & 0x00002000) != 0)
            {
                snprintf(status, size, "%s", "enabled");
            }
            else
            {
                snprintf(status, size, "%s", "disabled");
            }

            break;
        }
    }
    return 0;
}

int readPtpOcTimePropertiesDsUtcOffsetValue(char *offset, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_offset;
    snprintf(offset, size, "%s", "NA");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(offset, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(offset, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(offset, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
            {
                snprintf(offset, size, "%s", "err3");
                return -3;
            }

            temp_offset = (temp_data >> 16) & 0x0000FFFF;

            snprintf(offset, size, "%ld", temp_offset);

            break;
        }
    }
    return 0;
}

int readPtpOcTimePropertiesDsCurrentOffsetValue(char *offset, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_offset;
    snprintf(offset, size, "%s", "NA");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(offset, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(offset, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(offset, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs2Reg, &temp_data))
            {
                snprintf(offset, size, "%s", "err3");
                return -3;
            }

            temp_offset = temp_data;

            snprintf(offset, size, "%ld", temp_offset);

            break;
        }
    }
    return 0;
}

int readPtpOcTimePropertiesDsJumpSecondsValue(char *seconds, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_seconds;
    snprintf(seconds, size, "%s", "NA");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(seconds, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(seconds, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(seconds, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs3Reg, &temp_data))
            {
                snprintf(seconds, size, "%s", "err3");
                return -3;
            }

            temp_seconds = temp_data;

            snprintf(seconds, size, "%ld", temp_seconds);

            break;
        }
    }
    return 0;
}

int readPtpOcTimePropertiesDsNextJumpValue(char *seconds, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_next_jump;
    snprintf(seconds, size, "%s", "NA");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(seconds, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(seconds, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(seconds, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs4Reg, &temp_data))
            {
                snprintf(seconds, size, "%s", "err3");
                return -3;
            }

            temp_next_jump = temp_data;
            temp_next_jump = temp_next_jump << 32;

            if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs5Reg, &temp_data))
            {
                return -4;
            }
            temp_next_jump |= temp_data;
            snprintf(seconds, size, "%ld", temp_next_jump);

            // ui->PtpOcTimePropertiesDsNextJumpValue->setText(QString::number(temp_next_jump));
            // snprintf(seconds, size, "%ld", temp_next_jump);
        }

        break;
    }
    return 0;
}

int readPtpOcTimePropertiesDsDisplayNameValue(char *seconds, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x40000000;
    int64_t temp_length = 0x00000000;
    char temp_string[16];
    snprintf(seconds, size, "%s", "NA");

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        snprintf(seconds, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(seconds, size, "%s", "err1: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
        {
            snprintf(seconds, size, "%s", "err2");
            return -2;
        }

        if ((temp_data & 0x80000000) != 0)
        {

            // display name
            // temp_string.clear();
            if (0 == readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs6Reg, &temp_length))
            {
                for (int j = 0; j < 3; j++)
                {
                    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs7Reg + (j * 4), &temp_data))
                    {
                        return -3;
                    }
                    temp_string[j * 4 + 0] = (temp_data >> 0) & 0x000000FF;
                    temp_string[j * 4 + 1] = (temp_data >> 8) & 0x000000FF;
                    temp_string[j * 4 + 2] = (temp_data >> 16) & 0x000000FF;
                    temp_string[j * 4 + 3] = (temp_data >> 24) & 0x000000FF;
                }
            }
            // truncate your face
            temp_string[MIN(temp_length, 12)] = '\0';
            snprintf(seconds, size, "%s", temp_string);

            // temp_string.truncate(temp_length);
            // ui->PtpOcTimePropertiesDsDisplayNameValue->setText(temp_string);
        }

        break;
    }
    return 0;
}

int writePtpOcProfile(char *profile, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -1;
    }
    temp_data &= ~0x00000007;

    if (0 == strncmp(profile, "Default", size))
    {
        temp_data |= 0x00000000;
    }
    else if (0 == strncmp(profile, "Power", size))
    {
        temp_data |= 0x00000001;
    }
    else if (0 == strncmp(profile, "Utility", size))
    {
        temp_data |= 0x00000002;
    }
    else if (0 == strncmp(profile, "TSN", size))
    {
        temp_data |= 0x00000003;
    }
    else if (0 == strncmp(profile, "ITUG8265.1", size))
    {
        temp_data |= 0x00000004;
    }
    else if (0 == strncmp(profile, "ITUG8275.1", size))
    {
        temp_data |= 0x00000005;
    }
    else if (0 == strncmp(profile, "ITUG8275.2", size))
    {
        temp_data |= 0x00000006;
    }
    else
    {

        return -1;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -2; // write fail
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -3; // write fail
    }

    return 0;
}

int writePtpOcDefaultDsTwoStepStatus(char *status, size_t size)
{

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -1;
    }

    // clear the two step bit
    temp_data &= ~0x00000100;

    if (0 == strcmp(status, "enabled"))
    {
        temp_data |= 0x00000100;
    }
    else if (0 == strcmp(status, "disabled"))
    {
        temp_data |= 0x00000000;
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -3; // write fail
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -4; // write fail
    }

    return 0;
}

int writePtpOcDefaultDsSignalingStatus(char *status, size_t size)
{

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -1;
    }

    // clear the two step bit
    temp_data &= ~0x00000200;

    if (0 == strcmp(status, "enabled"))
    {
        temp_data |= 0x00000200;
    }
    else if (0 == strcmp(status, "disabled"))
    {
        temp_data |= 0x00000000;
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -3; // write fail
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -4; // write fail
    }

    return 0;
}

int writePtpOcDefaultDsSlaveOnlyStatus(char *status, size_t size)
{

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -1;
    }

    printf("temp data in write slave only: 0x%08lx\r\n", temp_data);

    printf("temp data in write slave only: 0x%08lx\r\n", temp_data >> 20);

    // clear both bits
    temp_data &= ~(0x00100000 | 0x00200000);

    if (0 == strcmp(status, "enabled"))
    {
        temp_data |= 0x00100000;
    }
    else if (0 == strcmp(status, "disabled"))
    {
        temp_data |= 0x00000000;
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -3; // write fail
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -4; // write fail
    }

    return 0;
}

int writePtpOcDefaultDsMasterOnlyStatus(char *status, size_t size)
{

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -1;
    }

    temp_data &= ~(0x00100000 | 0x00200000);

    if (0 == strcmp(status, "enabled"))
    {
        temp_data |= 0x00200000;
    }
    else if (0 == strcmp(status, "disabled"))
    {
        temp_data |= 0x00000000;
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -3; // write fail
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -4; // write fail
    }

    return 0;
}

int writePtpOcDefaultDsDisableOffsetCorrectionStatus(char *status, size_t size)
{

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -1;
    }

    temp_data &= ~0x00400000;

    if (0 == strcmp(status, "enabled"))
    {
        temp_data |= 0x00400000;
    }
    else if (0 == strcmp(status, "disabled"))
    {
        temp_data |= 0x00000000;
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -3; // write fail
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -4; // write fail
    }

    return 0;
}

int writePtpOcDefaultDsListedUnicastSlavesOnlyStatus(char *status, size_t size)
{

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -1;
    }

    temp_data &= ~0x00800000;

    if (0 == strcmp(status, "enabled"))
    {
        temp_data |= 0x00800000;
    }
    else if (0 == strcmp(status, "disabled"))
    {
        temp_data |= 0x00000000;
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -3; // write fail
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -4; // write fail
    }

    return 0;
}

int writePtpOcLayer(char *layer, size_t size)
{

    char currentAddress[size];
    if (0 != readPtpOcIpAddress(currentAddress, size))
    {
        return -1;
    }

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -2;
    }

    // clear both
    temp_data &= ~(0x00010000 | 0x00020000);

    if (0 == strcmp(layer, "Layer 2"))
    {

        temp_data |= 0x00000000;
    }

    else if (0 == strcmp(layer, "Layer 3v4"))
    {
        // temp_data &= ~0x00020000;
        temp_data |= 0x00010000;
    }

    else if (0 == strcmp(layer, "Layer 3v6"))
    {
        // temp_data &= ~0x00010000;
        temp_data |= 0x00020000;
    }

    else
    {
        return -3;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -4; // write fail
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -5; // write fail
    }

    if (0 != writePtpOcIpAddress(currentAddress, size))
    {
        return -6;
    }

    return 0;
}

int writePtpOcDelayMechanismValue(char *mechanism, size_t size)
{

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -1;
    }

    // clear both
    temp_data &= ~(0x01000000 | 0x03000000);

    if (0 == strcmp(mechanism, "P2P"))
    {
        temp_data |= 0x00000000;
    }

    else if (0 == strcmp(mechanism, "E2E"))
    {
        temp_data |= 0x01000000;
    }

    else if (0 == strcmp(mechanism, "E2E Unicast"))
    {
        temp_data |= 0x03000000;
    }

    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigProfileReg, &temp_data))
    {
        return -3; // write fail
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -4; // write fail
    }

    return 0;
}

int writePtpOcVlanAddress(char *address, size_t size)
{
    // return -1;
    //  readConfig();
    if (strlen(address) > 6)
    {
        return -1;
    }
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    long temp_vlan = 0;
    address = &address[2];

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigVlanReg, &temp_data))
    {
        return -2; // read current settings fails
    }

    temp_vlan = strtol(address, NULL, 16);

    temp_data &= 0xFFFF0000;

    temp_data |= temp_vlan;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigVlanReg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000002;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcVlanStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_ConfigVlanReg, &temp_data))
    {
        return -1;
    }

    temp_data &= 0x0000FFFF;

    if (0 == strncmp(status, "enabled", size))
    {
        temp_data = 0x00010000 | temp_data;
    }
    else if (0 == strncmp(status, "disabled", size))
    {
        temp_data = 0x00000000 | temp_data; // disable
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigVlanReg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -4;
    }

    return 0;
}

int writePtpOcIpAddress(char *ipAddress, size_t size)
{
    char currentMode[size];

    if (0 != readPtpOcLayer(currentMode, size))
    {
        return -1;
    }

    if (0 == strncmp(currentMode, "Layer 3v4", size))
    {
        ptp_ipv4_addr_to_register_value(ipAddress, size);
    }
    else if (0 == strncmp(currentMode, "Layer 3v6", size))
    {
        ptp_ipv6_addr_to_register_value(ipAddress, size);
    }

    else if (0 == strncmp(currentMode, "Layer 2", size))
    {
        // nothing
    }
    else
    {
        return -2;
    }
    return 0;
}

int ptp_ipv4_addr_to_register_value(char *ipAddress, size_t size)
{
    long temp_ip[4] = {0};

    if (strchr(ipAddress, '.'))
    { // ipv4
        printf("ipv4 case \n");

        char *token;
        for (int i = 0; i < 4; i++)
        {
            token = (i == 0) ? strtok(ipAddress, ".") : strtok(NULL, ".");
            if (token == NULL)
                break;
            temp_ip[i] = strtol(token, NULL, 10);
        }
    }

    else if (strchr(ipAddress, ':'))
    {
        printf("ipv6 going to ipv4?");
        // temp_ip[0] = 0;
        // temp_ip[1] = 0;
        // temp_ip[2] = 0;
        // temp_ip[3] = 0;

        ipAddress = ipAddress + 25;

        char *token;
        char byte[3];
        for (int i = 0; i < 4; i += 2)
        {

            if (i == 0)
            {
                token = strtok(ipAddress, ":"); // grabs first token
                printf("first token: %s\n", token);
                if (0 == strncmp(token, "ffff", 5))
                {
                    token = strtok(NULL, ":");
                    printf("ffff so it is a mapped ipv4\n");
                }
                else
                {
                    printf("not ffff, not mapped so exit\n");
                    temp_ip[0] = 0;
                    temp_ip[1] = 0;
                    temp_ip[2] = 0;
                    temp_ip[3] = 0;
                    break;
                }
            }
            else
            {
                token = strtok(NULL, ":");
            }

            if (token == NULL)
                break;

            printf("token: %s \n", token);
            // Extract first two characters
            strncpy(byte, token, 2);

            snprintf(byte, 3, "%2s", token);

            printf("byte: %s \n", byte);

            temp_ip[i] = strtol(byte, NULL, 16);

            // Extract next two characters
            snprintf(byte, 3, "%2s", token + 2);
            printf("byte: %s \n", byte);

            temp_ip[i + 1] = strtol(byte, NULL, 16);
        }
    }
    else
    {
        printf("big fat else\n");
        return -1;
    }
    temp_data = 0x00000000;
    temp_data |= (temp_ip[3]) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_ip[2]) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_ip[1]) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_ip[0]) & 0x000000FF;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigIpReg, &temp_data))
    {
        printf("failed to write config \n");
        return -1;
    }
    temp_data = 0x00000008; // write
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        printf("failed to write control \n");
        return -1;
    }
    return 0;
}
int ptp_ipv6_addr_to_register_value(char *ipAddress, size_t size)
{
    long temp_ip[16] = {0};

    if (strchr(ipAddress, ':'))
    { // ipv4
        char *token;
        char *err;
        char byte[3];
        for (int i = 0; i < size; i += 2)
        {

            token = (i == 0) ? strtok(ipAddress, ":") : strtok(NULL, ":");
            if (token == NULL)
                break;

            // Extract first two characters
            strncpy(byte, token, 2);
            byte[2] = '\0';
            temp_ip[i] = strtol(byte, &err, 16);
            if (err == token || *err != '\0')
            {
                return -1;
            }

            // Extract next two characters
            strncpy(byte, token + 2, 2);
            byte[2] = '\0';
            temp_ip[i + 1] = strtol(byte, &err, 16);
            if (err == token || *err != '\0')
            {
                return -1;
            }
        }
    }

    else if (strchr(ipAddress, '.'))
    {
        char *token;
        char *err;
        for (int i = 12; i < 16; i++)
        {
            token = (i == 12) ? strtok(ipAddress, ".") : strtok(NULL, ".");
            if (token == NULL)
                break;
            temp_ip[i] = strtol(token, &err, 10);

            if (err == token || *err != '\0')
            {
                return -1;
            }
        }
        temp_ip[10] = 0x000000FF;
        temp_ip[11] = 0x000000FF;
    }
    else
    {
        return -1;
    }

    temp_data = 0x00000000;
    temp_data |= temp_ip[3] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[2] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[1] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[0] & 0x000000FF;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigIpReg, &temp_data))
    {
        return -1;
    }
    temp_data = 0x00000000;
    temp_data |= temp_ip[7] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[6] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[5] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[4] & 0x000000FF;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigIpv61Reg, &temp_data))
    {
        return -2;
    }
    temp_data = 0x00000000;
    temp_data |= temp_ip[11] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[10] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[9] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[8] & 0x000000FF;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigIpv62Reg, &temp_data))
    {
        return -3;
    }
    temp_data = 0x00000000;
    temp_data |= temp_ip[15] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[14] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[13] & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= temp_ip[12] & 0x000000FF;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigIpv63Reg, &temp_data))
    {
        return -4;
    }
    temp_data = 0x00000008; // write
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ConfigControlReg, &temp_data))
    {
        return -5;
    }
    return 0;
}

//********************************
// default dataset
//********************************

int writePtpOcDefaultDsClockIdValue(char *clockid, size_t size)
{ // mac
    // readConfig();
    // AA:BB:CC:DD:EE:FF:GG:HH
    if (strlen(clockid) > 23)
    {
        return -1;
    }
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    // int j = 0;
    long temp_clockid = 0;

    // removes :
    for (int i = 0, j = 0; i < size; i++)
    {
        if (clockid[i] != ':')
        {
            clockid[j] = clockid[i];
            j++;
        }

        if (clockid[i] == '\0')
        {
            break;
        }
    }

    temp_clockid = strtol(clockid, NULL, 16);

    temp_data = 0x00000000;
    temp_data |= (temp_clockid >> 32) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_clockid >> 40) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_clockid >> 48) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_clockid >> 56) & 0x000000FF;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDs1Reg, &temp_data))
    {
        return -1;
    }

    temp_data = 0x00000000;
    temp_data |= (temp_clockid >> 0) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_clockid >> 8) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_clockid >> 16) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_clockid >> 24) & 0x000000FF;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDs2Reg, &temp_data))
    {
        return -1;
    }

    temp_data = 0x00000001; // write
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        return -1;
    }
    // write success
    return 0;
}

int writePtpOcDefaultDsDomainValue(char *domain, size_t size)
{

    if (strlen(domain) != 4)
    {
        return -1;
    }
    domain[0] = domain[2];
    domain[1] = domain[3];
    domain[2] = '\0';

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs3Reg, &temp_data))
    {
        return -2; // read current settings fails
    }
    // remove current domain
    temp_data &= ~0x000000FF;
    // temp_data &= 0x000000FF;

    temp_data |= strtol(domain, NULL, 16);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDs3Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000002;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcDefaultDsPriority1Value(char *priority1, size_t size)
{

    if (strlen(priority1) != 4)
    {
        return -1;
    }

    priority1[0] = priority1[2];
    priority1[1] = priority1[3];
    priority1[2] = '\0';

    long temp_priority1 = (strtol(priority1, NULL, 16)); // takes 0x44 and puts in the top of the ds3 reg -> 0x44000000

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs3Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    temp_data &= ~(0xFF << 24);          // mask the bottom 6 bytes
    temp_data |= (temp_priority1 << 24); // set top 2 bytes

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDs3Reg, &temp_data))
    {
        return -3;
    }

    // write
    temp_data = 0x00000008;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcDefaultDsPriority2Value(char *priority2, size_t size)
{

    if (strlen(priority2) != 4)
    {
        return -1;
    }
    priority2[0] = priority2[2];
    priority2[1] = priority2[3];
    priority2[2] = '\0';

    long temp_priority2 = (strtol(priority2, NULL, 16)); // takes 0x44 and puts in the top of the ds3 reg -> 0x44000000

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs3Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    temp_data &= ~(0xFF << 16);          // mask the bottom 6 bytes
    temp_data |= (temp_priority2 << 16); // set top 2 bytes

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDs3Reg, &temp_data))
    {
        return -3;
    }

    // write
    temp_data = 0x00000010;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcDefaultDsClassValue(char *class, size_t size)
{

    if (strlen(class) != 4)
    {
        return -1;
    }
    class[0] = class[2];
    class[1] = class[3];
    class[2] = '\0';

    long temp_class = (strtol(class, NULL, 16)); // takes 0x44 and puts in the top of the ds3 reg -> 0x44000000

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs4Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    temp_data &= ~(0xFF << 24);      // mask 0x 00 FF
    temp_data |= (temp_class << 24); // set top 2 bytes

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDs4Reg, &temp_data))
    {
        return -3;
    }

    // write
    temp_data = 0x00000004;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcDefaultDsAccuracyValue(char *accuracy, size_t size)
{

    long temp_accuracy = (strtol(accuracy, NULL, 10));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs4Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    temp_data &= ~(0xFF << 16);         // mask bytes
    temp_data |= (temp_accuracy << 16); // set bytes

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDs4Reg, &temp_data))
    {
        return -3;
    }

    // write
    temp_data = 0x00000004;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcDefaultDsVarianceValue(char *variance, size_t size)
{

    if (strlen(variance) != 6)
    {
        return -1;
    }
    variance[0] = variance[2];
    variance[1] = variance[3];
    variance[2] = variance[4];
    variance[3] = variance[5];
    variance[5] = '\0';

    long temp_class = (strtol(variance, NULL, 16)); // takes 0x44 and puts in the top of the ds3 reg -> 0x44000000

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs4Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    temp_data &= ~(0xFFFF << 0);    // mask 0x 00 FF
    temp_data |= (temp_class << 0); // set top 2 bytes

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDs4Reg, &temp_data))
    {
        return -3;
    }

    // write
    temp_data = 0x00000004;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcDefaultDsShortIdValue(char *shortid, size_t size)
{

    if (strlen(shortid) != 6)
    {
        return -1;
    }
    shortid[0] = shortid[2];
    shortid[1] = shortid[3];
    shortid[2] = shortid[4];
    shortid[3] = shortid[5];
    shortid[5] = '\0';

    long temp_shortid = (strtol(shortid, NULL, 16));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    // this is disregarded
    if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs5Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    temp_data = temp_shortid;

    // temp_data &= ~(0xFF << 16);         // mask bytes
    // temp_data |= (temp_accuracy << 16); // set bytes
    //
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDs5Reg, &temp_data))
    {
        return -3;
    }

    // write
    temp_data = 0x00000020;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcDefaultDsInaccuracyValue(char *inaccuracy, size_t size)
{

    long temp_inaccuracy = (strtol(inaccuracy, NULL, 10));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    // temp_data = 0x00000000;
    //
    // if (0 != readRegister(temp_addr + Ucm_PtpOc_DefaultDs6Reg, &temp_data))
    //{
    //    return -2; // read current settings fails
    //}

    // temp_data &= ~(0xFF << 16);         // mask bytes
    // temp_data |= (temp_accuracy << 16); // set bytes
    //
    temp_data = temp_inaccuracy;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDs6Reg, &temp_data))
    {
        return -3;
    }

    // write
    temp_data = 0x00000040;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_DefaultDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

// number of ports - RO

//********************************
// port dataset
//********************************
int PortDsSetCustomIntervals = 0;
int readPtpOcPortDsSetCustomIntervalsStatus(char *status, size_t size)
{
    snprintf(status, size, "%s", "err");

    switch (PortDsSetCustomIntervals)
    {
    case 0:
        snprintf(status, size, "%s", "disabled");
        break;

    case 1:
        snprintf(status, size, "%s", "enabled");
        break;
    default:
        snprintf(status, size, "%s", "err");
    }
    return 0;
}
int writePtpOcPortDsSetCustomIntervalsStatus(char *status, size_t size)
{

    if (0 == strncmp(status, "enabled", size))
    {
        PortDsSetCustomIntervals = 1;
    }
    else
    {
        return -1;
    }

    if (0 == strncmp(status, "disabled", size))
    {
        PortDsSetCustomIntervals = 0;
    }
    else
    {
        return -1;
    }

    return 0;
}

int writePtpOcPortDsDelayReceiptTimeoutValue(char *timeout, size_t size)
{

    long temp_timeout = (strtol(timeout, NULL, 10));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs4Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    printf("read temp_data: 0x%08lx\n", temp_data);

    temp_data &= ~(0x000000FF << 16);  // mask bytes
    temp_data |= (temp_timeout << 16); // set bytes

    printf("write temp_data: 0x%08lx\n", temp_data);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDs4Reg, &temp_data))
    {
        return -3;
    }

    // set intervals
    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcPortDsDelayReqLogMsgIntervalValue(char *interval, size_t size)
{

    long temp_interval = (strtol(interval, NULL, 10));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs4Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    printf("read temp_data: 0x%08lx\n", temp_data);

    temp_data &= ~(0x000000FF << 8);   // mask bytes
    temp_data |= (temp_interval << 8); // set bytes

    printf("write temp_data: 0x%08lx\n", temp_data);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDs4Reg, &temp_data))
    {
        return -3;
    }

    // set intervals
    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcPortDsPDelayReqLogMsgIntervalValue(char *interval, size_t size)
{

    long temp_interval = (strtol(interval, NULL, 10));

    if (temp_interval < 0)
    {
        return -1; // o is too small
    }

    if (temp_interval >= 128)
    {
        return -1; // ooo is too biiig
    }

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs4Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    printf("read temp_data: 0x%08lx\n", temp_data);

    temp_data &= ~(0x000000FF << 0);   // mask bytes
    temp_data |= (temp_interval << 0); // set bytes

    printf("write temp_data: 0x%08lx\n", temp_data);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDs4Reg, &temp_data))
    {
        return -3;
    }

    // set intervals
    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcPortDsAnnounceReceiptTimeoutValue(char *timeout, size_t size)
{
    snprintf(timeout, size, "%s", "read-only");
    return 0;
    /*
    long temp_timeout = (strtol(timeout, NULL, 10));

    if (temp_timeout < 0)
    {
        return -1; // o is too small
    }

    if (temp_timeout > 127)
    {
        return -1; // ooo is too biiig
    }

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs5Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    printf("read temp_data: 0x%08lx\n", temp_data);

    temp_data &= ~(0x000000FF << 8);  // mask bytes
    temp_data |= (temp_timeout << 8); // set bytes

    printf("write temp_data: 0x%08lx\n", temp_data);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDs5Reg, &temp_data))
    {
        return -3;
    }

    // set intervals
    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
    */
}

int writePtpOcPortDsAnnounceLogMsgIntervalValue(char *interval, size_t size)
{

    long temp_interval = (strtol(interval, NULL, 10));

    if (temp_interval < 0)
    {
        return -1; // o is too small
    }

    if (temp_interval > 127)
    {
        return -1; // ooo is too biiig
    }

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs5Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    printf("read temp_data: 0x%08lx\n", temp_data);

    temp_data &= ~(0x000000FF << 0);   // mask bytes
    temp_data |= (temp_interval << 0); // set bytes

    printf("write temp_data: 0x%08lx\n", temp_data);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDs5Reg, &temp_data))
    {
        return -3;
    }

    // set intervals
    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcPortDsSyncReceiptTimeoutValue(char *timeout, size_t size)
{

    long temp_timeout = (strtol(timeout, NULL, 10));

    if (temp_timeout < 0)
    {
        return -1; // o is too small
    }

    if (temp_timeout > 127)
    {
        return -1; // ooo is too biiig
    }

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs6Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    printf("read temp_data: 0x%08lx\n", temp_data);

    temp_data &= ~(0x000000FF << 8);  // mask bytes
    temp_data |= (temp_timeout << 8); // set bytes

    printf("write temp_data: 0x%08lx\n", temp_data);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDs6Reg, &temp_data))
    {
        return -3;
    }

    // set intervals
    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcPortDsSyncLogMsgIntervalValue(char *interval, size_t size)
{

    long temp_interval = (strtol(interval, NULL, 10));

    if (temp_interval < 0)
    {
        return -1; // o is too small
    }

    if (temp_interval > 127)
    {
        return -1; // ooo is too biiig
    }

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs6Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    printf("read temp_data: 0x%08lx\n", temp_data);

    temp_data &= ~(0x000000FF << 0);   // mask bytes
    temp_data |= (temp_interval << 0); // set bytes

    printf("write temp_data: 0x%08lx\n", temp_data);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDs6Reg, &temp_data))
    {
        return -3;
    }

    // set intervals
    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcPortDsAsymmetryValue(char *asymmetry, size_t size)
{

    long temp_asymmetry = (strtol(asymmetry, NULL, 10));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs7Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    printf("read temp_data: 0x%08lx\n", temp_data);

    // temp_data &= ~(0x000000FF << 0);   // mask bytes
    // temp_data |= (temp_asymmetry << 0); // set bytes

    temp_data = temp_asymmetry;

    printf("write temp_data: 0x%08lx\n", temp_data);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDs7Reg, &temp_data))
    {
        return -3;
    }

    // set asymmetry
    temp_data = 0x00000004;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcPortDsMaxPeerDelayValue(char *delay, size_t size)
{

    long temp_delay = (strtol(delay, NULL, 10));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_PtpOc_PortDs8Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    printf("read temp_data: 0x%08lx\n", temp_data);

    // temp_data &= ~(0x000000FF << 0);   // mask bytes
    // temp_data |= (temp_asymmetry << 0); // set bytes

    temp_data = temp_delay;

    printf("write temp_data: 0x%08lx\n", temp_data);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDs8Reg, &temp_data))
    {
        return -3;
    }

    // set delay
    temp_data = 0x00000008;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_PortDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int TimePropertiesDsSetLocalPropertiesCheckBox = 0;
int readPtpOcTimePropertiesDsSetLocalPropertiesCheckBox(char *status, size_t size)
{
    snprintf(status, size, "%s", "err");

    switch (TimePropertiesDsSetLocalPropertiesCheckBox)
    {
    case 0:
        snprintf(status, size, "%s", "disabled");
        break;

    case 1:
        snprintf(status, size, "%s", "enabled");
        break;
    default:
        snprintf(status, size, "%s", "err");
    }
    return 0;
}
int writePtpOcTimePropertiesDsSetLocalPropertiesCheckBox(char *status, size_t size)
{

    if (0 == strncmp(status, "enabled", size))
    {
        TimePropertiesDsSetLocalPropertiesCheckBox = 1;
    }
    else
    {
        return -1;
    }

    if (0 == strncmp(status, "disabled", size))
    {
        TimePropertiesDsSetLocalPropertiesCheckBox = 0;
    }
    else
    {
        return -1;
    }

    return 0;
}

int writePtpOcTimePropertiesDsTimeSourceValue(char *source, size_t size)
{

    if (strlen(source) != 4)
    {
        return -1;
    }
    source[0] = source[2];
    source[1] = source[3];
    source[2] = '\0';

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -2; // read current settings fails
    }
    // remove current domain
    temp_data &= ~0x000000FF;
    // temp_data &= 0x000000FF;

    temp_data |= strtol(source, NULL, 16);

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000080;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcTimePropertiesDsPtpTimescaleStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -1;
    }

    // temp_data &= 0x0000FFFF;
    //
    temp_data &= ~0x00000100;

    if (0 == strncmp(status, "enabled", size))
    {
        temp_data = 0x00000100 | temp_data;
    }
    else if (0 == strncmp(status, "disabled", size))
    {
        temp_data = 0x00000000 | temp_data; // disable
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000040;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4;
    }

    return 0;
}

int writePtpOcTimePropertiesDsFreqTraceableStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -1;
    }

    // temp_data &= 0x0000FFFF;
    //
    temp_data &= ~0x00000200;

    if (0 == strncmp(status, "enabled", size))
    {
        temp_data = 0x00000200 | temp_data;
    }
    else if (0 == strncmp(status, "disabled", size))
    {
        temp_data = 0x00000000 | temp_data; // disable
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000020;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4;
    }

    return 0;
}

int writePtpOcTimePropertiesDsTimeTraceableStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -1;
    }

    // temp_data &= 0x0000FFFF;
    //
    temp_data &= ~0x00000400;

    if (0 == strncmp(status, "enabled", size))
    {
        temp_data = 0x00000400 | temp_data;
    }
    else if (0 == strncmp(status, "disabled", size))
    {
        temp_data = 0x00000000 | temp_data; // disable
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000010;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4;
    }

    return 0;
}

int writePtpOcTimePropertiesDsLeap61Status(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -1;
    }

    // temp_data &= 0x0000FFFF;
    //
    temp_data &= ~0x00000800;

    if (0 == strncmp(status, "enabled", size))
    {
        temp_data = 0x00000800 | temp_data;
    }
    else if (0 == strncmp(status, "disabled", size))
    {
        temp_data = 0x00000000 | temp_data; // disable
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000008;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4;
    }

    return 0;
}

int writePtpOcTimePropertiesDsLeap59Status(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -1;
    }

    // temp_data &= 0x0000FFFF;
    //
    temp_data &= ~0x00001000;

    if (0 == strncmp(status, "enabled", size))
    {
        temp_data = 0x00001000 | temp_data;
    }
    else if (0 == strncmp(status, "disabled", size))
    {
        temp_data = 0x00000000 | temp_data; // disable
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000004;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4;
    }

    return 0;
}

int writePtpOcTimePropertiesDsUtcOffsetValStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -1;
    }

    // temp_data &= 0x0000FFFF;
    //
    temp_data &= ~0x00002000;

    if (0 == strncmp(status, "enabled", size))
    {
        temp_data = 0x00002000 | temp_data;
    }
    else if (0 == strncmp(status, "disabled", size))
    {
        temp_data = 0x00000000 | temp_data; // disable
    }
    else
    {
        return -2;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4;
    }

    return 0;
}

int writePtpOcTimePropertiesDsUtcOffsetValue(char *offset, size_t size)
{
    long temp_offset = (strtol(offset, NULL, 10));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    printf("read temp_data: 0x%08lx\n", temp_data);
    // clear the top of temp_data we just read
    temp_data &= ~0xFFFF0000;
    temp_data |= (temp_offset << 16);
    // temp_data &= 0x000000FF;
    printf("write temp_data: 0x%08lx\n", temp_data);
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs1Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcTimePropertiesDsCurrentOffsetValue(char *offset, size_t size)
{
    long temp_offset = (strtol(offset, NULL, 10));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs2Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    // printf("read temp_data: 0x%08lx\n", temp_data);
    // clear the top of temp_data we just read
    // temp_data &= ~0xFFFF0000;
    // temp_data |= (temp_offset << 16);
    // temp_data &= 0x000000FF;

    temp_data = temp_offset;
    // printf("write temp_data: 0x%08lx\n", temp_data);
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs2Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000100;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcTimePropertiesDsJumpSecondsValue(char *seconds, size_t size)
{
    long temp_seconds = (strtol(seconds, NULL, 10));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs3Reg, &temp_data))
    {
        return -2; // read current settings fails
    }

    // printf("read temp_data: 0x%08lx\n", temp_data);
    // clear the top of temp_data we just read
    // temp_data &= ~0xFFFF0000;
    // temp_data |= (temp_offset << 16);
    // temp_data &= 0x000000FF;

    temp_data = temp_seconds;
    // printf("write temp_data: 0x%08lx\n", temp_data);
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs3Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000200;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcTimePropertiesDsNextJumpValue(char *next, size_t size)
{
    long temp_next = (strtol(next, NULL, 10));

    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    // unused
    // if (0 != readRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs4Reg, &temp_data))
    //{
    //    return -2; // read current settings fails
    //}

    // printf("read temp_data: 0x%08lx\n", temp_data);
    // clear the top of temp_data we just read
    // temp_data &= ~0x00000000;
    temp_data = (temp_next >> 32);
    // temp_data &= 0x000000FF;

    // temp_data = temp_seconds;
    //  printf("write temp_data: 0x%08lx\n", temp_data);
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs4Reg, &temp_data))
    {
        return -3;
    }

    temp_data = temp_next & 0x00000000FFFFFFFF;

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs5Reg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000400;
    if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writePtpOcTimePropertiesDsDisplayNameValue(char *name, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;

    for (int j = 0; j < 3; j++)
    {
        temp_data = 0x00000000;
        if ((j * 4) + 0 < strlen(name))
        {
            temp_data |= ((name[((j * 4) + 0)]) & 0x000000FF) << 0;
        }

        if ((j * 4) + 1 < strlen(name))
        {
            temp_data |= ((name[((j * 4) + 1)]) & 0x000000FF) << 8;
        }

        if ((j * 4) + 2 < strlen(name))
        {
            temp_data |= ((name[((j * 4) + 2)]) & 0x000000FF) << 16;
        }

        if ((j * 4) + 3 < strlen(name))
        {
            temp_data |= ((name[((j * 4) + 3)]) & 0x000000FF) << 24;
        }

        if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs7Reg + (j * 4), &temp_data))
        {
            return -1;
        }
        if (j == 2)
        {
            temp_data = strlen(name); // length
            if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDs6Reg, &temp_data))
            {
                return -2;
            }

            temp_data = 0x00001800; // write
            if (0 != writeRegister(temp_addr + Ucm_PtpOc_TimePropertiesDsControlReg, &temp_data))
            {
                return -3;
            }

            // ui->PtpOcTimePropertiesDsDisplayNameValue->setText(temp_string);
        }
    }
}

int writePtpOcStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_PtpOrdinaryClockCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 == strncmp(status, "enabled", size))
    {
        temp_data |= 0x00000001;
    }

    if (0 != writeRegister(temp_addr + Ucm_PtpOc_ControlReg, &temp_data))
    {
        return -1;
    }
    return 0;
}