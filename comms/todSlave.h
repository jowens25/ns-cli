#ifndef TOD_SLAVE_H

#define TOD_SLAVE_H

#define ControlReg 0x00000000
#define StatusReg 0x00000004
#define PolarityReg 0x00000008
#define VersionReg 0x0000000C
#define CorrectionReg 0x00000010
#define UartBaudRateReg 0x00000020
#define UtcStatusReg 0x00000030
#define TimeToLeapSecondReg 0x00000034
#define GnssStatus_Reg_Con 0x00000040
#define SatelliteNumber_Reg_Con 0x00000044

#endif // TOD_SLAVE_H