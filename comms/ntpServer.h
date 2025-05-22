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
// int64_t temp_data;
// int64_t temp_addr;
int readNtpServerStatus(char *status, size_t size);              // Ntp Server Status
int readNtpServerInstanceNumber(char *status, size_t size);      // Ntp Server InstanceNumber
int readNtpServerIpMode(char *ipMode, size_t size);              // Ntp Server IpMode
int readNtpServerIpAddress(char *ipAddr, size_t size);           // Ntp Server IpAddress
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

int writeNtpServerStatus(char *status, size_t size);     // ntp server status
int writeNtpServerMacAddress(char *addr, size_t size);   // Ntp Server MacAddress
int writeNtpServerVlanStatus(char *status, size_t size); // Ntp Server VlanStatus
int writeNtpServerVlanAddress(char *value, size_t size); // Ntp Server VlanAddress
int writeNtpServerIpMode(char *mode, size_t size);       // Ntp Server Ip Mode
int writeNtpServerIpAddress(char *addr, size_t size);
int writeNtpServerUnicastMode(char *mode, size_t size);          // Ntp Server UnicastMode
int writeNtpServerMulticastMode(char *mode, size_t size);        // Ntp Server MulticastMode
int writeNtpServerBroadcastMode(char *mode, size_t size);        // Ntp Server BroadcastMode
int writeNtpServerPrecisionValue(char *value, size_t size);      // Ntp Server PrecisionValue
int writeNtpServerPollIntervalValue(char *value, size_t size);   // Ntp Server PollIntervalValue
int writeNtpServerStratumValue(char *value, size_t size);        // Ntp Server StratumValue
int writeNtpServerReferenceIdValue(char *value, size_t size);    // Ntp Server ReferenceIdValue
int writeNtpServerUtcSmearingStatus(char *status, size_t size);  // Ntp Server UtcSmearingStatus
int writeNtpServerLeap61Status(char *status, size_t size);       // Ntp Server Leap61Status
int writeNtpServerLeap59Status(char *status, size_t size);       // Ntp Server Leap59Status
int writeNtpServerUtcOffsetStatus(char *status, size_t size);    // Ntp Server UtcOffsetStatus
int writeNtpServerUtcOffsetValue(char *value, size_t size);      // Ntp Server UtcOffsetValue
int writeNtpServerClearCountersStatus(char *value, size_t size); // Ntp Server ClearCountersStatus
// int writeNtpServerVersion(char *value, size_t size);              // Ntp Server Version
int ipv4toipv6(char *ipv4Address, char *ipv6Address, size_t size);
int ipv4_addr_to_register_value(char *ipAddress, size_t size);
int ipv6_addr_to_register_value(char *ipAddress, size_t size);

int ipv6toipv4(char *address, size_t size);
int writeNtpServerStatus(char *status, size_t size);
int ipAddressToByteArray(char *ipAddress, long *addressByteArray, size_t size);
int to4(char *ipv6, size_t size);
int to16(char *ipv4, size_t size);
#endif // NTP_SERVER_H