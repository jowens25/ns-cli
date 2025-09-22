
#include "axi.h"
#include "cores.h"
#include "todSlave.h"

char *TodSlaveProperties[] = {
    [TodSlaveVersion] = "version",
    [TodSlaveInstance] = "instance",
    [TodSlaveProtocol] = "protocol",
    [TodSlaveGnss] = "gnss",
    [TodSlaveMsgDisable] = "msg_disable",
    [TodSlaveCorrection] = "correction",
    [TodSlaveBaudRate] = "baud_rate",
    [TodSlaveInvertedPolarity] = "inverted_polarity",
    [TodSlaveUtcOffset] = "utc_offset",
    [TodSlaveUtcInfoValid] = "utc_info_valid",
    [TodSlaveLeapAnnounce] = "leap_announce",
    [TodSlaveLeap59] = "leap59",
    [TodSlaveLeap61] = "leap61",
    [TodSlaveLeapInfoValid] = "leap_info_valid",
    [TodSlaveTimeToLeap] = "time_to_leap",
    [TodSlaveGnssFix] = "gnss_fix",
    [TodSlaveGnssFixOk] = "gnss_fix_ok",
    [TodSlaveSpoofingState] = "spoofing_state",
    [TodSlaveFixAndSpoofingInfoValid] = "fix_and_spoofing_info_valid",
    [TodSlaveJammingLevel] = "jamming_level",
    [TodSlaveJammingState] = "jamming_state",
    [TodSlaveAntennaState] = "antenna_state",
    [TodSlaveAntennaAndJammingInfoValid] = "antenna_and_jamming_info_valid",
    [TodSlaveNrOfSatellitesSeen] = "nr_of_satellites_seen",
    [TodSlaveNrOfSatellitesLocked] = "nr_of_satellites_locked",
    [TodSlaveNrOfSatellitesInfo] = "nr_of_satellites_info",
    [TodSlaveEnable] = "enable",
    [TodSlaveInputOk] = "input_ok",
    [28] = "NULL",
};

int hasTodSlave(char *in, size_t size)
{
    if (Ucm_CoreConfig_TodSlaveCoreType != cores[Ucm_CoreConfig_TodSlaveCoreType].core_type)
    {

        return -1;
    }

    return 0;
}

int readTodSlaveVersion(char *version, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_VersionReg, &temp_data))
    {
        snprintf(version, size, "%s", "NA");
        return -1;
    }
    snprintf(version, size, "0x%lx", temp_data);
    return 0;
}

int readTodSlaveInstance(char *instanceNumber, size_t size)
{
    snprintf(instanceNumber, size, "%ld", cores[Ucm_CoreConfig_TodSlaveCoreType].core_instance_nr);
    return 0;
}

int readTodSlaveEnable(char *status, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    // enabled
    if (0 != readRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_data))
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

int readTodSlaveProtocol(char *protocol, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_data))
    {

        snprintf(protocol, size, "%s", "NA");
        return -1;
    }

    // protocol
    switch ((temp_data >> 28) & 0x3)
    {
    case 0:
        snprintf(protocol, size, "%s", "NMEA");
        break;
    case 1:
        snprintf(protocol, size, "%s", "UBX");
        break;
    case 2:
        snprintf(protocol, size, "%s", "TSIP");
        break;
    default:
        snprintf(protocol, size, "%s", "NA");
        break;
    }

    return 0;
}

int readTodSlaveGnss(char *gnss, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_data))
    {

        snprintf(gnss, size, "%s", "NA");
        return -1;
    }

    // gnss
    switch ((temp_data >> 24) & 0xF)
    {
    case 0:
        snprintf(gnss, size, "%s", "ALL");
        break;
    case 1:
        snprintf(gnss, size, "%s", "COMBINED");
        break;
    case 2:
        snprintf(gnss, size, "%s", "GPS");
        break;
    case 3:
        snprintf(gnss, size, "%s", "GLONASS");
        break;
    case 4:
        snprintf(gnss, size, "%s", "GALILEO");
        break;
    case 5:
        snprintf(gnss, size, "%s", "BEIDOU");
        break;
    default:
        snprintf(gnss, size, "%s", "NA");
        break;
    }

    return 0;
}

