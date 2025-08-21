package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var ip4Cmd = &cobra.Command{
	Use:   "ip4",
	Short: "IPv4 configuration commands",
	Long:  "Use this command to manage IPv4 addresses, gateways, DNS, DHCP, and routes.",
}

var ip4GetCmd = &cobra.Command{
	Use:   "get <intfc>",
	Short: "Display IPv4 address and netmask",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		iface := args[0]

		fmt.Println("ip4 address: ", lib.GetIPv4Address(iface))
		fmt.Println("ip4 address: ", lib.GetIPv4Netmask(iface))

	},
}

var ip4SetCmd = &cobra.Command{
	Use:   "set <intfc> <addr> <gw> <dns>",
	Short: "Set IPv4 address and netmask",
	Args:  cobra.ExactArgs(3),
	Run: func(cmd *cobra.Command, args []string) {
		iface, addr, gw, dns := args[0], args[1], args[2], args[3]
		if err := lib.SetStaticIPv4(iface, addr, gw, dns); err != nil {
			fmt.Println("Error:", err)
		} else {
			fmt.Println("IPv4 address set successfully")
		}

	},
}

var dns4GetCmd = &cobra.Command{
	Use:   "dns4get <intfc>",
	Short: "Display DNS servers",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		iface := args[0]
		pri := lib.GetPrimaryDNS(iface)
		sec := lib.GetSecondaryDNS(iface)

		fmt.Printf("%s DNS -> primary: %s, secondary: %s\n", iface, pri, sec)
	},
}

var routes4Cmd = &cobra.Command{
	Use:   "routes4 [intfc]",
	Short: "Display current IPv4 routing table",
	Args:  cobra.MaximumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		iface := ""
		if len(args) > 0 {
			iface = args[0]
		}
		lib.PrintRoutes(iface)

	},
}

func init() {
	rootCmd.AddCommand(ip4Cmd)

	// attach subcommands
	ip4Cmd.AddCommand(ip4GetCmd)
	ip4Cmd.AddCommand(ip4SetCmd)
	ip4Cmd.AddCommand(dns4GetCmd)
	ip4Cmd.AddCommand(routes4Cmd)

	// TODO: add gw4get, gw4set, dhcp4set, rt4add, etc. in the same style
}
