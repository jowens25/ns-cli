
#include "axi.h"
#include "coreConfig.h"
#include "ntpServer.h"
#include "config.h"

int64_t ntplow;
int64_t temp_data = 0x00000000;
long temp_addr = 0x00000000;
// read Ntp Server Status ======================================================
int readNtpServerStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    readConfig();

    // enabled
    if (0 == readRegister(temp_addr + Ucm_NtpServer_ControlReg, &temp_data))

    {
        if ((temp_data & 0x00000001) == 0)
        {
            snprintf(status, size, "%s", "disabled");
            return 0;
            ////ui->NtpServerEnableCheckBox->setChecked(false);
        }
        else
        {
            snprintf(status, size, "%s", "enabled");
            return 1;
            // ui->NtpServerEnableCheckBox->setChecked(true);
        }
    }
    else
    {
        snprintf(status, size, "%s", "disabled");
        return 0;
        // ui->NtpServerEnableCheckBox->setChecked(false);
    }
}
// read ntp server mac address ======================================================
int readNtpServerMacAddress(char *macAddr, size_t size)
{
    temp_data = 0x00000000;
    long temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    unsigned char temp_mac[6];
    // mac
    // temp_string.clear();
    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigMac1Reg, &temp_data))
    {
        temp_mac[0] = ((temp_data >> 0) & 0x000000FF);
        temp_mac[1] = ((temp_data >> 8) & 0x000000FF);
        temp_mac[2] = ((temp_data >> 16) & 0x000000FF);
        temp_mac[3] = ((temp_data >> 24) & 0x000000FF);

        if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigMac2Reg, &temp_data))
        {

            temp_mac[4] = ((temp_data >> 0) & 0x000000FF);
            temp_mac[5] = ((temp_data >> 8) & 0x000000FF);

            snprintf(macAddr, size, "%02x:%02x:%02x:%02x:%02x:%02x", temp_mac[0], temp_mac[1], temp_mac[2], temp_mac[3], temp_mac[4], temp_mac[5]);

            // ui->NtpServerMacValue->setText(temp_string);
        }
        else
        {
            snprintf(macAddr, size, "%s", "NA");

            // ui->NtpServerMacValue->setText("NA");
        }
    }
    else
    {
        snprintf(macAddr, size, "%s", "NA");

        // ui->NtpServerMacValue->setText("NA");
    }
}

// vlan status
int readNtpServerVlanStatus(char *vlanStatus, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigVlanReg, &temp_data))
    {
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
    }
    else
    {
        snprintf(vlanStatus, size, "%s", "disabled");

        // ui->NtpServerVlanEnableCheckBox->setChecked(false);
        // ui->NtpServerVlanValue->setText("NA");
    }
}

// vlan addr
int readNtpServerVlanAddress(char *vlanAddr, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigVlanReg, &temp_data))
    {

        temp_data &= 0x0000FFFF;
        snprintf(vlanAddr, size, "0x%04lx", temp_data);

        // ui->NtpServerVlanValue->setText(QString("0x%1").arg(temp_data, 4, 16, QLatin1Char('0')));
    }
    else
    {
        snprintf(vlanAddr, size, "%s", "NA");
    }
}

// read Ntp Server IP MODE ======================================================
int readNtpServerIpMode(char *ipMode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

    temp_data = 0x00000000;
    snprintf(ipMode, size, "%s", "err");
    // mode & server config
    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
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
    }
    else
    {
        snprintf(ipMode, size, "%s", "NA");
        return -1;
    }

    return 0;
}

// read Ntp Server Unicast mode ======================================================
int readNtpServerUnicastMode(char *mode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    snprintf(mode, size, "%s", "err");
    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        if (((temp_data) & 0x00000010) == 0)
        {
            snprintf(mode, size, "%s", "disabled");
        }
        else
        {
            snprintf(mode, size, "%s", "enabled");
        }
    }
    else
    {
        snprintf(mode, size, "%s", "disabled");
        return -1;
    }
    return 0;
}