int readTodSlaveMsgDisable(char *msgdisable, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_data))
    {
        snprintf(msgdisable, size, "%s", "NA");
        return -1;
    }

    // disabled msg
    snprintf(msgdisable, size, "0x%02lx", ((temp_data >> 16) & 0xFF));

    return 0;
}

int readTodSlaveCorrection(char *correction, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_CorrectionReg, &temp_data))
    {
        snprintf(correction, size, "%s", "NA");
        return -1;
    }

    // disabled msg
    snprintf(correction, size, "0x%08lx", temp_data);

    return 0;
}
int readTodSlaveBaudRate(char *baudrate, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_UartBaudRateReg, &temp_data))
    {
        snprintf(baudrate, size, "%s", "NA");
        return -1;
    }

    switch (temp_data)
    {
    case 0:
        snprintf(baudrate, size, "%s", "1200");
        break;
    case 1:
        snprintf(baudrate, size, "%s", "2400");
        break;
    case 2:
        snprintf(baudrate, size, "%s", "4800");
        break;
    case 3:
        snprintf(baudrate, size, "%s", "9600");
        break;
    case 4:
        snprintf(baudrate, size, "%s", "19200");
        break;
    case 5:
        snprintf(baudrate, size, "%s", "38400");
        break;
    case 6:
        snprintf(baudrate, size, "%s", "57600");
        break;
    case 7:
        snprintf(baudrate, size, "%s", "115200");
        break;
    case 8:
        snprintf(baudrate, size, "%s", "230400");
        break;
    case 9:
        snprintf(baudrate, size, "%s", "460800");
        break;
    case 10:
        snprintf(baudrate, size, "%s", "921600");
        break;
    case 11:
        snprintf(baudrate, size, "%s", "1000000");
        break;
    case 12:
        snprintf(baudrate, size, "%s", "2000000");
        break;
    default:
        snprintf(baudrate, size, "%s", "NA");
        break;
    }

    return 0;
}

int readTodSlaveInvertedPolarity(char *invertedpolarity, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    // invertedpolarity
    if (0 != readRegister(temp_addr + Ucm_TodSlave_PolarityReg, &temp_data))
    {
        snprintf(invertedpolarity, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x00000001) == 0)
    {
        snprintf(invertedpolarity, size, "%s", "enabled");
    }

    else
    {
        snprintf(invertedpolarity, size, "%s", "disabled");
    }

    return 0;
}

int readTodSlaveInputOk(char *inputok, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    // input ok
    if (0 != readRegister(temp_addr + Ucm_TodSlave_StatusReg, &temp_data))
    {
        snprintf(inputok, size, "%s", "disabled");

        return -1;
    }

    if (temp_data == 0)
    {
        snprintf(inputok, size, "%s", "enabled");
    }

    else
    {
        snprintf(inputok, size, "%s", "disabled");
    }

    // clear after read
    writeRegister(temp_addr + Ucm_TodSlave_StatusReg, &temp_data);
    // writeTodSlaveInputOk(temp_addr + Ucm_TodSlave_StatusReg, &temp_data);

    return 0;
}

int readTodSlaveUtcOffset(char *utcoffset, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    // input ok
    if (0 != readRegister(temp_addr + Ucm_TodSlave_UtcStatusReg, &temp_data))
    {
        snprintf(utcoffset, size, "%s", "disabled");

        return -1;
    }

    snprintf(utcoffset, size, "%d", (signed char)(temp_data & 0xFF));

    return 0;
}
int readTodSlaveUtcInfoValid(char *utcinfovalid, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_UtcStatusReg, &temp_data))
    {
        snprintf(utcinfovalid, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x00000100) == 0)
    {
        snprintf(utcinfovalid, size, "%s", "disabled");
    }

    else
    {
        snprintf(utcinfovalid, size, "%s", "enabled");
    }

    return 0;
}
int readTodSlaveLeapAnnounce(char *leapannounce, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_UtcStatusReg, &temp_data))
    {
        snprintf(leapannounce, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x00001000) == 0)
    {
        snprintf(leapannounce, size, "%s", "disabled");
    }

    else
    {
        snprintf(leapannounce, size, "%s", "enabled");
    }

    return 0;
}
int readTodSlaveLeap59(char *leap59, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_UtcStatusReg, &temp_data))
    {
        snprintf(leap59, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x00002000) == 0)
    {
        snprintf(leap59, size, "%s", "disabled");
    }

    else
    {
        snprintf(leap59, size, "%s", "enabled");
    }

    return 0;
}
int readTodSlaveLeap61(char *leap61, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_UtcStatusReg, &temp_data))
    {
        snprintf(leap61, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x00004000) == 0)
    {
        snprintf(leap61, size, "%s", "disabled");
    }

    else
    {
        snprintf(leap61, size, "%s", "enabled");
    }

    return 0;
}
int readTodSlaveLeapInfoValid(char *leapinfovalid, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_UtcStatusReg, &temp_data))
    {
        snprintf(leapinfovalid, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x00010000) == 0)
    {
        snprintf(leapinfovalid, size, "%s", "disabled");
    }

    else
    {
        snprintf(leapinfovalid, size, "%s", "enabled");
    }

    return 0;
}
int readTodSlaveTimeToLeap(char *timetoleap, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    // input ok
    if (0 != readRegister(temp_addr + Ucm_TodSlave_TimeToLeapSecondReg, &temp_data))
    {
        snprintf(timetoleap, size, "%s", "NA");

        return -1;
    }

    snprintf(timetoleap, size, "%ld", temp_data);

    return 0;
}

