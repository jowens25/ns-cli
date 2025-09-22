#ifndef TOD_SLAVE_H

#define TOD_SLAVE_H

#include <stddef.h>
#include <stdint.h>
#define Ucm_TodSlave_ControlReg 0x00000000
#define Ucm_TodSlave_StatusReg 0x00000004
#define Ucm_TodSlave_PolarityReg 0x00000008
#define Ucm_TodSlave_VersionReg 0x0000000C
#define Ucm_TodSlave_CorrectionReg 0x00000010
#define Ucm_TodSlave_UartBaudRateReg 0x00000020
#define Ucm_TodSlave_UtcStatusReg 0x00000030
#define Ucm_TodSlave_TimeToLeapSecondReg 0x00000034
#define Ucm_TodSlave_GnssStatus_Reg_Con 0x00000040
#define Ucm_TodSlave_SatelliteNumber_Reg_Con 0x00000044

#define TodSlaveVersion 0
#define TodSlaveInstance 1
#define TodSlaveProtocol 2
#define TodSlaveGnss 3
#define TodSlaveMsgDisable 4
#define TodSlaveCorrection 5
#define TodSlaveBaudRate 6
#define TodSlaveInvertedPolarity 7
#define TodSlaveUtcOffset 8
#define TodSlaveUtcInfoValid 9
#define TodSlaveLeapAnnounce 10
#define TodSlaveLeap59 11
#define TodSlaveLeap61 12
#define TodSlaveLeapInfoValid 13
#define TodSlaveTimeToLeap 14
#define TodSlaveGnssFix 15
#define TodSlaveGnssFixOk 16
#define TodSlaveSpoofingState 17
#define TodSlaveFixAndSpoofingInfoValid 18
#define TodSlaveJammingLevel 19
#define TodSlaveJammingState 20
#define TodSlaveAntennaState 21
#define TodSlaveAntennaAndJammingInfoValid 22
#define TodSlaveNrOfSatellitesSeen 23
#define TodSlaveNrOfSatellitesLocked 24
#define TodSlaveNrOfSatellitesInfo 25
#define TodSlaveEnable 26
#define TodSlaveInputOk 27

extern char *TodSlaveProperties[];

int readTodSlaveVersion(char *version, size_t size);
int readTodSlaveInstance(char *instance, size_t size);
int readTodSlaveProtocol(char *protocol, size_t size);
int readTodSlaveGnss(char *gnss, size_t size);
int readTodSlaveMsgDisable(char *msgdisable, size_t size);
int readTodSlaveCorrection(char *correction, size_t size);
int readTodSlaveBaudRate(char *baudrate, size_t size);
int readTodSlaveInvertedPolarity(char *inverted, size_t size);
int readTodSlaveUtcOffset(char *utcoffset, size_t size);
int readTodSlaveUtcInfoValid(char *utcinfovalid, size_t size);
int readTodSlaveLeapAnnounce(char *leapannounce, size_t size);
int readTodSlaveLeap59(char *leap59, size_t size);
int readTodSlaveLeap61(char *leap61, size_t size);
int readTodSlaveLeapInfoValid(char *leapinfovalid, size_t size);
int readTodSlaveTimeToLeap(char *timetoleap, size_t size);
int readTodSlaveGnssFix(char *gnssfix, size_t size);
int readTodSlaveGnssFixOk(char *gnssfixok, size_t size);
int readTodSlaveSpoofingState(char *spoofingstate, size_t size);
int readTodSlaveFixAndSpoofingInfoValid(char *fixandspoofinginfovalid, size_t size);
int readTodSlaveJammingLevel(char *jamminglevel, size_t size);
int readTodSlaveJammingState(char *jammingstate, size_t size);
int readTodSlaveAntennaState(char *antennastate, size_t size);
int readTodSlaveAntennaAndJammingInfoValid(char *antennaandjamminginfovalid, size_t size);
int readTodSlaveNrOfSatellitesSeen(char *nrofsatellitesseen, size_t size);
int readTodSlaveNrOfSatellitesLocked(char *nrofsatelliteslocked, size_t size);
int readTodSlaveNrOfSatellitesInfo(char *nrofsatellitesinfo, size_t size);
int readTodSlaveEnable(char *enable, size_t size);
int readTodSlaveInputOk(char *inputok, size_t size);

int writeTodSlaveProtocol(char *protocol, size_t size);
int writeTodSlaveGnss(char *gnss, size_t size);
int writeTodSlaveMsgDisable(char *msgdisable, size_t size);
int writeTodSlaveCorrection(char *correction, size_t size);
int writeTodSlaveBaudRate(char *baudrate, size_t size);
int writeTodSlaveInvertedPolarity(char *inverted, size_t size);

int writeTodSlaveEnable(char *enable, size_t size);

int hasTodSlave(char *in, size_t size);

#endif // TOD_SLAVE_H