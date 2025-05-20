
#include "axi.h"
#include "coreConfig.h"
#include "ntpServer.h"
#include "config.h"

int64_t ntplow;
int64_t temp_data = 0x00000000;
long temp_addr = 0x00000000;

int hasNtpServer(char *in, size_t size)
{
    if (Ucm_CoreConfig_NtpServerCoreType != cores[Ucm_CoreConfig_NtpServerCoreType].core_type)
    {

        return -1;
    }

    return 0;
}
// read Ntp Server Status ======================================================
int readNtpServerStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

    // enabled
    if (0 != readRegister(temp_addr + Ucm_NtpServer_ControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");

        return -1;
    }

    if ((temp_data & 0x00000001) == 0)
    {
        snprintf(status, size, "%s", "disabled");
        ////ui->NtpServerEnableCheckBox->setChecked(false);
    }

    else
    {
        snprintf(status, size, "%s", "enabled");
        // ui->NtpServerEnableCheckBox->setChecked(true);
    }

    return 0;
}
// read ntp server mac address ======================================================
int readNtpServerMacAddress(char *macAddr, size_t size)
{
    temp_data = 0x00000000;
    long temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    unsigned char temp_mac[6];
    // mac
    // temp_string.clear();

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigMac1Reg, &temp_data))
    {
        snprintf(macAddr, size, "%s", "NA");

        return -1;
    }

    temp_mac[0] = ((temp_data >> 0) & 0x000000FF);
    temp_mac[1] = ((temp_data >> 8) & 0x000000FF);
    temp_mac[2] = ((temp_data >> 16) & 0x000000FF);
    temp_mac[3] = ((temp_data >> 24) & 0x000000FF);

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigMac2Reg, &temp_data))
    {
        snprintf(macAddr, size, "%s", "NA");

        return -1;
    }

    temp_mac[4] = ((temp_data >> 0) & 0x000000FF);
    temp_mac[5] = ((temp_data >> 8) & 0x000000FF);

    snprintf(macAddr, size, "%02x:%02x:%02x:%02x:%02x:%02x", temp_mac[0], temp_mac[1], temp_mac[2], temp_mac[3], temp_mac[4], temp_mac[5]);

    // ui->NtpServerMacValue->setText(temp_string);

    return 0;
}

// vlan status
int readNtpServerVlanStatus(char *vlanStatus, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigVlanReg, &temp_data))
    {
        snprintf(vlanStatus, size, "%s", "NA");
        return -1;
    }

    if ((temp_data & 0x00010000) == 0)
    {
        // ui->NtpServerVlanEnableCheckBox->setChecked(false);
        snprintf(vlanStatus, size, "%s", "disabled");
    }
    else
    {
        // ui->NtpServerVlanEnableCheckBox->setChecked(true);
        snprintf(vlanStatus, size, "%s", "enabled");
    }

    temp_data &= 0x0000FFFF;

    // ui->NtpServerVlanValue->setText(QString("0x%1").arg(temp_data, 4, 16, QLatin1Char('0')));
    return 0;
}

// vlan addr
int readNtpServerVlanAddress(char *vlanAddr, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigVlanReg, &temp_data))
    {
        snprintf(vlanAddr, size, "%s", "NA");

        return -1;
    }

    temp_data &= 0x0000FFFF;
    snprintf(vlanAddr, size, "0x%04lx", temp_data);

    // ui->NtpServerVlanValue->setText(QString("0x%1").arg(temp_data, 4, 16, QLatin1Char('0')));
    return 0;
}

// read Ntp Server IP MODE ======================================================
int readNtpServerIpMode(char *ipMode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    // mode & server config
    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        snprintf(ipMode, size, "%s", "NA");
        return -1;
    }

    if (((temp_data >> 0) & 0x00000003) == 1)
    {
        snprintf(ipMode, size, "%s", "IPv4");
    }
    else if (((temp_data >> 0) & 0x00000003) == 2)
    {
        snprintf(ipMode, size, "%s", "IPv6");
    }
    else
    {
        snprintf(ipMode, size, "%s", "NA");
    }

    return 0;
}

