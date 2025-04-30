#ifndef NTP_SERVER_H

#define NTP_SERVER_H

#define Ucm_NtpServer_ControlReg 0x00000000
#define StatusReg 0x00000004
#define VersionReg 0x0000000C
#define CountControlReg 0x00000010
#define CountReqReg 0x00000014
#define CountRespReg 0x00000018
#define CountReqDroppedReg 0x0000001C
#define CountBroadcastReg 0x00000020
#define ConfigControlReg 0x00000080
#define ConfigModeReg 0x00000084
#define ConfigVlanReg 0x00000088
#define ConfigMac1Reg 0x0000008C
#define ConfigMac2Reg 0x00000090
#define ConfigIpReg 0x00000094
#define ConfigIpv61Reg 0x00000098
#define ConfigIpv62Reg 0x0000009C
#define ConfigIpv63Reg 0x000000A0
#define ConfigReferenceIdReg 0x000000A4
#define UtcInfoControlReg 0x00000100
#define UtcInfoReg 0x0000010

int readStatus(void);

#endif // NTP_SERVER_H