// read Ntp Server Multicast mode ======================================================
int readNtpServerMulticastMode(char *mode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    snprintf(mode, size, "%s", "err");
    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        if (((temp_data) & 0x00000020) == 0)
        {
            snprintf(mode, size, "%s", "disabled");
        }
        else
        {
            snprintf(mode, size, "%s", "enabled");
        }
    }
    else
    {
        snprintf(mode, size, "%s", "disabled");
        return -1;
    }
    return 0;
}

// read Ntp Server Broadcast mode ======================================================
int readNtpServerBroadcastMode(char *mode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    snprintf(mode, size, "%s", "err");
    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        if (((temp_data) & 0x00000040) == 0)
        {
            snprintf(mode, size, "%s", "disabled");
        }
        else
        {
            snprintf(mode, size, "%s", "enabled");
        }
    }
    else
    {
        snprintf(mode, size, "%s", "disabled");
        return -1;
    }
    return 0;
}

// read Ntp Server Precision mode ======================================================
int readNtpServerPrecisionValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    snprintf(value, size, "%s", "err");
    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        // ui->NtpServerPrecisionValue->setText(QString::number((char)((temp_data >> 8) & 0x000000FF)));
        snprintf(value, size, "%d", (char)((temp_data >> 8) & 0x000000FF));
    }
    else
    {
        snprintf(value, size, "%s", "NA");
        return -1;
    }
    return 0;
}
// read Ntp Server PollIntervalValue mode ======================================================
int readNtpServerPollIntervalValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    snprintf(value, size, "%s", "err");
    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        // ui->NtpServerPrecisionValue->setText(QString::number((char)((temp_data >> 8) & 0x000000FF)));
        snprintf(value, size, "%ld", ((temp_data >> 16) & 0x000000FF));
    }
    else
    {
        snprintf(value, size, "%s", "NA");
        return -1;
    }
    return 0;
}

// read Ntp Server Stratum value mode ======================================================
int readNtpServerStratumValue(char *value, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    snprintf(value, size, "%s", "err");
    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        // ui->NtpServerPrecisionValue->setText(QString::number((char)((temp_data >> 8) & 0x000000FF)));
        snprintf(value, size, "%ld", ((temp_data >> 24) & 0x000000FF));
    }
    else
    {
        snprintf(value, size, "%s", "NA");
        return -1;
    }
    return 0;
}

int readNtpServerReferenceId(char *refId, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data = 0x00000000;
    snprintf(refId, size, "%s", "err");

    char temp_refid[4] = {0};

    // reference id
    // temp_string.clear();
    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigReferenceIdReg, &temp_data))
    {
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
    }
    else
    {
        snprintf(refId, size, "%s", "NA");
        return 0;
        // ui->NtpServerReferenceIdValue->setText("NA");
    }

    return -1;
}