// read Ntp Server Unicast mode ======================================================
int readNtpServerUnicastMode(char *mode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        snprintf(mode, size, "%s", "NA");
        return -1;
    }

    if (((temp_data) & 0x00000010) == 0)
    {
        snprintf(mode, size, "%s", "disabled");
    }

    else
    {
        snprintf(mode, size, "%s", "enabled");
    }

    return 0;
}

// read Ntp Server Multicast mode ======================================================
int readNtpServerMulticastMode(char *mode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        snprintf(mode, size, "%s", "NA");
        return -1;
    }

    if (((temp_data) & 0x00000020) == 0)
    {
        snprintf(mode, size, "%s", "disabled");
    }

    else
    {
        snprintf(mode, size, "%s", "enabled");
    }

    return 0;
}

// read Ntp Server Broadcast mode ======================================================
int readNtpServerBroadcastMode(char *mode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        snprintf(mode, size, "%s", "NA");
        return -1;
    }

    if (((temp_data) & 0x00000040) == 0)
    {
        snprintf(mode, size, "%s", "disabled");
    }

    else
    {
        snprintf(mode, size, "%s", "enabled");
    }

    return 0;
}

// read Ntp Server Precision mode ======================================================
int readNtpServerPrecisionValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");
        return -1;
    }
    // ui->NtpServerPrecisionValue->setText(QString::number((char)((temp_data >> 8) & 0x000000FF)));
    snprintf(value, size, "%d", (char)((temp_data >> 8) & 0x000000FF));

    return 0;
}
// read Ntp Server PollIntervalValue mode ======================================================
int readNtpServerPollIntervalValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");
        return -1;
    }
    // ui->NtpServerPrecisionValue->setText(QString::number((char)((temp_data >> 8) & 0x000000FF)));
    snprintf(value, size, "%ld", ((temp_data >> 16) & 0x000000FF));

    return 0;
}

// read Ntp Server Stratum value mode ======================================================
int readNtpServerStratumValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");
        return -1;
    }
    // ui->NtpServerPrecisionValue->setText(QString::number((char)((temp_data >> 8) & 0x000000FF)));
    snprintf(value, size, "%ld", ((temp_data >> 24) & 0x000000FF));

    return 0;
}

int readNtpServerReferenceId(char *refId, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    char temp_refid[4] = {0};

    // reference id
    // temp_string.clear();
    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigReferenceIdReg, &temp_data))
    {
        snprintf(refId, size, "%s", "NA");
        return -1;
    }
    // temp_string.append(temp_refid[0] = (QChar)((temp_data >> 24) & 0x000000FF));
    // temp_string.append(temp_refid[0] = (QChar)((temp_data >> 16) & 0x000000FF));
    // temp_string.append(temp_refid[0] = (QChar)((temp_data >> 8) & 0x000000FF));
    // temp_string.append(temp_refid[0] = (QChar)((temp_data >> 0) & 0x000000FF));
    //
    temp_refid[0] = (char)((temp_data >> 24) & 0x000000FF);
    temp_refid[1] = (char)((temp_data >> 16) & 0x000000FF);
    temp_refid[2] = (char)((temp_data >> 8) & 0x000000FF);
    temp_refid[3] = (char)((temp_data >> 0) & 0x000000FF);
    snprintf(refId, size, "%c%c%c%c", temp_refid[0], temp_refid[1], temp_refid[2], temp_refid[3]);

    // ui->NtpServerReferenceIdValue->setText(temp_string); // TODO

    return 0;
}

