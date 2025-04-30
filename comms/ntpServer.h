#ifndef NTP_SERVER_H

#define NTP_SERVER_H

#define Ucm_NtpServer_ControlReg 0x00000000
#define Ucm_NtpServer_StatusReg 0x00000004
#define Ucm_NtpServer_VersionReg 0x0000000C
#define Ucm_NtpServer_CountControlReg 0x00000010
#define Ucm_NtpServer_CountReqReg 0x00000014
#define Ucm_NtpServer_CountRespReg 0x00000018
#define Ucm_NtpServer_CountReqDroppedReg 0x0000001C
#define Ucm_NtpServer_CountBroadcastReg 0x00000020
#define Ucm_NtpServer_ConfigControlReg 0x00000080
#define Ucm_NtpServer_ConfigModeReg 0x00000084
#define Ucm_NtpServer_ConfigVlanReg 0x00000088
#define Ucm_NtpServer_ConfigMac1Reg 0x0000008C
#define Ucm_NtpServer_ConfigMac2Reg 0x00000090
#define Ucm_NtpServer_ConfigIpReg 0x00000094
#define Ucm_NtpServer_ConfigIpv61Reg 0x00000098
#define Ucm_NtpServer_ConfigIpv62Reg 0x0000009C
#define Ucm_NtpServer_ConfigIpv63Reg 0x000000A0
#define Ucm_NtpServer_ConfigReferenceIdReg 0x000000A4
#define Ucm_NtpServer_UtcInfoControlReg 0x00000100
#define Ucm_NtpServer_UtcInfoReg 0x0000010

int readStatus(char *status);
int writeStatus(char *status);

#endif // NTP_SERVER_H