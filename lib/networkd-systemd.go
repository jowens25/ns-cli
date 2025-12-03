package lib

import (
	"encoding/json"
	"fmt"
	"net"
	"os"
	"os/exec"
	"strings"
)

type NetworkInterface struct {
	Index                             *int          `json:"Index"`
	Name                              *string       `json:"Name"`
	Type                              *string       `json:"Type"`
	Driver                            *string       `json:"Driver"`
	Flags                             *int          `json:"Flags"`
	FlagsString                       *string       `json:"FlagsString"`
	KernelOperationalState            *int          `json:"KernelOperationalState"`
	KernelOperationalStateString      *string       `json:"KernelOperationalStateString"`
	MTU                               *int          `json:"MTU"`
	MinimumMTU                        *int          `json:"MinimumMTU"`
	MaximumMTU                        *int          `json:"MaximumMTU"`
	HardwareAddress                   *[]int        `json:"HardwareAddress"`
	PermanentHardwareAddress          *[]int        `json:"PermanentHardwareAddress"`
	BroadcastAddress                  *[]int        `json:"BroadcastAddress"`
	IPv6LinkLocalAddress              *[]int        `json:"IPv6LinkLocalAddress"`
	AdministrativeState               *string       `json:"AdministrativeState"`
	OperationalState                  *string       `json:"OperationalState"`
	CarrierState                      *string       `json:"CarrierState"`
	AddressState                      *string       `json:"AddressState"`
	IPv4AddressState                  *string       `json:"IPv4AddressState"`
	IPv6AddressState                  *string       `json:"IPv6AddressState"`
	OnlineState                       *string       `json:"OnlineState"`
	NetworkFile                       *string       `json:"NetworkFile"`
	NetworkFileDropins                *[]string     `json:"NetworkFileDropins"`
	RequiredForOnline                 *bool         `json:"RequiredForOnline"`
	RequiredOperationalStateForOnline *[]string     `json:"RequiredOperationalStateForOnline"`
	RequiredFamilyForOnline           *string       `json:"RequiredFamilyForOnline"`
	ActivationPolicy                  *string       `json:"ActivationPolicy"`
	LinkFile                          *string       `json:"LinkFile"`
	Path                              *string       `json:"Path"`
	Vendor                            *string       `json:"Vendor"`
	Model                             *string       `json:"Model"`
	DNS                               *[]DNSEntry   `json:"DNS"`
	NTP                               *[]NTPEntry   `json:"NTP"`
	DNSSettings                       *[]DNSSetting `json:"DNSSettings"`
	Addresses                         *[]Address    `json:"Addresses"`
	Routes                            *[]Route      `json:"Routes"`
	DHCPv4Client                      *DHCPv4Client `json:"DHCPv4Client,omitempty"`
}

type DNSEntry struct {
	Family         *int    `json:"Family"`
	Address        *[]int  `json:"Address"`
	ConfigSource   *string `json:"ConfigSource"`
	ConfigProvider *[]int  `json:"ConfigProvider,omitempty"`
}

type NTPEntry struct {
	Family         *int    `json:"Family"`
	Address        *[]int  `json:"Address"`
	ConfigSource   *string `json:"ConfigSource"`
	ConfigProvider *[]int  `json:"ConfigProvider,omitempty"`
}

type DNSSetting struct {
	LLMNR        *string `json:"LLMNR,omitempty"`
	MDNS         *string `json:"MDNS,omitempty"`
	ConfigSource *string `json:"ConfigSource"`
}

type Address struct {
	Family                *int    `json:"Family"`
	Address               *[]int  `json:"Address"`
	PrefixLength          *int    `json:"PrefixLength"`
	ConfigSource          *string `json:"ConfigSource"`
	ConfigProvider        *[]int  `json:"ConfigProvider,omitempty"`
	Broadcast             *[]int  `json:"Broadcast,omitempty"`
	Scope                 *int    `json:"Scope"`
	ScopeString           *string `json:"ScopeString"`
	Flags                 *int    `json:"Flags"`
	FlagsString           *string `json:"FlagsString"`
	PreferredLifetimeUSec *int64  `json:"PreferredLifetimeUSec,omitempty"`
	PreferredLifetimeUsec *int64  `json:"PreferredLifetimeUsec,omitempty"`
	ValidLifetimeUSec     *int64  `json:"ValidLifetimeUSec,omitempty"`
	ValidLifetimeUsec     *int64  `json:"ValidLifetimeUsec,omitempty"`
	ConfigState           *string `json:"ConfigState"`
}

type Route struct {
	Family                  *int    `json:"Family"`
	Destination             *[]int  `json:"Destination"`
	DestinationPrefixLength *int    `json:"DestinationPrefixLength"`
	Gateway                 *[]int  `json:"Gateway,omitempty"`
	PreferredSource         *[]int  `json:"PreferredSource,omitempty"`
	TOS                     *int    `json:"TOS"`
	Scope                   *int    `json:"Scope"`
	Protocol                *int    `json:"Protocol"`
	Type                    *int    `json:"Type"`
	Priority                *int    `json:"Priority"`
	Table                   *int    `json:"Table"`
	Flags                   *int    `json:"Flags"`
	ConfigSource            *string `json:"ConfigSource"`
	ConfigProvider          *[]int  `json:"ConfigProvider,omitempty"`
	ScopeString             *string `json:"ScopeString"`
	ProtocolString          *string `json:"ProtocolString"`
	TypeString              *string `json:"TypeString"`
	TableString             *string `json:"TableString"`
	Preference              *int    `json:"Preference"`
	FlagsString             *string `json:"FlagsString"`
	ConfigState             *string `json:"ConfigState"`
}

