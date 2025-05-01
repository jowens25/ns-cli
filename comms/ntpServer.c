
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
        // temp_string.append(QString("%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
        temp_mac[0] = ((temp_data >> 0) & 0x000000FF);
        temp_mac[1] = ((temp_data >> 8) & 0x000000FF);
        temp_mac[2] = ((temp_data >> 16) & 0x000000FF);
        temp_mac[3] = ((temp_data >> 24) & 0x000000FF);
        // temp_string.append(":");
        // temp_string.append(QString("%1").arg(((temp_data >> 8) & 0x000000FF), 2, 16, QLatin1Char('0')));
        // temp_string.append(":");
        // temp_string.append(QString("%1").arg(((temp_data >> 16) & 0x000000FF), 2, 16, QLatin1Char('0')));
        // temp_string.append(":");
        // temp_string.append(QString("%1").arg(((temp_data >> 24) & 0x000000FF), 2, 16, QLatin1Char('0')));
        // temp_string.append(":");
        if (0 == readRegister(temp_addr + Ucm_NtpServer_ConfigMac2Reg, &temp_data))
        {
            // temp_string.append(QString("%1").arg(((temp_data >> 0) & 0x000000FF), 2, 16, QLatin1Char('0')));
            // temp_string.append(":");
            // temp_string.append(QString("%1").arg(((temp_data >> 8) & 0x000000FF), 2, 16, QLatin1Char('0')));
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
        if ((temp_data & 0x00010000) == 0)
        {
            // ui->NtpServerVlanEnableCheckBox->setChecked(false);
            // snprintf(vlanStatus, "disabled", size);
        }
        else
        {
            // ui->NtpServerVlanEnableCheckBox->setChecked(true);
            // snprintf(vlanStatus, "enabled", size);
        }

        temp_data &= 0x0000FFFF;
        snprintf(vlanAddr, size, "%lx", temp_data);

        // ui->NtpServerVlanValue->setText(QString("0x%1").arg(temp_data, 4, 16, QLatin1Char('0')));
    }
    else
    {
        snprintf(vlanAddr, size, "%s", "NA");

        // ui->NtpServerVlanEnableCheckBox->setChecked(false);
        // ui->NtpServerVlanValue->setText("NA");
    }
}
// read Ntp Server Instance Number ======================================================
int readNtpServerInstanceNumber(char *instanceNumber, size_t size)
{
    readConfig();

    snprintf(instanceNumber, size, "%ld", cores[Ucm_CoreConfig_NtpServerCoreType].core_instance_nr);
}

// read Ntp Server IP MODE ======================================================
int readNtpServerIpMode(char *ipMode, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;

    temp_data = 0x00000000;
    snprintf(ipMode, size, "%s", "NA");
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

        // ipMode = "NA"; // IPv4 IPv6 NA

        return -1;
    }

    return 0;
}

// read NtpServer IP ADDRESS ======================================================
int readNtpServerIpAddress(char *ipAddr, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_NtpServerCoreType].address_range_low;
    int64_t temp_ip = 0;

    char ipMode[size];

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

//
//
//============================================ write it boi
//
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