// read NtpServer IP ADDRESS ======================================================
int readNtpServerIpAddress(char *ipAddr, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    int64_t temp_ip = 0;
    char ipMode[size];

    int err = readNtpServerIpMode(ipMode, size);

    if (err != 0)
    {
        snprintf(ipAddr, size, "%s", "mode err");
        return -1;
    }

    if (0 == strncmp(ipMode, "IPv4", size))
    {
        if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigIpReg, &temp_data))
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
    else if (0 == strncmp(ipMode, "IPv6", size))
    {
        unsigned char temp_ip6[16];
        // temp_string.clear();
        if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigIpReg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err0-3");
            return -1;
        }
        temp_ip6[0] = (temp_data >> 0) & 0x000000FF;
        temp_ip6[1] = (temp_data >> 8) & 0x000000FF;
        temp_ip6[2] = (temp_data >> 16) & 0x000000FF;
        temp_ip6[3] = (temp_data >> 24) & 0x000000FF;

        if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigIpv61Reg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err4-7");
            return -1;
        }
        temp_ip6[4] = (temp_data >> 0) & 0x000000FF;
        temp_ip6[5] = (temp_data >> 8) & 0x000000FF;
        temp_ip6[6] = (temp_data >> 16) & 0x000000FF;
        temp_ip6[7] = (temp_data >> 24) & 0x000000FF;

        if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigIpv62Reg, &temp_data))
        {
            snprintf(ipAddr, size, "%s", "err8-11");
            return -1;
        }
        temp_ip6[8] = (temp_data >> 0) & 0x000000FF;
        temp_ip6[9] = (temp_data >> 8) & 0x000000FF;
        temp_ip6[10] = (temp_data >> 16) & 0x000000FF;
        temp_ip6[11] = (temp_data >> 24) & 0x000000FF;

        if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigIpv63Reg, &temp_data))
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

int readNtpServerSmearingStatus(char *status, size_t size)
{
    // utc info
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x40000000;
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(status, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
        {
            snprintf(status, size, "%s", "read err");
            return -1;
        }

        if ((temp_data & 0x80000000) != 0)
        {
            if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
            {
                return -1;
            }

            if ((temp_data & 0x00000100) == 0)
            {
                snprintf(status, size, "%s", "disabled");
                // snprintf(status, size, "%lx", temp_data);
            }
            else
            {
                snprintf(status, size, "%s", "enabled");
            }
            break;
        }
    }

    return 0;
}