type DHCPv4Client struct {
	Lease *Lease `json:"Lease"`
}

type Lease struct {
	LeaseTimestampUSec *int64 `json:"LeaseTimestampUSec"`
	Timeout1USec       *int64 `json:"Timeout1USec"`
	Timeout2USec       *int64 `json:"Timeout2USec"`
}

// reset networkd
func ResetNetworkd() {

	cmd := exec.Command("systemctl", "restart", "systemd-networkd")

	out, err := cmd.CombinedOutput()
	if err != nil {
		Print("%s", err.Error())
	}
	Print("%s", string(out))

}

func ResetResolved() {
	cmd := exec.Command("systemctl", "restart", "systemd-resolved")

	out, err := cmd.CombinedOutput()
	if err != nil {
		Print("%s", err.Error())
	}
	Print("%s", string(out))
}

// get networkctl info
func GetNetworkInfo(i string) NetworkInterface {

	cmd := exec.Command("networkctl", "status", i, "--json", "pretty")

	out, err := cmd.CombinedOutput()
	if err != nil {
		Print("%s", "cmd error?")
		Print("%s", err.Error())
	}

	var iface NetworkInterface

	err = json.Unmarshal(out, &iface)
	if err != nil {
		Print("%s", err)
	}

	if iface.Index == nil {
		Print("interface not found: %s", i)
	}

	//Print("%s", iface.OnlineState)

	return iface

}

// get interface from networkd config file
func GetManagedInterfaceName() string {
	content, err := os.ReadFile("/etc/systemd/network/ns.network")
	if err != nil {
		Print("%s", err.Error())
		return "--"
	}

	for line := range strings.SplitSeq(string(content), "\n") {
		if strings.HasPrefix(line, "Name=") {
			parts := strings.Split(line, "=")
			if len(parts) > 0 {
				return parts[1]
			}
		}
	}
	return "--"
}

// is connection up or down
func GetConnectionStatus(i string) string {

	interfaceData := GetNetworkInfo(i)

	out := "--"

	if interfaceData.OnlineState != nil {
		out = *interfaceData.OnlineState
	}

	return out
}

func GetPortSpeed(i string) string {
	cmd := exec.Command("networkctl", "status", i, "--no-pager")
	out, err := cmd.CombinedOutput()
	if err != nil {
		Print("%s", err.Error())
	}

	for line := range strings.SplitSeq(string(out), "\n") {
		line = strings.TrimSpace(line)
		if strings.HasPrefix(line, "Speed:") {
			parts := strings.Split(line, ":")
			if len(parts) > 0 {
				return strings.TrimSpace(parts[1])
			}

		}
	}

	return "--"
}

func GetIpv4MacAddress(i string) string {
	interfaceData := GetNetworkInfo(i)

	out := "--"

	if interfaceData.PermanentHardwareAddress != nil {

		macBytes := make([]byte, len(*interfaceData.PermanentHardwareAddress))
		for i, v := range *interfaceData.PermanentHardwareAddress {
			macBytes[i] = byte(v)
		}

		out = fmt.Sprintf("%02x:%02x:%02x:%02x:%02x:%02x",
			macBytes[0], macBytes[1], macBytes[2], macBytes[3], macBytes[4], macBytes[5])

		//out = *interfaceData.PermanentHardwareAddress
	}

	return out
}

func GetIpv4Address(i string) string {
	interfaceData := GetNetworkInfo(i)

	out := "--"

	if interfaceData.Addresses != nil {

		for _, addr := range *interfaceData.Addresses {

			if *addr.Family == 2 {

				out = fmt.Sprintf("%d.%d.%d.%d", (*addr.Address)[0], (*addr.Address)[1], (*addr.Address)[2], (*addr.Address)[3])

			}

		}

	}

	return out
}

func GetIpv4Gateway(i string) string {
	interfaceData := GetNetworkInfo(i)

	out := "--"

	if interfaceData.Addresses != nil {

		for _, addr := range *interfaceData.Addresses {

			if *addr.Family == 2 {

				out = fmt.Sprintf("%d.%d.%d.%d", (*addr.ConfigProvider)[0], (*addr.ConfigProvider)[1], (*addr.ConfigProvider)[2], (*addr.ConfigProvider)[3])

			}

		}

	}

	return out
}

func GetIpv4Netmask(i string) string {
	interfaceData := GetNetworkInfo(i)

	out := "--"

	if interfaceData.Addresses != nil {

		for _, addr := range *interfaceData.Addresses {

			if *addr.Family == 2 {

				mask := net.CIDRMask(*addr.PrefixLength, 32)
				out = net.IP(mask).String()

			}

		}

	}

	return out
}

func GetIpv4DhcpState(i string) string {
	interfaceData := GetNetworkInfo(i)

	out := "--"

	if interfaceData.Addresses != nil {

		for _, addr := range *interfaceData.Addresses {

			if *addr.Family == 2 {

				out = *addr.ConfigSource

			}

		}

	}

	return out
}

func GetIpv4Dns(i string) string {

	cmd := exec.Command("resolvectl", "dns", i)

	out, err := cmd.CombinedOutput()
	if err != nil {
		Print("%s", err.Error())
	}

	parts := strings.Split(string(out), ":")

	if len(parts) > 0 {
		return strings.TrimSpace(parts[1])
	} else {
		return "--"
	}

}

func GetDnsConfigSource(i string) string {
	interfaceData := GetNetworkInfo(i)

	out := "--"

	if interfaceData.DNS != nil {

		for _, entry := range *interfaceData.DNS {

			if *entry.Family == 2 {

				out = *entry.ConfigSource

			}

		}

	}

	return out
}