// read NtpServer IP ADDRESS ======================================================
int readNtpServerIpAddress(char *ipAddr, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    int64_t temp_ip = 0;

    char ipMode[size];
    snprintf(ipAddr, size, "%s", "err");

    readNtpServerIpMode(ipMode, size);

    // temp_string = ui->NtpServerIpModeValue->currentText();

    if (0 == strncmp(ipMode, "IPv4", size))
    {
        // temp_string.clear();
        if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigIpReg, &temp_data))
        {
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

            // temp_string = QHostAddress(temp_ip).toString();

            snprintf(ipAddr, size, "%d.%d.%d.%d", ip_bytes[3], ip_bytes[2], ip_bytes[1], ip_bytes[0]);

            // printf("ip addr: %s ", ipAddr);

            // ui->NtpServerIpValue->setText(temp_string);
        }
        else
        {
            // ui->NtpServerIpValue->setText("NA");
        }
    }
    else if (0 == strncmp(ipMode, "IPv6", size))
    {
        unsigned char temp_ip6[16];
        // temp_string.clear();
        if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigIpReg, &temp_data))
        {
            temp_ip6[0] = (temp_data >> 0) & 0x000000FF;
            temp_ip6[1] = (temp_data >> 8) & 0x000000FF;
            temp_ip6[2] = (temp_data >> 16) & 0x000000FF;
            temp_ip6[3] = (temp_data >> 24) & 0x000000FF;

            if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigIpv61Reg, &temp_data))
            {
                temp_ip6[4] = (temp_data >> 0) & 0x000000FF;
                temp_ip6[5] = (temp_data >> 8) & 0x000000FF;
                temp_ip6[6] = (temp_data >> 16) & 0x000000FF;
                temp_ip6[7] = (temp_data >> 24) & 0x000000FF;

                if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigIpv62Reg, &temp_data))
                {
                    temp_ip6[8] = (temp_data >> 0) & 0x000000FF;
                    temp_ip6[9] = (temp_data >> 8) & 0x000000FF;
                    temp_ip6[10] = (temp_data >> 16) & 0x000000FF;
                    temp_ip6[11] = (temp_data >> 24) & 0x000000FF;

                    if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigIpv63Reg, &temp_data))
                    {
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
                        // printf("ip addr: %s ", ipAddr);

                        // this is ugly like your mom
                        // temp_string = QHostAddress(temp_ip6).toString();

                        // ui->NtpServerIpValue->setText(temp_string);
                    }
                    else
                    {
                        // ui->NtpServerIpValue->setText("NA");
                        snprintf(ipAddr, size, "%s", "NA");
                    }
                }
                else
                {
                    snprintf(ipAddr, size, "%s", "NA");

                    // ui->NtpServerIpValue->setText("NA");
                }
            }
            else
            {
                snprintf(ipAddr, size, "%s", "NA");

                // ui->NtpServerIpValue->setText("NA");
            }
        }
        else
        {
            snprintf(ipAddr, size, "%s", "NA");

            // ui->NtpServerIpValue->setText("NA");
        }
    }
    else
    {
        snprintf(ipAddr, size, "%s", "NA");

        // ui->NtpServerIpValue->setText("NA");
    }
}

