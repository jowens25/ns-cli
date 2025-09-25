package lib

import (
	"fmt"
	"log"
	"net"
	"net/http"
	"strings"

	"github.com/gin-gonic/gin"
)

func GetIpv4Address(i string) string {

	addrLine := GetNmcliInterfaceField(i, "IP4.ADDRESS")

	fields := strings.Fields(addrLine)
	if len(fields) > 1 {

		ipAddr, _, err := net.ParseCIDR(fields[1])
		if err != nil {
			log.Printf("Error parsing CIDR: %v", err)
			return "--"

		}

		return ipAddr.String()
	}

	return "--"
}

func GetIpv4NetmaskBits(i string) string {

	addrLine := GetNmcliInterfaceField(i, "IP4.ADDRESS")

	fields := strings.Fields(addrLine)
	if len(fields) > 1 {
		_, ipNet, err := net.ParseCIDR(fields[1])
		_, bits := ipNet.Mask.Size()

		if err != nil {
			log.Printf("Error parsing CIDR: %v", err)
			return "--"

		}

		return fmt.Sprintf("%d", bits)
	}

	return "--"
}

func GetIpv4NetmaskAddress(i string) string {

	addrLine := GetNmcliInterfaceField(i, "IP4.ADDRESS")

	fields := strings.Fields(addrLine)
	if len(fields) > 1 {

		_, ipNet, err := net.ParseCIDR(fields[1])
		if err != nil {
			log.Printf("Error parsing CIDR: %v", err)
			return "--"

		}

		return net.IP(ipNet.Mask).String()
	}

	return "--"
}

func GetIpv4Gateway(i string) string {

	gwLine := GetNmcliInterfaceField(i, "IP4.GATEWAY")

	fields := strings.Fields(gwLine)
	if len(fields) > 1 {
		return strings.TrimSpace(fields[1])
	}

	return "--"
}

func GetIpv4MacAddress(i string) string {

	macLine := GetNmcliInterfaceField(i, "GENERAL.HWADDR")
	fields := strings.Fields(macLine)
	if len(fields) > 1 {
		return strings.TrimSpace(fields[1])
	}

	return "--"
}

