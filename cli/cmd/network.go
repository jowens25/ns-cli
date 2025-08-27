package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var networkCmd = &cobra.Command{
	Use:   "network",
	Short: "network configuration",
	Long:  `Use this command to view overall network configuration.`,
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		itf := args[0]

		fmt.Println("Hostname:", lib.GetHostname())
		fmt.Println("Main IPv4 default gateway:", lib.GetIpGateway(itf))
		fmt.Println("Main IPv6 default gateway:", lib.GetIp6Gateway(itf))
		fmt.Println(itf, lib.GetPortSpeed(itf))
		fmt.Println("MAC:", lib.GetIpv4MacAddress(itf))
		fmt.Println(lib.GetIpv4Address(itf))
		fmt.Println("DHCPv4:", itf, lib.GetIpv4DhcpState(itf))
		fmt.Println("DNS 1:", lib.GetIpv4Dns1(itf))
		fmt.Println("DNS 2:", lib.GetIpv4Dns2(itf))
		fmt.Println(lib.GetIpv6Address(itf))
		fmt.Println("DHCPv6:", itf, lib.GetIpv6DhcpState(itf))

		//fmt.Println(lib.Get)

	},
}

//	var networkEnableCmd = &cobra.Command{
//		Use:   "enable [protocol]",
//		Short: "Enable a network protocol",
//		Args:  cobra.ExactArgs(1),
//		Run: func(cmd *cobra.Command, args []string) {
//
//			switch args[0] {
//			case "ssh":
//				lib.EnableSsh()
//			case "ftp":
//				lib.EnableFtp()
//			case "telnet":
//				lib.EnableTelnet()
//			case "http":
//			case "app":
//				lib.StartApp()
//			default:
//				cmd.Help()
//			}
//		},
//	}
//
//	var networkDisableCmd = &cobra.Command{
//		Use:   "disable [protocol]",
//		Short: "Disable a network protocol",
//		Args:  cobra.ExactArgs(1),
//		Run: func(cmd *cobra.Command, args []string) {
//
//			switch args[0] {
//			case "ssh":
//				lib.DisableSsh()
//			case "ftp":
//				lib.DisableFtp()
//			case "telnet":
//				lib.DisableTelnet()
//			case "http":
//			case "app":
//				lib.StopApp()
//			default:
//				cmd.Help()
//			}
//		},
//	}
func init() {
	rootCmd.AddCommand(networkCmd)
	//networkCmd.AddCommand(networkEnableCmd)
	//networkCmd.AddCommand(networkDisableCmd)
}