int readNtpServerSmearingStatus(char *status, size_t size)
{
    // utc info
    snprintf(status, size, "%s", "err");

    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    // utc info
    temp_data = 0x40000000;
    if (0 == writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {

        for (int i = 0; i < 10; i++)
        {
            if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
            {

                if ((temp_data & 0x80000000) != 0)
                {
                    if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
                    {

                        if ((temp_data & 0x00000100) == 0)
                        {
                            snprintf(status, size, "%s", "disabled");
                            // snprintf(status, size, "%lx", temp_data);
                        }
                        else
                        {
                            snprintf(status, size, "%s", "enabled");
                        }
                    }
                    else
                    {
                        snprintf(status, size, "%s", "disabled");
                    }
                    break;
                }
                else if (i == 9)
                {
                    snprintf(status, size, "%s", "err: read did not complete");
                    return -1;
                }
            }
            else
            {
                snprintf(status, size, "%s", "disabled");
            }
        }
    }
    else
    {
        snprintf(status, size, "%s", "disabled");
    }

    return 0;
}

int readNtpServerLeap61Progress(char *progress, size_t size)
{
    // utc info
    snprintf(progress, size, "%s", "err");

    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    // utc info
    temp_data = 0x40000000;
    if (0 == writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {

        for (int i = 0; i < 10; i++)
        {
            if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
            {

                if ((temp_data & 0x80000000) != 0)
                {
                    if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
                    {

                        if ((temp_data & 0x00000200) == 0)
                        {
                            snprintf(progress, size, "%s", "not progressing");
                        }
                        else
                        {
                            snprintf(progress, size, "%s", "in progress");
                        }
                    }
                    else
                    {
                        snprintf(progress, size, "%s", "not progressing");
                    }
                    break;
                }
                else if (i == 9)
                {
                    snprintf(progress, size, "%s", "err: read did not complete");
                    return -1;
                }
            }
            else
            {
                snprintf(progress, size, "%s", "not progressing");
            }
        }
    }
    else
    {
        snprintf(progress, size, "%s", "not progressing");
    }

    return 0;
}
int readNtpServerLeap59Progress(char *progress, size_t size)
{
    // utc info
    snprintf(progress, size, "%s", "err");

    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    // utc info
    temp_data = 0x40000000;
    if (0 == writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {

        for (int i = 0; i < 10; i++)
        {
            if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
            {

                if ((temp_data & 0x80000000) != 0)
                {
                    if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
                    {

                        if ((temp_data & 0x00000400) == 0)
                        {
                            snprintf(progress, size, "%s", "not progressing");
                        }
                        else
                        {
                            snprintf(progress, size, "%s", "in progress");
                        }
                    }
                    else
                    {
                        snprintf(progress, size, "%s", "not progressing");
                    }
                    break;
                }
                else if (i == 9)
                {
                    snprintf(progress, size, "%s", "err: read did not complete");
                    return -1;
                }
            }
            else
            {
                snprintf(progress, size, "%s", "not progressing");
            }
        }
    }
    else
    {
        snprintf(progress, size, "%s", "not progressing");
    }

    return 0;
}
int readNtpServerLeap61Status(char *status, size_t size) // leap 61 0x00000800
{
    // utc info
    snprintf(status, size, "%s", "err");

    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    // utc info
    temp_data = 0x40000000;
    if (0 == writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {

        for (int i = 0; i < 10; i++)
        {
            if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
            {

                if ((temp_data & 0x80000000) != 0)
                {
                    if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
                    {

                        if ((temp_data & 0x00000800) == 0)
                        {
                            snprintf(status, size, "%s", "disabled");
                        }
                        else
                        {
                            snprintf(status, size, "%s", "enabled");
                        }
                    }
                    else
                    {
                        snprintf(status, size, "%s", "disabled");
                    }
                    break;
                }
                else if (i == 9)
                {
                    snprintf(status, size, "%s", "err: read did not complete");
                    return -1;
                }
            }
            else
            {
                snprintf(status, size, "%s", "disabled");
            }
        }
    }
    else
    {
        snprintf(status, size, "%s", "disabled");
    }

    return 0;
}
int readNtpServerLeap59Status(char *status, size_t size)
{
    // utc info
    snprintf(status, size, "%s", "err");

    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    // utc info
    temp_data = 0x40000000;
    if (0 == writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {

        for (int i = 0; i < 10; i++)
        {
            if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
            {

                if ((temp_data & 0x80000000) != 0)
                {
                    if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
                    {

                        if ((temp_data & 0x00001000) == 0)
                        {
                            snprintf(status, size, "%s", "disabled");
                        }
                        else
                        {
                            snprintf(status, size, "%s", "enabled");
                        }
                    }
                    else
                    {
                        snprintf(status, size, "%s", "disabled");
                    }
                    break;
                }
                else if (i == 9)
                {
                    snprintf(status, size, "%s", "err: read did not complete");
                    return -1;
                }
            }
            else
            {
                snprintf(status, size, "%s", "disabled");
            }
        }
    }
    else
    {
        snprintf(status, size, "%s", "disabled");
    }

    return 0;
}
int readNtpServerUtcOffsetStatus(char *status, size_t size)
{
    // utc info
    snprintf(status, size, "%s", "err");

    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    // utc info
    temp_data = 0x40000000;
    if (0 == writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        for (int i = 0; i < 10; i++)
        {
            if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
            {
                if ((temp_data & 0x80000000) != 0)
                {
                    if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
                    {
                        if ((temp_data & 0x00002000) == 0)
                        {
                            snprintf(status, size, "%s", "disabled");
                        }
                        else
                        {
                            snprintf(status, size, "%s", "enabled");
                        }
                    }
                    else
                    {
                        snprintf(status, size, "%s", "disabled");
                    }
                    break;
                }
                else if (i == 9)
                {
                    snprintf(status, size, "%s", "err: read did not complete");
                    return -1;
                }
            }
            else
            {
                snprintf(status, size, "%s", "disabled");
            }
        }
    }
    else
    {
        snprintf(status, size, "%s", "disabled");
    }

    return 0;
}
int readNtpServerUtcOffsetValue(char *value, size_t size)
{
    // utc info
    snprintf(value, size, "%s", "err");

    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    // utc info
    temp_data = 0x40000000;
    if (0 == writeRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
    {
        for (int i = 0; i < 10; i++)
        {
            if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoControlReg, &temp_data))
            {
                if ((temp_data & 0x80000000) != 0)
                {
                    if (0 == readRegister(temp_addr + Ucm_NtpServer_UtcInfoReg, &temp_data))
                    {
                        snprintf(value, size, "%ld", ((temp_data >> 16) & 0x0000FFFF));
                    }
                    else
                    {
                        snprintf(value, size, "%s", "NA");
                    }
                    break;
                }
                else if (i == 9)
                {
                    snprintf(value, size, "%s", "err: read did not complete");
                    return -1;
                }
            }
            else
            {
                snprintf(value, size, "%s", "NA");
            }
        }
    }
    else
    {
        snprintf(value, size, "%s", "NA");
    }

    return 0;
}

int readNtpServerRequestsValue(char *value, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    snprintf(value, size, "%s", "err");
    if (0 == readRegister(temp_addr + Ucm_NtpServer_CountReqReg, &temp_data))
    {
        // ui->NtpServerRequestsValue->setText(QString::number(temp_data));
        snprintf(value, size, "%ld", temp_data);
    }
    else
    {
        // ui->NtpServerRequestsValue->setText("NA");
        snprintf(value, size, "%s", "NA");
    }

    return 0;
}
int readNtpServerResponsesValue(char *value, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    snprintf(value, size, "%s", "err");
    if (0 == readRegister(temp_addr + Ucm_NtpServer_CountRespReg, &temp_data))
    {
        // ui->NtpServerRequestsValue->setText(QString::number(temp_data));
        snprintf(value, size, "%ld", temp_data);
    }
    else
    {
        // ui->NtpServerRequestsValue->setText("NA");
        snprintf(value, size, "%s", "NA");
    }

    return 0;
}
int readNtpServerRequestsDroppedValue(char *value, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    snprintf(value, size, "%s", "err");
    if (0 == readRegister(temp_addr + Ucm_NtpServer_CountReqDroppedReg, &temp_data))
    {
        // ui->NtpServerRequestsValue->setText(QString::number(temp_data));
        snprintf(value, size, "%ld", temp_data);
    }
    else
    {
        // ui->NtpServerRequestsValue->setText("NA");
        snprintf(value, size, "%s", "NA");
    }

    return 0;
}
int readNtpServerBroadcastsValue(char *value, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    snprintf(value, size, "%s", "err");
    if (0 == readRegister(temp_addr + Ucm_NtpServer_CountBroadcastReg, &temp_data))
    {
        // ui->NtpServerRequestsValue->setText(QString::number(temp_data));
        snprintf(value, size, "%ld", temp_data);
    }
    else
    {
        // ui->NtpServerRequestsValue->setText("NA");
        snprintf(value, size, "%s", "NA");
    }

    return 0;
}
int readNtpServerClearCountersStatus(char *status, size_t size)
{
    temp_data = 0x00000000;
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    if (0 == readRegister(temp_addr + Ucm_NtpServer_CountControlReg, &temp_data))
    {
        if ((temp_data & 0x00000001) == 0)
        {
            snprintf(status, size, "%s", "disable");
        }
        else
        {
            snprintf(status, size, "%s", "enable");
        }
    }
    else
    {
        snprintf(status, size, "%s", "disable");
    }
    return 0;
}
int readNtpServerVersion(char *value, size_t size)
{
    // version
    if (0 == readRegister(temp_addr + Ucm_NtpServer_VersionReg, &temp_data))
    {
        // ui->NtpServerVersionValue->setText(QString("0x%1").arg(temp_data, 8, 16, QLatin1Char('0')));
        snprintf(value, size, "0x%lx", temp_data);
    }
    else
    {
        snprintf(value, size, "%s", "NA");
    }

    return 0;
}

// read Ntp Server Instance Number ======================================================
int readNtpServerInstanceNumber(char *instanceNumber, size_t size)
{
    readConfig();

    snprintf(instanceNumber, size, "%ld", cores[Ucm_CoreConfig_NtpServerCoreType].core_instance_nr);
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

    if (0 == writeRegister(temp_addr + Ucm_NtpServer_ConfigMac1Reg, &temp_data))
    {

        temp_data = 0x00000000;
        temp_data |= (temp_mac >> 0) & 0x000000FF;
        temp_data = temp_data << 8;
        temp_data |= (temp_mac >> 8) & 0x000000FF;

        if (0 == writeRegister(temp_addr + Ucm_NtpServer_ConfigMac2Reg, &temp_data))
        {

            temp_data = 0x00000004; // write
            if (0 == writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
            {
                // write success
                return 0;
            }
            else
            {
                // ui->NtpServerMacValue->setText("NA");
                return -1;
            }
        }
        else
        {
            // ui->NtpServerMacValue->setText("NA");
            return -1;
        }
    }
    else
    {
        // ui->NtpServerMacValue->setText("NA");
        return -1;
    }
}

int writeNtpServerVlanStatus(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    temp_data &= 0x0000FFFF;
    long currentSettings = 0;
    char *currentStatus;

    if (0 != readRegister(temp_addr, &currentSettings))
    {
        return -1;
    }
    currentSettings &= 0x0000FFFF;

    temp_data = 0x00000000 | currentSettings;

    if (0 == strncmp(status, "enabled", size))
    {
        temp_data = 0x00010000 | currentSettings;
    }

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigVlanReg, &temp_data))
    {
        return -1;
    }

    temp_data = 0x00000002;
    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1;
    }

    return 0;
}
int writeNtpServerVlanAddress(char *value, size_t size)
{
    // readConfig();
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
        return -1; // read current settings fails
    }

    temp_vlan = strtol(value, NULL, 16);

    temp_data &= 0xFFFF0000;

    temp_data |= temp_vlan;

    if (0 == writeRegister(temp_addr + Ucm_NtpServer_ConfigVlanReg, &temp_data))
    {
        return -1;
    }

    temp_data = 0x00000002;

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1; // failed to write control reg
    }

    return 0;
}