int readTodSlaveGnssFix(char *gnssfix, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_GnssStatus_Reg_Con, &temp_data))
    {
        snprintf(gnssfix, size, "%s", "NA");
        return -1;
    }

    // gnss fix
    switch ((temp_data >> 17) & 0xFF)
    {
    case 0:
        snprintf(gnssfix, size, "%s", "NO FIX");
        break;
    case 1:
        snprintf(gnssfix, size, "%s", "DEAD RECKONING");
        break;
    case 2:
        snprintf(gnssfix, size, "%s", "2D FIX");
        break;
    case 3:
        snprintf(gnssfix, size, "%s", "3D FIX");
        break;
    case 4:
        snprintf(gnssfix, size, "%s", "GNSS & DEAD RECKONING");
        break;
    default:
        snprintf(gnssfix, size, "%s", "NA");
        break;
    }

    return 0;
}
int readTodSlaveGnssFixOk(char *gnssfixok, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_GnssStatus_Reg_Con, &temp_data))
    {
        snprintf(gnssfixok, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x00010000) == 0)
    {
        snprintf(gnssfixok, size, "%s", "disabled");
    }

    else
    {
        snprintf(gnssfixok, size, "%s", "enabled");
    }

    return 0;
}
int readTodSlaveSpoofingState(char *spoofingstate, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_GnssStatus_Reg_Con, &temp_data))
    {
        snprintf(spoofingstate, size, "%s", "NA");
        return -1;
    }

    // spoofing state
    switch ((temp_data >> 25) & 0x3)
    {
    case 0:
        snprintf(spoofingstate, size, "%s", "UNKNOWN");
        break;
    case 1:
        snprintf(spoofingstate, size, "%s", "NO SPOOFING");
        break;
    case 2:
        snprintf(spoofingstate, size, "%s", "SPOOFING");
        break;
    default:
        snprintf(spoofingstate, size, "%s", "NA");
        break;
    }

    return 0;
}
int readTodSlaveFixAndSpoofingInfoValid(char *fixandspoofinginfovalid, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_GnssStatus_Reg_Con, &temp_data))
    {
        snprintf(fixandspoofinginfovalid, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x10000000) == 0)
    {
        snprintf(fixandspoofinginfovalid, size, "%s", "disabled");
    }

    else
    {
        snprintf(fixandspoofinginfovalid, size, "%s", "enabled");
    }

    return 0;
}
int readTodSlaveJammingLevel(char *jamminglevel, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    // input ok
    if (0 != readRegister(temp_addr + Ucm_TodSlave_GnssStatus_Reg_Con, &temp_data))
    {
        snprintf(jamminglevel, size, "%s", "NA");

        return -1;
    }

    snprintf(jamminglevel, size, "%ld", ((temp_data >> 5) & 0xFF));

    return 0;
}

