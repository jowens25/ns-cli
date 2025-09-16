package lib

import (
	"net"
	"strings"
)

func GetIpv4Address(i string) string {

	addrLine := GetNmcliField("IP4.ADDRESS", i)

	fields := strings.Fields(addrLine)
	if len(fields) > 1 {
		return strings.TrimSpace(fields[1])
	}

	return "--"
}

func GetIpv4Gateway(i string) string {
	gwLine := GetNmcliField("IP4.GATEWAY", i)

	fields := strings.Fields(gwLine)
	if len(fields) > 1 {
		return strings.TrimSpace(fields[1])
	}

	return "--"
}

func GetIpv4MacAddress(i string) string {
	macLine := GetNmcliField("GENERAL.HWADDR", i)
	fields := strings.Fields(macLine)
	if len(fields) > 1 {
		return strings.TrimSpace(fields[1])
	}

	return "--"
}

func GetIpv4Dns1(i string) string {
	dnsLines := GetNmcliField("IP4.DNS", i)

	for line := range strings.SplitSeq(dnsLines, "\n") {
		if strings.Contains(line, "IP4.DNS[1]") {
			fields := strings.Fields(line)
			if len(fields) > 1 {
				return strings.TrimSpace(fields[1])
			}
		}
	}

	return "--"
}

func GetIpv4Dns2(i string) string {
	dnsLines := GetNmcliField("IP4.DNS", i)

	for line := range strings.SplitSeq(dnsLines, "\n") {
		if strings.Contains(line, "IP4.DNS[2]") {
			fields := strings.Fields(line)
			if len(fields) > 1 {
				return strings.TrimSpace(fields[1])
			}
		}
	}

	return "--"
}

func SetIpv4Address(c string, address string) {
	ip := net.ParseIP(address)
	if ip != nil {
		SetNmcliField(c, "ipv4.addresses", address)
	}
}

func SetIpv4Gateway(c string, address string) {
	gw := net.ParseIP(address)
	if gw != nil {
		SetNmcliField(c, "ipv4.gateway", gw.String())
	}
}

func SetIpv4Method(c string, method string) {
	SetNmcliField(c, "ipv4.method", method)
}

func SetIpv4IgnoreAutoDns(c string, yesno string) {
	SetNmcliField(c, "ipv4.ignore-auto-dns", yesno)
}

func SetGateway(i string, gw string, addr string) {
	c := GetConnectionNameFromDevice(i)
	SetNmcliConnectionStatus(c, "down")

	SetIpv4Gateway(c, gw)

	SetIpv4Address(c, addr)

	SetNmcliConnectionStatus(c, "up")

}

func SetIpAddr(i string, addr string, gw ...string) {
	c := GetConnectionNameFromDevice(i)
	SetNmcliConnectionStatus(c, "down")

	if len(gw) == 1 {
		SetIpv4Gateway(c, gw[0])
	}

	SetIpv4Address(c, addr)

	SetNmcliConnectionStatus(c, "up")

}

func SetDns(i string, dns ...string) {

	c := GetConnectionNameFromDevice(i)

	SetNmcliConnectionStatus(c, "down")

	SetIpv4IgnoreAutoDns(c, "yes")

	dnsArg := dns[0]
	if len(dns) > 1 {
		dnsArg = dns[0] + "," + dns[1]
	}

	SetNmcliField(c, "ipv4.dns", dnsArg)

}

func ResetNetworkConfig(i string, address string) {

	c := GetConnectionNameFromDevice(i)
	SetNmcliConnectionStatus(c, "down")

	gw := net.ParseIP(address)
	if gw != nil {
		SetIpv4Method(c, "auto")
		SetIpv4IgnoreAutoDns(c, "no")
		SetIpv4Gateway(i, gw.String())
		SetIpv4Gateway(i, gw.String())

	}
	SetNmcliConnectionStatus(c, "up")

}
