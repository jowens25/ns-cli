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

func EnableDhcp4(i string) {

	SetNmcliField(i, "ipv4.method", "auto")

}

func DisableDhcp4(i string) {

	SetNmcliField(i, "ipv4.method", "manual")

}