int readTodSlaveJammingState(char *jammingstate, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_GnssStatus_Reg_Con, &temp_data))
    {
        snprintf(jammingstate, size, "%s", "NA");
        return -1;
    }

    switch ((temp_data >> 3) & 0x3)
    {
    case 0:
        snprintf(jammingstate, size, "%s", "UNKNOWN");
        break;
    case 1:
        snprintf(jammingstate, size, "%s", "NO JAMMING");
        break;
    case 2:
        snprintf(jammingstate, size, "%s", "JAMMING LEVEL WARNING");
        break;
    case 3:
        snprintf(jammingstate, size, "%s", "JAMMING LEVEL CRITICAL");
        break;
    default:
        snprintf(jammingstate, size, "%s", "NA");
        break;
    }

    return 0;
}

int readTodSlaveAntennaState(char *antennastate, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_GnssStatus_Reg_Con, &temp_data))
    {
        snprintf(antennastate, size, "%s", "NA");
        return -1;
    }

    // antenna state
    switch ((temp_data >> 0) & 0x7)
    {
    case 0:
        snprintf(antennastate, size, "%s", "INIT");
        break;
    case 1:
        snprintf(antennastate, size, "%s", "DONT KNOW");
        break;
    case 2:
        snprintf(antennastate, size, "%s", "OK");
        break;
    case 3:
        snprintf(antennastate, size, "%s", "SHORT");
        break;
    case 4:
        snprintf(antennastate, size, "%s", "OPEN");
        break;
    default:
        snprintf(antennastate, size, "%s", "NA");
        break;
    }

    return 0;
}
int readTodSlaveAntennaAndJammingInfoValid(char *antennaandjamminginfovalid, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_GnssStatus_Reg_Con, &temp_data))
    {
        snprintf(antennaandjamminginfovalid, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x20000000) == 0)
    {
        snprintf(antennaandjamminginfovalid, size, "%s", "disabled");
    }

    else
    {
        snprintf(antennaandjamminginfovalid, size, "%s", "enabled");
    }

    return 0;
}
int readTodSlaveNrOfSatellitesSeen(char *nrofsatellitesseen, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    // input ok
    if (0 != readRegister(temp_addr + Ucm_TodSlave_SatelliteNumber_Reg_Con, &temp_data))
    {
        snprintf(nrofsatellitesseen, size, "%s", "NA");

        return -1;
    }

    snprintf(nrofsatellitesseen, size, "%ld", ((temp_data >> 0) & 0xFF));

    return 0;
}
int readTodSlaveNrOfSatellitesLocked(char *nrofsatelliteslocked, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    // input ok
    if (0 != readRegister(temp_addr + Ucm_TodSlave_SatelliteNumber_Reg_Con, &temp_data))
    {
        snprintf(nrofsatelliteslocked, size, "%s", "NA");

        return -1;
    }

    snprintf(nrofsatelliteslocked, size, "%ld", ((temp_data >> 8) & 0xFF));

    return 0;
}
int readTodSlaveNrOfSatellitesInfo(char *nrofsatellitesinfo, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_SatelliteNumber_Reg_Con, &temp_data))
    {
        snprintf(nrofsatellitesinfo, size, "%s", "disabled");

        return -1;
    }

    if ((temp_data & 0x00010000) == 0)
    {
        snprintf(nrofsatellitesinfo, size, "%s", "disabled");
    }

    else
    {
        snprintf(nrofsatellitesinfo, size, "%s", "enabled");
    }

    return 0;
}

// ===========================================================================================
// ===========================================================================================
// ===========================================================================================
// ===========================================================================================
// ===========================================================================================
// ===========================================================================================
// ===========================================================================================

int writeTodSlaveProtocol(char *protocol, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    int64_t temp_ctrl = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_ctrl))
    {
        return -1;
    }

    temp_data = temp_ctrl & ~0x30000000;

    if (0 == strncmp(protocol, "NMEA", size))
    {
        temp_data |= 0x00000000;
    }
    else if (0 == strncmp(protocol, "UBX", size))
    {
        temp_data |= 0x10000000;
    }
    else if (0 == strncmp(protocol, "TSIP", size))
    {
        temp_data |= 0x20000000;
    }
    else
    {
        temp_data = temp_ctrl;
    }

    if (0 != writeRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_data))
    {
        return -1;
    }

    return 0;
}

