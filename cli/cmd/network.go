package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var networkCmd = &cobra.Command{
	Use:   "network",
	Short: "network configuration",
}

var networkStatusCmd = &cobra.Command{
	Use:   "status",
	Short: "network configuration overview",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		itf := args[0]

		if !lib.HasInterface(itf) {
			return
		}

		fmt.Println("Hostname:", lib.GetHostname())
		fmt.Println("Main IPv4 default gateway:", lib.GetIpv4Gateway(itf))
		fmt.Println("Main IPv6 default gateway:", lib.GetIpv6Gateway(itf))
		fmt.Println(itf, lib.GetPortSpeed(itf))
		fmt.Println("MAC:", lib.GetIpv4MacAddress(itf))
		fmt.Println(lib.GetIpv4Address(itf))
		fmt.Println("DHCPv4:", lib.GetIpv4DhcpState(itf))
		fmt.Println("DNS 1:", lib.GetIpv4Dns1(itf))
		fmt.Println("DNS 2:", lib.GetIpv4Dns2(itf))
		fmt.Println(lib.GetIpv6Address(itf))
		fmt.Println("DHCPv6:", lib.GetIpv6DhcpState(itf))

	},
}

var dnsCmd = &cobra.Command{
	Use:   "dns [interface] [dns1] [dns2]",
	Short: "get and set dns",

	Run: func(cmd *cobra.Command, args []string) {

		itf := args[0]

		if !lib.HasInterface(itf) {
			return
		}

		if len(args) == 2 {
			lib.SetIpv4Dns(args[0], args[1])

		}
		if len(args) == 3 {
			lib.SetIpv4Dns(args[0], args[1], args[2])

		}

		fmt.Println("DNS1:", lib.GetIpv4Dns1(args[0]))
		fmt.Println("DNS2:", lib.GetIpv4Dns2(args[0]))

	},
}

var ipCmd = &cobra.Command{
	Use:   "ip [interface] [address]",
	Short: "get and set ip address",
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

func init() {
	rootCmd.AddCommand(networkCmd)
	networkCmd.AddCommand(networkStatusCmd)
	networkCmd.AddCommand(dnsCmd)
	networkCmd.AddCommand(ipCmd)
	//networkCmd.AddCommand(networkEnableCmd)
	//networkCmd.AddCommand(networkDisableCmd)
}