int writeNtpServerUnicastMode(char *mode, size_t size)
{
    temp_addr = temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

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

    if (0 == writeRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
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
    temp_addr = temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

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
    temp_addr = temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

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
    temp_addr = temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

    temp_data = 0x00000000;

    long temp_precision = 0;

    if (0 != readRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        return -1;
    }

    temp_precision = strtol(value, NULL, 16);
    temp_data &= ~((0xFFFFFFFF & 0x000000FF) << 8);
    temp_data |= ((temp_precision & 0x000000FF) << 8);

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigModeReg, &temp_data))
    {
        temp_data = 0x00000001;
    }

    if (0 != writeRegister(temp_addr + Ucm_NtpServer_ConfigControlReg, &temp_data))
    {
        return -1;
    }
}

int writeNtpServerStratumValue(char *value, size_t size)
{
}
int writeNtpServerReferenceIdValue(char *value, size_t size)
{
}
int writeNtpServerUtcSmearingStatus(char *status, size_t size)
{
}
int writeNtpServerLeap61Status(char *status, size_t size)
{
}
int writeNtpServerLeap59Status(char *status, size_t size)
{
}
int writeNtpServerUtcOffsetStatus(char *status, size_t size)
{
}
int writeNtpServerUtcOffsetValue(char *value, size_t size)
{
}
int writeNtpServerClearCountersStatus(char *value, size_t size)
{
}

//
int writeStatus(char *status, size_t size)
{
    readConfig();
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