int writeTodSlaveGnss(char *gnss, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    int64_t temp_ctrl = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_ctrl))
    {
        return -1;
    }

    temp_data = temp_ctrl & ~0x0F000000;

    if (0 == strncmp(gnss, "ALL", size))
    {
        temp_data |= 0x00000000;
    }
    else if (0 == strncmp(gnss, "COMBINED", size))
    {
        temp_data |= 0x01000000;
    }
    else if (0 == strncmp(gnss, "GPS", size))
    {
        temp_data |= 0x02000000;
    }
    else if (0 == strncmp(gnss, "GLONASS", size))
    {
        temp_data |= 0x03000000;
    }
    else if (0 == strncmp(gnss, "GALILEO", size))
    {
        temp_data |= 0x04000000;
    }
    else if (0 == strncmp(gnss, "BEIDOU", size))
    {
        temp_data |= 0x05000000;
    }
    else
    {
        temp_data |= temp_ctrl & 0x0F000000;
    }

    if (0 != writeRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_data))
    {
        return -1;
    }

    return 0;
}

int writeTodSlaveMsgDisable(char *msgdisable, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    int64_t temp_ctrl = 0x00000000;

    if (0 != readRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_ctrl))
    {
        return -1;
    }

    temp_data = temp_ctrl & ~0x00FF0000;

    temp_data |= ((strtol(msgdisable, NULL, 16) & 0xFF) << 16);

    if (0 != writeRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_data))
    {
        return -1;
    }

    return 0;
}

int writeTodSlaveCorrection(char *correction, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;
    int64_t temp_correction = 0;
    correction = &correction[2];
    temp_correction = strtol(correction, NULL, 16);
    temp_data |= temp_correction;

    if (0 != writeRegister(temp_addr + Ucm_TodSlave_CorrectionReg, &temp_data))
    {
        return -1;
    }

    return 0;
}

int writeTodSlaveBaudRate(char *baudrate, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 == strncmp(baudrate, "1200", size))
    {
        temp_data = 0x00000000;
    }
    else if (0 == strncmp(baudrate, "2400", size))
    {
        temp_data = 0x0000001;
    }
    else if (0 == strncmp(baudrate, "4800", size))
    {
        temp_data = 0x00000002;
    }
    else if (0 == strncmp(baudrate, "9600", size))
    {
        temp_data = 0x00000003;
    }
    else if (0 == strncmp(baudrate, "19200", size))
    {
        temp_data = 0x00000004;
    }
    else if (0 == strncmp(baudrate, "38400", size))
    {
        temp_data = 0x00000005;
    }
    else if (0 == strncmp(baudrate, "57600", size))
    {
        temp_data = 0x00000006;
    }
    else if (0 == strncmp(baudrate, "115200", size))
    {
        temp_data = 0x00000007;
    }
    else if (0 == strncmp(baudrate, "230400", size))
    {
        temp_data = 0x00000008;
    }
    else if (0 == strncmp(baudrate, "460800", size))
    {
        temp_data = 0x00000009;
    }
    else if (0 == strncmp(baudrate, "921600", size))
    {
        temp_data = 0x000000A;
    }
    else if (0 == strncmp(baudrate, "1000000", size))
    {
        temp_data = 0x0000000B;
    }
    else if (0 == strncmp(baudrate, "2000000", size))
    {
        temp_data = 0x0000000C;
    }
    else
    {
        temp_data = 0x00000000;
    }

    if (0 != writeRegister(temp_addr + Ucm_TodSlave_UartBaudRateReg, &temp_data))
    {
        return -1;
    }

    return 0;
}

int writeTodSlaveInvertedPolarity(char *inverted, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    if (0 == strncmp(inverted, "disabled", size))
    {
        temp_data |= 0x00000001; // no inversion
    }

    if (0 != writeRegister(temp_addr + Ucm_TodSlave_PolarityReg, &temp_data))
    {
        return -1;
    }
    return 0;
}

int writeTodSlaveEnable(char *enable, size_t size)
{
    temp_addr = cores[Ucm_CoreConfig_TodSlaveCoreType].address_range_low;
    temp_data = 0x00000000;

    temp_data &= ~0x0000000F;

    if (0 == strncmp(enable, "enabled", size))
    {
        temp_data |= 0x00000001;
    }

    if (0 != writeRegister(temp_addr + Ucm_TodSlave_ControlReg, &temp_data))
    {
        return -1;
    }
    return 0;
}