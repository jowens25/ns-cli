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
#define Ucm_NtpServer_UtcInfoReg 0x00000104

int readNtpServerStatus(char *status, size_t size);              // Ntp Server Status
int readNtpServerInstanceNumber(char *status, size_t size);      // Ntp Server InstanceNumber
int readNtpServerIpMode(char *ipMode, size_t size);              // Ntp Server IpMode
int readNtpServerIpAddress(char *ipMode, size_t size);           // Ntp Server IpAddress
int readNtpServerMacAddress(char *macAddr, size_t size);         // Ntp Server MacAddress
int readNtpServerVlanStatus(char *vlanStatus, size_t size);      // Ntp Server VlanStatus
int readNtpServerVlanAddress(char *vlanStatus, size_t size);     // Ntp Server VlanAddress
int readNtpServerUnicastMode(char *mode, size_t size);           // Ntp Server UnicastMode
int readNtpServerMulticastMode(char *mode, size_t size);         // Ntp Server MulticastMode
int readNtpServerBroadcastMode(char *mode, size_t size);         // Ntp Server BroadcastMode
int readNtpServerPrecisionValue(char *value, size_t size);       // Ntp Server PrecisionValue
int readNtpServerPollIntervalValue(char *value, size_t size);    // Ntp Server PollIntervalValue
int readNtpServerStratumValue(char *value, size_t size);         // Ntp Server StratumValue
int readNtpServerReferenceId(char *value, size_t size);          // Ntp Server ReferenceId
int readNtpServerSmearingStatus(char *status, size_t size);      // Ntp Server SmearingStatus
int readNtpServerLeap61Progress(char *progress, size_t size);    // Ntp Server Leap61Progress
int readNtpServerLeap59Progress(char *progress, size_t size);    // Ntp Server Leap59Progress
int readNtpServerLeap61Status(char *status, size_t size);        // Ntp Server Leap61Status
int readNtpServerLeap59Status(char *status, size_t size);        // Ntp Server Leap59Status
int readNtpServerUtcOffsetStatus(char *status, size_t size);     // Ntp Server UtcOffsetStatus
int readNtpServerUtcOffsetValue(char *value, size_t size);       // Ntp Server UtcOffsetValue
int readNtpServerRequestsValue(char *value, size_t size);        // Ntp Server RequestsValue
int readNtpServerResponsesValue(char *value, size_t size);       // Ntp Server ResponsesValue
int readNtpServerRequestsDroppedValue(char *value, size_t size); // Ntp Server RequestsDroppedValue
int readNtpServerBroadcastsValue(char *value, size_t size);      // Ntp Server BroadcastsValue
int readNtpServerClearCountersStatus(char *value, size_t size);  // Ntp Server ClearCountersStatus
int readNtpServerVersion(char *value, size_t size);              // Ntp Server Version

int writeStatus(char *status, size_t size);

#endif // NTP_SERVER_H