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

	return "ip4 address parsing error"
}

func GetIpv4Gateway(i string) string {
	gwLine := GetNmcliField("IP4.GATEWAY", i)

	fields := strings.Fields(gwLine)
	if len(fields) > 1 {
		return strings.TrimSpace(fields[1])
	}

	return "ip4 gateway parsing error"
}

func GetIpv4MacAddress(i string) string {
	macLine := GetNmcliField("GENERAL.HWADDR", i)
	fields := strings.Fields(macLine)
	if len(fields) > 1 {
		return strings.TrimSpace(fields[1])
	}

	return "mac address parsing error"
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

	return "dns 1 parsing error"
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

	return "dns 2 parsing error"
}

// ignore yes / no
func SetIgnoreAutoDns(i string, ignore string) {
	SetNmcliField(i, "ipv4.ignore-auto-dns", ignore)
}

func SetIp4Method(i string, method string) {
	SetNmcliField(i, "ipv4.method", method)
}

func SetIpv4Dns(i string, dns ...string) {

	SetIgnoreAutoDns(i, "yes")

	// Join multiple DNS addresses into a comma-separated string
	dnsArg := dns[0]
	if len(dns) > 1 {
		dnsArg = dns[0] + "," + dns[1]
	}

	SetNmcliField(i, "ipv4.dns", dnsArg)

}

func SetIpv4Address(i string, address string) {

	//currentGw := GetIpv4Gateway(i)

	ip := net.ParseIP(address)
	if ip != nil {
		SetIp4Method(i, "manual")
		//SetIpv4Gateway(i, currentGw)
		SetNmcliField(i, "ipv4.address", address)

	}

}

func SetIpv4Gateway(i string, address string) {
	//
	//	currentIp := GetIpv4Address(i)
	//
	//	gw := net.ParseIP(address)
	//	if gw != nil {
	//		SetIp4Method(i, "manual")
	//		SetIpv4Address(i, currentIp)
	//		SetNmcliField(i, "ipv4.gateway", gw.String())
	//
	//	}

}
