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
	Use:   "status [interface]",
	Short: "network configuration overview",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		itf := args[0]
		if !lib.HasInterface(itf) {
			return
		}

		fmt.Println("Hostname:", lib.GetHostname())
		fmt.Println("Main IPv4 default gateway:", lib.GetIpv4Gateway(itf))
		fmt.Println(itf, lib.GetPortSpeed(itf))
		fmt.Println("MAC:", lib.GetIpv4MacAddress(itf))
		fmt.Println(lib.GetIpv4Address(itf))
		fmt.Println("DHCPv4:", lib.GetIpv4DhcpState(itf))
		fmt.Println("DNS 1:", lib.GetIpv4Dns1(itf))
		fmt.Println("DNS 2:", lib.GetIpv4Dns2(itf))
	},
}

// ---- DNS ----
var dnsCmd = &cobra.Command{
	Use:   "dns [interface] [dns1] [dns2]",
	Short: "get and set dns",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		itf := args[0]
		if !lib.HasInterface(itf) {
			return
		}

		if len(args) == 2 {
			lib.SetIpv4Dns(itf, args[1])
		}
		if len(args) == 3 {
			lib.SetIpv4Dns(itf, args[1], args[2])
		}

		fmt.Println("DNS1:", lib.GetIpv4Dns1(itf))
		fmt.Println("DNS2:", lib.GetIpv4Dns2(itf))
	},
}

// ---- IP ----
var ipCmd = &cobra.Command{
	Use:   "ip [interface] [address]",
	Short: "get and set ip address",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		itf := args[0]
		if !lib.HasInterface(itf) {
			return
		}

		if len(args) == 2 {
			lib.SetIpv4Address(itf, args[1])
		}

		fmt.Println(lib.GetIpv4Address(itf))
		fmt.Println(lib.GetIpv4Netmask(itf))
	},
}

// ---- GW ----
var gwCmd = &cobra.Command{
	Use:   "gw [interface] [address]",
	Short: "get and set gateway address",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		itf := args[0]
		if !lib.HasInterface(itf) {
			return
		}

		if len(args) == 2 {
			lib.SetIpv4Gateway(itf, args[1])
		}

		fmt.Println(lib.GetIpv4Gateway(itf))
	},
}

var routesCmd = &cobra.Command{
	Use:   "routes",
	Short: "manage static routes",

	Args: cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		itf := args[0]
		if !lib.HasInterface(itf) {
			return
		}

		fmt.Println(lib.ShowIpv4Routes(itf))

	},
}

var routeAddCmd = &cobra.Command{
	Use:   "add [interface] [subnet] [next-hop]",
	Short: "add a static route",
	Args:  cobra.ExactArgs(3),
	Run: func(cmd *cobra.Command, args []string) {
		itf, subnet, nextHop := args[0], args[1], args[2]
		if !lib.HasInterface(itf) {
			return
		}
		lib.AddIpv4Route(itf, subnet, nextHop)

		fmt.Println(lib.ShowIpv4Routes(itf))

	},
}

var routeRemoveCmd = &cobra.Command{
	Use:   "remove [interface] [subnet] [next-hop]",
	Short: "remove a static route",
	Args:  cobra.ExactArgs(3),
	Run: func(cmd *cobra.Command, args []string) {
		itf, subnet, nextHop := args[0], args[1], args[2]
		if !lib.HasInterface(itf) {
			return
		}
		lib.RemoveIpv4Route(itf, subnet, nextHop)

		fmt.Println(lib.ShowIpv4Routes(itf))

	},
}

func init() {
	rootCmd.AddCommand(networkCmd)

	// Base network configuration
	networkCmd.AddCommand(networkStatusCmd)
	networkCmd.AddCommand(dnsCmd)
	networkCmd.AddCommand(ipCmd)
	networkCmd.AddCommand(gwCmd)

	// Routes
	networkCmd.AddCommand(routesCmd)
	routesCmd.AddCommand(routeAddCmd)
	routesCmd.AddCommand(routeRemoveCmd)
}
