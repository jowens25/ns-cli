package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var networkCmd = &cobra.Command{
	Use:   "network",
	Short: "configure network parameters",
}

// ---- STATUS ----
var networkStatusCmd = &cobra.Command{
	Use:   "status",
	Short: "network configuration overview",

	Run: func(cmd *cobra.Command, args []string) {

		if !lib.HasInterface(lib.AppConfig.Network.Interface) {
			return
		}

		fmt.Println("Ethernet:     ", lib.GetPortPhysicalStatus(lib.AppConfig.Network.Interface))
		fmt.Print("Hostname:      ", lib.GetHostname())
		fmt.Println("Gateway:      ", lib.GetIpv4Gateway(lib.AppConfig.Network.Interface))
		fmt.Println("Interface:    ", lib.AppConfig.Network.Interface, lib.GetPortSpeed(lib.AppConfig.Network.Interface))
		fmt.Println("MAC:          ", lib.GetIpv4MacAddress(lib.AppConfig.Network.Interface))
		fmt.Println("IPv4:         ", lib.GetIpv4Address(lib.AppConfig.Network.Interface))
		fmt.Println("DHCPv4:       ", lib.GetIpv4DhcpState(lib.AppConfig.Network.Interface))
		fmt.Println("DNS 1:        ", lib.GetIpv4Dns1(lib.AppConfig.Network.Interface))
		fmt.Println("DNS 2:        ", lib.GetIpv4Dns2(lib.AppConfig.Network.Interface))
		fmt.Println("Connection:   ", lib.GetPortConnectionStatus(lib.AppConfig.Network.Interface))
	},
}

// ---- DNS ----
var dnsCmd = &cobra.Command{
	Use:   "dns [dns1] [dns2]",
	Short: "get and set dns",
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 1 {
			lib.SetIpv4Dns(lib.AppConfig.Network.Interface, args[0])
		}
		if len(args) == 2 {
			lib.SetIpv4Dns(lib.AppConfig.Network.Interface, args[0], args[1])
		}

		fmt.Println("DNS1:", lib.GetIpv4Dns1(lib.AppConfig.Network.Interface))
		fmt.Println("DNS2:", lib.GetIpv4Dns2(lib.AppConfig.Network.Interface))
	},
}

// ---- IP ----
var ipCmd = &cobra.Command{
	Use:   "ip [address]",
	Short: "get and set ip address",
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 1 {
			lib.SetIpv4Address(lib.AppConfig.Network.Interface, args[0])
		}

		fmt.Println(lib.GetIpv4Address(lib.AppConfig.Network.Interface))
	},
}

// ---- GW ----
var gwCmd = &cobra.Command{
	Use:   "gw [address]",
	Short: "get and set gateway address",

	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 1 {
			lib.SetIpv4Gateway(lib.AppConfig.Network.Interface, args[0])
		}

		fmt.Println(lib.GetIpv4Gateway(lib.AppConfig.Network.Interface))
	},
}

var routesCmd = &cobra.Command{
	Use:   "routes",
	Short: "manage static routes",
	Args:  cobra.MinimumNArgs(0),
	Run: func(cmd *cobra.Command, args []string) {

		fmt.Println(lib.ShowIpv4Routes(lib.AppConfig.Network.Interface))

	},
}

var routeAddCmd = &cobra.Command{
	Use:   "add [subnet] [next-hop]",
	Short: "add a static route",
	Args:  cobra.ExactArgs(2),
	Run: func(cmd *cobra.Command, args []string) {
		subnet, nextHop := args[0], args[1]

		lib.AddIpv4Route(lib.AppConfig.Network.Interface, subnet, nextHop)

		fmt.Println(lib.ShowIpv4Routes(lib.AppConfig.Network.Interface))

	},
}

var routeRemoveCmd = &cobra.Command{
	Use:   "remove [subnet] [next-hop]",
	Short: "remove a static route",
	Args:  cobra.ExactArgs(2),
	Run: func(cmd *cobra.Command, args []string) {
		subnet, nextHop := args[0], args[1]

		lib.RemoveIpv4Route(lib.AppConfig.Network.Interface, subnet, nextHop)

		fmt.Println(lib.ShowIpv4Routes(lib.AppConfig.Network.Interface))

	},
}

var portEnableCmd = &cobra.Command{
	Use:   "enable",
	Short: "enable lan port",

	Run: func(cmd *cobra.Command, args []string) {
		lib.PortConnect(lib.AppConfig.Network.Interface)
	},
}

var portDisableCmd = &cobra.Command{
	Use:   "disable",
	Short: "disable lan port",

	Run: func(cmd *cobra.Command, args []string) {
		lib.PortDisconnect(lib.AppConfig.Network.Interface)
	},
}

var networkResetCmd = &cobra.Command{
	Use:   "reset [address]",
	Short: "reset network to use dhcp and auto dns on specified gateway",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		lib.ResetNetworkConfig(lib.AppConfig.Network.Interface, args[0])
	},
}

func init() {
	rootCmd.AddCommand(networkCmd)

	// Base network configuration
	networkCmd.AddCommand(networkStatusCmd)
	networkCmd.AddCommand(networkResetCmd)

	networkCmd.AddCommand(dnsCmd)
	networkCmd.AddCommand(ipCmd)
	networkCmd.AddCommand(gwCmd)

	networkCmd.AddCommand(portEnableCmd)
	networkCmd.AddCommand(portDisableCmd)

	// Routes
	networkCmd.AddCommand(routesCmd)
	routesCmd.AddCommand(routeAddCmd)
	routesCmd.AddCommand(routeRemoveCmd)
}