func GetIpv4Dns1(i string) string {

	dnsLines := GetNmcliInterfaceField(i, "IP4.DNS")

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

	dnsLines := GetNmcliInterfaceField(i, "IP4.DNS")

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

func GetIgnoreAutoDns(i string) string {

	c, _ := GetConnectionNameFromDevice(i)

	ignoreDnsLines := GetNmcliField(c, "ipv4.ignore-auto-dns")

	fields := strings.Fields(ignoreDnsLines)
	if len(fields) > 1 {
		return strings.TrimSpace(fields[1])
	}

	return "--"
}

func GetIpv4DhcpState(i string) string {
	c, _ := GetConnectionNameFromDevice(i)

	methodLine := GetNmcliField(c, "ipv4.method")

	fields := strings.Fields(methodLine)
	if len(fields) > 1 {
		return strings.TrimSpace(fields[1])
	}

	return "--"

}

// requires std addr format
func _combineNetmaskAndAddress(netmaskBits string, address string) string {

	return address + "/" + netmaskBits

}

func SetGateway(i string, gw string) {
	c, _ := GetConnectionNameFromDevice(i)
	//currentNetmask := GetIpv4Netmask(i)
	SetNmcliConnectionStatus(c, "down")

	if !isValidAddress(gw) {
		return
	}

	SetNmcliField(c, "ipv4.gateway", gw)

	SetNmcliField(c, "ipv4.method", "manual")

	SetNmcliConnectionStatus(c, "up")

}

func SetNetmask(i string, mask string) {

	parsedMask := net.IPMask(net.ParseIP(mask).To4())

	// Get the CIDR prefix length (number of bits in the mask)
	ones, _ := parsedMask.Size()

	netmask := fmt.Sprintf("%d", ones)

	addr := GetIpv4Address(AppConfig.Network.Interface)

	c, _ := GetConnectionNameFromDevice(i)

	SetNmcliConnectionStatus(c, "down")

	combinedAddress := _combineNetmaskAndAddress(netmask, addr)

	if !isValidAddressCIDR(combinedAddress) {
		return
	}

	// this is confusing because it looks like we are setting the ip... trust us
	SetNmcliField(c, "ipv4.address", addr)

	SetNmcliField(c, "ipv4.method", "manual")

	SetNmcliConnectionStatus(c, "up")

}

func isValidAddress(a string) bool {

	// returns nil if not a valid address
	err := net.ParseIP(a)
	if err == nil {
		fmt.Println("invalid address")
		return false
	}
	return true

}

func isValidAddressCIDR(a string) bool {
	_, _, err := net.ParseCIDR(a)
	if err != nil {
		fmt.Println("invalid CIDR address")
		return false
	}
	return true
}

func SetIpAddr(i string, ip string, gw ...string) {

	gateway := GetIpv4Gateway(AppConfig.Network.Interface)

	if len(gw) > 0 {
		gateway = gw[0]
	}

	addr := _combineNetmaskAndAddress("24", ip)

	c, _ := GetConnectionNameFromDevice(i)

	fmt.Println("setting static ip...")

	SetNmcliConnectionStatus(c, "down")

	if !isValidAddressCIDR(addr) {

		SetNmcliConnectionStatus(c, "up")

		return
	}

	SetNmcliField(c, "ipv4.address", addr)

	fmt.Println("gateway: " + gateway)

	if isValidAddress(gateway) {
		SetNmcliField(c, "ipv4.gateway", gateway)
	}

	SetNmcliField(c, "ipv4.dns", "8.8.8.8,8.8.4.4")

	SetNmcliField(c, "ipv4.method", "manual")

	SetNmcliConnectionStatus(c, "up")
}

func SetDns(i string, dns ...string) {

	c, _ := GetConnectionNameFromDevice(i)

	SetNmcliConnectionStatus(c, "down")

	SetNmcliField(c, "ipv4.ignore-auto-dns", "yes")

	dnsArg := dns[0]
	if len(dns) > 1 {
		dnsArg = dns[0] + "," + dns[1]
	}

	SetNmcliField(c, "ipv4.dns", dnsArg)

	SetNmcliConnectionStatus(c, "up")

}

func SetDns1(i string, dns1 string) {

	dns2 := GetIpv4Dns2(AppConfig.Network.Interface)

	SetDns(AppConfig.Network.Interface, dns1, dns2)

}

func SetDns2(i string, dns2 string) {

	dns1 := GetIpv4Dns1(AppConfig.Network.Interface)

	SetDns(AppConfig.Network.Interface, dns1, dns2)

}

func ResetNetworkConfig(i string) {

	MakeDefaultNmcliConnection()

	c, _ := GetConnectionNameFromDevice(i)

	SetNmcliConnectionStatus(c, "up")

	//SetNmcliConnectionStatus(c, "down")
	//
	//SetNmcliField(c, "ipv4.method", "auto")
	//SetNmcliField(c, "ipv4.ignore-auto-dns", "no")
	//
	//ClearNmcliField(c, "ipv4.gateway")
	//ClearNmcliField(c, "ipv4.addresses")
	//
	//SetNmcliConnectionStatus(c, "up")

}

// auto, manual
func SetDhcp4(i string, m string) {
	c, _ := GetConnectionNameFromDevice(i)
	SetNmcliConnectionStatus(c, "down")
	SetNmcliField(c, "ipv4.method", m)
	SetNmcliConnectionStatus(c, "up")

}

// yes / no
func SetIgnoreAutoDns(i string, m string) {

	c, _ := GetConnectionNameFromDevice(i)

	SetNmcliConnectionStatus(c, "down")

	SetNmcliField(c, "ipv4.ignore-auto-dns", m)

	SetNmcliConnectionStatus(c, "up")

}

func readNetworkInfo(c *gin.Context) {

	var myNetwork NetworkInfo

	myNetwork.Port = GetPortPhysicalStatus(AppConfig.Network.Interface)
	myNetwork.Hostname = GetHostname()
	myNetwork.Gateway = GetIpv4Gateway(AppConfig.Network.Interface)
	myNetwork.Interface = AppConfig.Network.Interface
	myNetwork.Speed = GetPortSpeed(AppConfig.Network.Interface)
	myNetwork.Mac = GetIpv4MacAddress(AppConfig.Network.Interface)
	myNetwork.IpAddr = GetIpv4Address(AppConfig.Network.Interface)
	myNetwork.Netmask = GetIpv4NetmaskAddress(AppConfig.Network.Interface)
	myNetwork.Dhcp = GetIpv4DhcpState(AppConfig.Network.Interface)
	myNetwork.Dns1 = GetIpv4Dns1(AppConfig.Network.Interface)
	myNetwork.Dns2 = GetIpv4Dns2(AppConfig.Network.Interface)
	myNetwork.IgnoreAutoDns = GetIgnoreAutoDns(AppConfig.Network.Interface)
	myNetwork.Connection = GetPortConnectionStatus(AppConfig.Network.Interface)

	c.JSON(http.StatusOK, gin.H{
		"info": myNetwork,
	})

}

func writeNetworkInfo(c *gin.Context) {

	serialMutex.Lock()
	defer serialMutex.Unlock()

	property := c.Param("prop")

	var data map[string]string
	if err := c.ShouldBindJSON(&data); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	v := data[property]

	switch property {

	case "port_status":
	case "hostname":
		SetHostname(v)
	case "gateway":
		SetGateway(AppConfig.Network.Interface, v)
	case "interface":
	case "speed":
	case "mac":
	case "ip_address":
		SetIpAddr(AppConfig.Network.Interface, v)
	case "netmask":
		SetNetmask(AppConfig.Network.Interface, v)
	case "dhcp":
		SetDhcp4(AppConfig.Network.Interface, v)
	case "dns1":
		SetDns1(AppConfig.Network.Interface, v)

	case "dns2":
		SetDns2(AppConfig.Network.Interface, v)

	case "ignore_auto_dns":
		SetIgnoreAutoDns(AppConfig.Network.Interface, v)
	case "connection_status":

	}

	c.JSON(http.StatusOK, gin.H{
		"message": "updated",
	})

}

func writeNetworkReset(c *gin.Context) {

	serialMutex.Lock()
	defer serialMutex.Unlock()

	ResetNetworkConfig(AppConfig.Network.Interface)
	c.JSON(http.StatusOK, gin.H{
		"message": "network config reset",
	})

}
