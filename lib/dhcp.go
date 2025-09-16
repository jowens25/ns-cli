package lib

import (
	"strings"
)

func GetIpv4DhcpState(i string) string {

	connection := GetConnectionNameFromDevice(i)

	methodLine := GetNmcliConnectionField("ipv4.method", connection)

	fields := strings.Fields(methodLine)
	if len(fields) > 1 {
		return strings.TrimSpace(fields[1])
	}

	return "ip4 method parsing error"

}

// auto, manual
func SetDhcp4(i string, m string) {
	connection := GetConnectionNameFromDevice(i)
	SetNmcliConnectionStatus(connection, "down")
	SetNmcliField(i, "ipv4.method", m)
	SetNmcliConnectionStatus(connection, "up")

}