int readNtpServerLeap61Progress(char *progress, size_t size)
{
    // utc info
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x40000000;
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        snprintf(progress, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(progress, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
        {
            snprintf(progress, size, "%s", "read err");
            return -1;
        }

        if ((temp_data & 0x80000000) != 0)
        {
            if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
            {
                return -1;
            }

            if ((temp_data & 0x00000200) == 0)
            {
                snprintf(progress, size, "%s", "disabled");
                // snprintf(status, size, "%lx", temp_data);
            }
            else
            {
                snprintf(progress, size, "%s", "enabled");
            }
            break;
        }
    }

    return 0;
}
int readNtpServerLeap59Progress(char *progress, size_t size)
{
    // utc info
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x40000000;
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        snprintf(progress, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(progress, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
        {
            snprintf(progress, size, "%s", "read err");
            return -1;
        }

        if ((temp_data & 0x80000000) != 0)
        {
            if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
            {
                return -1;
            }

            if ((temp_data & 0x00000400) == 0)
            {
                snprintf(progress, size, "%s", "disabled");
                // snprintf(status, size, "%lx", temp_data);
            }
            else
            {
                snprintf(progress, size, "%s", "enabled");
            }
            break;
        }
    }

    return 0;
}
int readNtpServerLeap61Status(char *status, size_t size) // leap 61 0x00000800
{
    // utc info
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x40000000;
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(status, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
        {
            snprintf(status, size, "%s", "read err");
            return -1;
        }

        if ((temp_data & 0x80000000) != 0)
        {
            if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
            {
                return -1;
            }

            if ((temp_data & 0x00000800) == 0)
            {
                snprintf(status, size, "%s", "disabled");
                // snprintf(status, size, "%lx", temp_data);
            }
            else
            {
                snprintf(status, size, "%s", "enabled");
            }
            break;
        }
    }

    return 0;
}
int readNtpServerLeap59Status(char *status, size_t size)
{
    // utc info
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x40000000;
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(status, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
        {
            snprintf(status, size, "%s", "read err");
            return -1;
        }

        if ((temp_data & 0x80000000) != 0)
        {
            if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
            {
                return -1;
            }

            if ((temp_data & 0x00001000) == 0)
            {
                snprintf(status, size, "%s", "disabled");
                // snprintf(status, size, "%lx", temp_data);
            }
            else
            {
                snprintf(status, size, "%s", "enabled");
            }
            break;
        }
    }

    return 0;
}
int readNtpServerUtcOffsetStatus(char *status, size_t size)
{
    // utc info
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x40000000;
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(status, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
        {
            snprintf(status, size, "%s", "read err");
            return -1;
        }

        if ((temp_data & 0x80000000) != 0)
        {
            if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
            {
                return -1;
            }

            if ((temp_data & 0x00002000) == 0)
            {
                snprintf(status, size, "%s", "disabled");
                // snprintf(status, size, "%lx", temp_data);
            }
            else
            {
                snprintf(status, size, "%s", "enabled");
            }
            break;
        }
    }

    return 0;
}
int readNtpServerUtcOffsetValue(char *value, size_t size)
{
    // utc info
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x40000000;
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        snprintf(value, size, "%s", "err");
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            snprintf(value, size, "%s", "err: read did not complete");
            return -1;
        }
        if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
        {
            snprintf(value, size, "%s", "read err");
            return -1;
        }

        if ((temp_data & 0x80000000) != 0)
        {
            if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
            {
                snprintf(value, size, "%s", "NA");
                return -1;
            }

            snprintf(value, size, "%ld", ((temp_data >> 16) & 0x0000FFFF));
            break;
        }
    }

    return 0;
}

int readNtpServerRequestsValue(char *value, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    snprintf(value, size, "%s", "err");
    if (0 != readRegister(temp_addr + Ucm_NtpServer_CountReqReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");
        return -1;
    }
    // ui->NtpServerRequestsValue->setText(QString::number(temp_data));
    snprintf(value, size, "%ld", temp_data);
    return 0;
}
int readNtpServerResponsesValue(char *value, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    snprintf(value, size, "%s", "err");
    if (0 != readRegister(temp_addr + Ucm_NtpServer_CountRespReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");

        return -1;
    }
    // ui->NtpServerRequestsValue->setText(QString::number(temp_data));
    snprintf(value, size, "%ld", temp_data);

    return 0;
}
int readNtpServerRequestsDroppedValue(char *value, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    snprintf(value, size, "%s", "err");
    if (0 != readRegister(temp_addr + Ucm_NtpServer_CountReqDroppedReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");
        return -1;
    }
    // ui->NtpServerRequestsValue->setText(QString::number(temp_data));
    snprintf(value, size, "%ld", temp_data);

    return 0;
}
int readNtpServerBroadcastsValue(char *value, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    snprintf(value, size, "%s", "err");
    if (0 != readRegister(temp_addr + Ucm_NtpServer_CountBroadcastReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");

        return -1;
    }
    // ui->NtpServerRequestsValue->setText(QString::number(temp_data));
    snprintf(value, size, "%ld", temp_data);

    return 0;
}
int readNtpServerClearCountersStatus(char *status, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    if (0 != readRegister(temp_addr + Ucm_NtpServer_CountControlReg, &temp_data))
    {
        snprintf(status, size, "%s", "NA");

        return -1;
    }
    if ((temp_data & 0x00000001) == 0)
    {
        snprintf(status, size, "%s", "disable");
    }
    else
    {
        snprintf(status, size, "%s", "enable");
    }

    return 0;
}
int readNtpServerVersion(char *value, size_t size)
{
    // version
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_VersionReg, &temp_data))
    {
        snprintf(value, size, "%s", "NA");

        return -1;
    }
    // ui->NtpServerVersionValue->setText(QString("0x%1").arg(temp_data, 8, 16, QLatin1Char('0')));
    snprintf(value, size, "0x%lx", temp_data);

    return 0;
}

// read Ntp Server Instance Number ======================================================
int readNtpServerInstanceNumber(char *instanceNumber, size_t size)
{
    snprintf(instanceNumber, size, "%ld", cores[Ucm_CoreConfig_NtpServerCoreType].core_instance_nr);
    return 0;
}

//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//
//========================================//========================================//========================================//

int writeNtpServerMacAddress(char *addr, size_t size)
{ // mac
    // readConfig();
    // AA:BB:CC:DD:EE:FF
    if (strlen(addr) > 17)
    {
        return -1;
    }
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    // int j = 0;
    long temp_mac = 0;
    for (int i = 0, j = 0; i < size; i++)
    {
        if (addr[i] != ':')
        {
            addr[j] = addr[i];
            j++;
        }

        if (addr[i] == '\0')
        {
            break;
        }
    }

    temp_mac = strtol(addr, NULL, 16);

    temp_data = 0x00000000;
    temp_data |= (temp_mac >> 16) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_mac >> 24) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_mac >> 32) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_mac >> 40) & 0x000000FF;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigMac1Reg, &temp_data))
    {
        return -1;
    }

    temp_data = 0x00000000;
    temp_data |= (temp_mac >> 0) & 0x000000FF;
    temp_data = temp_data << 8;
    temp_data |= (temp_mac >> 8) & 0x000000FF;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigMac2Reg, &temp_data))
    {
        return -1;
    }

    temp_data = 0x00000004; // write
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1;
    }
    // write success
    return 0;
}

int writeNtpServerVlanStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigVlanReg, &temp_data))
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

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigVlanReg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -4;
    }

    return 0;
}
int writeNtpServerVlanAddress(char *value, size_t size)
{
    // return -1;
    //  readConfig();
    if (strlen(value) > 6)
    {
        return -1;
    }
    temp_addr = temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    long temp_vlan = 0;
    value = &value[2];

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigVlanReg, &temp_data))
    {
        return -2; // read current settings fails
    }

    temp_vlan = strtol(value, NULL, 16);

    temp_data &= 0xFFFF0000;

    temp_data |= temp_vlan;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigVlanReg, &temp_data))
    {
        return -3;
    }

    temp_data = 0x00000002;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -4; // failed to write control reg
    }

    return 0;
}

int writeNtpServerIpMode(char *mode, const size_t size)
{
    char currentAddress[size];
    int err = readNtpServerIpAddress(currentAddress, size);
    if (err != 0)
    {
        return -1;
    }

    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -2;
    }

    if (0 == strncmp(mode, "IPv4", size))
    {
        temp_data &= ~0x01000002;
        temp_data |= 0x00000001;
    }
    else if (0 == strncmp(mode, "IPv6", size))
    {
        char ipv6Address[size];
        printf("current address: %s\n", currentAddress);

        temp_data &= ~0x00000001;
        temp_data |= 0x01000002;

        printf("IPv6 address: %s\n", ipv6Address);
    }
    else
    {
        return -3;
    }

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        // failed to update config
        return -4;
    }
    temp_data = 0x00000001;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        // failed to update config control reg
        return -5;
    }
    // printf("current IP address: %s\n", tempIpAddress);

    if (0 != writeNtpServerIpAddress(currentAddress, size))
    {
        return -6;
    }

    return 0;
}
int writeNtpServerIpAddress(char *ipAddress, size_t size)
{
    char currentMode[size];

    if (0 != readNtpServerIpMode(currentMode, size))
    {
        printf("failed to read current mode\n");
        return -1;
    }

    if (0 == strncmp(currentMode, "IPv4", size))
    {
        if (0 != ipv4_addr_to_register_value(ipAddress, size))
        {
            printf("failed to write ipv4 crap\n");
            return -4;
        }
    }
    else if (0 == strncmp(currentMode, "IPv6", size))
    {
        ipv6_addr_to_register_value(ipAddress, size);
    }
    else
    {
        return -6;
    }
    return 0;
}

