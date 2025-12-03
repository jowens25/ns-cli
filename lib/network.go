package lib

import (
	"fmt"
	"log"
	"net"
	"net/http"
	"os/exec"

	"github.com/gin-gonic/gin"
)

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

func Reboot() {

	cmd := exec.Command("reboot", "now")
	err := cmd.Run()
	if err != nil {
		log.Printf("Error rebooting: %v", err)
	}
}
func ResetNetworkConfig(i string) {

	DisableNetworking()

	MakeDefaultNmcliConnection()

	c, _ := GetConnectionNameFromDevice(i)

	SetNmcliConnectionStatus(c, "up")

	Reboot()

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

func getNetworkInfoHandler(c *gin.Context) {

	var myNetwork NetworkInfo

	myNetwork.Interface = GetManagedInterfaceName()
	myNetwork.Hostname = GetHostname()
	myNetwork.Connection = GetConnectionStatus(myNetwork.Interface)

	//myNetwork.Port = GetPortPhysicalStatus(myNetwork.Interface)
	myNetwork.Gateway = GetIpv4Gateway(myNetwork.Interface)
	myNetwork.Speed = GetPortSpeed(myNetwork.Interface)
	myNetwork.Mac = GetIpv4MacAddress(myNetwork.Interface)
	myNetwork.IpAddr = GetIpv4Address(myNetwork.Interface)
	myNetwork.Netmask = GetIpv4Netmask(myNetwork.Interface)
	myNetwork.Dhcp = GetIpv4DhcpState(myNetwork.Interface)
	myNetwork.Dns1 = GetIpv4Dns(myNetwork.Interface)
	myNetwork.IgnoreAutoDns = GetDnsConfigSource(myNetwork.Interface)

	c.JSON(http.StatusOK, gin.H{
		"info": myNetwork,
	})

}

func writeNetworkInfo(c *gin.Context) {

	SerialMutex.Lock()
	defer SerialMutex.Unlock()

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
		//SetDns1(AppConfig.Network.Interface, v)

	case "dns2":
		//SetDns2(AppConfig.Network.Interface, v)

	case "ignore_auto_dns":
		SetIgnoreAutoDns(AppConfig.Network.Interface, v)
	case "connection_status":

	}

	c.JSON(http.StatusOK, gin.H{
		"message": "updated",
	})

}

func writeNetworkReset(c *gin.Context) {

	SerialMutex.Lock()
	defer SerialMutex.Unlock()

	ResetNetworkConfig(AppConfig.Network.Interface)
	c.JSON(http.StatusOK, gin.H{
		"message": "network config reset",
	})

}