int ipv4_addr_to_register_value(char *ipAddress, size_t size)
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

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigIpReg, &temp_data))
    {
        printf("failed to write config \n");
        return -1;
    }
    temp_data = 0x00000008; // write
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        printf("failed to write control \n");
        return -1;
    }
    return 0;
}
int ipv6_addr_to_register_value(char *ipAddress, size_t size)
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

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigIpReg, &temp_data))
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

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigIpv61Reg, &temp_data))
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

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigIpv62Reg, &temp_data))
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

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigIpv63Reg, &temp_data))
    {
        return -4;
    }
    temp_data = 0x00000008; // write
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -5;
    }
    return 0;
}

int writeNtpServerUnicastMode(char *mode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }

    temp_data &= ~0x00000010;

    if (0 != strncmp(mode, "enabled", size))
    {
        return -1;
    }
    temp_data |= 0x000000010;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }

    temp_data = 0x00000001;
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1;
    }

    return 0;
}

int writeNtpServerMulticastMode(char *mode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }
    temp_data &= ~0x00000020;

    if (0 != strncmp(mode, "enabled", size))
    {
        return -1;
    }
    temp_data |= 0x000000020;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }
    temp_data = 0x00000001;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1; // contrl error
    }
    return 0;
}
int writeNtpServerBroadcastMode(char *mode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }

    temp_data &= ~0x00000040;

    if (0 != strncmp(mode, "enabled", size))
    {
        return -1;
    }

    temp_data |= 0x000000040;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }

    temp_data = 0x00000001;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1;
    }

    return 0;
}

int writeNtpServerPrecisionValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

    temp_data = 0x00000000;
    char *err;
    long temp_precision = 0;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }

    temp_precision = strtol(value, &err, 16);

    if (err == value || *err != '\0')
    {
        return -1;
    }
    temp_data &= ~((0xFFFFFFFF & 0x000000FF) << 8);
    temp_data |= ((temp_precision & 0x000000FF) << 8);

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }

    temp_data = 0x00000001;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1;
    }
}

int writeNtpServerPollIntervalValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

    temp_data = 0x00000000;
    char *err;
    long temp_precision = 0;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }

    temp_precision = strtol(value, &err, 16);

    if (err == value || *err != '\0')
    {
        return -1;
    }
    temp_data &= ~((0xFFFFFFFF & 0x000000FF) << 16);
    temp_data |= ((temp_precision & 0x000000FF) << 16);

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }

    temp_data = 0x00000001;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1;
    }
}

int writeNtpServerStratumValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    char *err;
    long temp_stratum = 0;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }

    temp_stratum = strtol(value, &err, 16);

    if (err == value || *err != '\0')
    {
        return -1;
    }
    temp_data &= ~((0xFFFFFFFF & 0x000000FF) << 24);
    temp_data |= ((temp_stratum & 0x000000FF) << 24);

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }
    temp_data = 0x00000001;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1;
    }
    return 0;
}
int writeNtpServerReferenceIdValue(char *referenceId, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    char *token;
    char referenceOptions[91] = "NTP,NULL,LOCL,CESM,RBDM,PPS,IRIG,ACTS,USNO,PTB,TDF,DCF,MSF,WWV,WWVB,WWVH,CHU,LORC,OMEG,GPS";

    for (int i = 0; i < size; i++) // num of ref options 20 + 1
    {
        token = (i == 0) ? strtok(referenceOptions, ",") : strtok(NULL, ",");
        if (token == NULL)
        {
            return -1;
        }

        if (0 == strncmp(token, referenceId, strlen(referenceId)))
        {
            break;
        }
    }

    if (1 == strlen(referenceId))
    {
        temp_data |= referenceId[0];
        temp_data = temp_data << 24;
    }
    else if (2 == strlen(referenceId))
    {
        temp_data |= referenceId[0];
        temp_data = temp_data << 8;
        temp_data |= referenceId[1];
        temp_data = temp_data << 16;
    }
    else if (3 == strlen(referenceId))
    {
        temp_data |= referenceId[0];
        temp_data = temp_data << 8;
        temp_data |= referenceId[1];
        temp_data = temp_data << 8;
        temp_data |= referenceId[2];
        temp_data = temp_data << 8;
    }
    else if (4 == strlen(referenceId))
    {
        temp_data |= referenceId[0];
        temp_data = temp_data << 8;
        temp_data |= referenceId[1];
        temp_data = temp_data << 8;
        temp_data |= referenceId[2];
        temp_data = temp_data << 8;
        temp_data |= referenceId[3];
    }
    else
    {
        return -1;
    }

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigReferenceIdReg, &temp_data))
    {
        return -1;
    }
    temp_data = 0x00000010;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1;
    }
    return 0;
}
int writeNtpServerUtcSmearingStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
    {
        return -1;
    }

    temp_data &= ~0x00000100; // Clear bit (using NOT + AND)
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
        return -1;
    }

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
    {
        return -1;
    }
    temp_data = 0x00000003;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        return -1;
    }
    return 0;
}
int writeNtpServerLeap61Status(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
    {
        return -1;
    }

    temp_data &= ~0x00000800; // Clear bit (using NOT + AND)
    if (0 == strcmp(status, "enabled"))
    {
        temp_data |= 0x00000800;
    }
    else if (0 == strcmp(status, "disabled"))
    {
        temp_data |= 0x00000000;
    }
    else
    {
        return -1;
    }

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
    {
        return -1;
    }
    temp_data = 0x00000003;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        return -1;
    }
    return 0;
}
int writeNtpServerLeap59Status(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
    {
        return -1;
    }

    temp_data &= ~0x00001000; // Clear bit (using NOT + AND)
    if (0 == strcmp(status, "enabled"))
    {
        temp_data |= 0x00001000;
    }
    else if (0 == strcmp(status, "disabled"))
    {
        temp_data |= 0x00000000;
    }
    else
    {
        return -1;
    }

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
    {
        return -1;
    }
    temp_data = 0x00000003;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        return -1;
    }
    return 0;
}
int writeNtpServerUtcOffsetStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
    {
        return -1;
    }

    temp_data &= ~0x00002000; // Clear bit (using NOT + AND)
    if (0 == strcmp(status, "enabled"))
    {
        temp_data |= 0x00002000;
    }
    else if (0 == strcmp(status, "disabled"))
    {
        temp_data |= 0x00000000;
    }
    else
    {
        return -1;
    }

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
    {
        return -1;
    }
    temp_data = 0x00000003;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        return -1;
    }
    return 0;
}
int writeNtpServerUtcOffsetValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    long temp_value = 0;
    char *err;
    if (0 != readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
    {
        return -1;
    }

    temp_data &= 0x0000FFFF;

    temp_value = strtol(value, &err, 10);

    if (err == value || *err != '\0')
    {
        return -1;
    }

    temp_data |= ((temp_value & 0x0000FFFF) << 16);

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
    {
        return -1;
    }
    temp_data = 0x00000003;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        return -1;
    }
    return 0;
}
int writeNtpServerClearCountersStatus(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    temp_data |= 0x00000001; // clear 'em!

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_CountControlReg, &temp_data))
    {
        return -1;
    }
    return 0;
}

//
int writeStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

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
        return -1;
        // printf("PLEASE ENTER A VALID STATUS\n");
        // temp_data = 0x00000000;
    }

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ControlReg, &temp_data))
    {
        return -1;
    }

    return 0;
}
