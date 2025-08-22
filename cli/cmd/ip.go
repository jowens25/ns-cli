package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

var ipCmd = &cobra.Command{
	Use:   "ip",
	Short: "ip configuration commands",
	Long:  "Use this command to configure IPv4",

	Run: func(cmd *cobra.Command, args []string) {
		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			switch f.Name {

			case "addr":
				if len(args) == 0 {
					fmt.Println(lib.GetIPv4Address("enp3s0"))

				} else if len(args) == 3 {
					lib.SetStaticIPv4("enp3s0", args[0], args[1], args[2])

				} else {
					cmd.Help()
				}

			case "routes":
				lib.Routes4()

			case "dns":
				fmt.Println(lib.GetDNSServers("enp3s0"))
			case "mask":
			case "gate":

			default:
				cmd.Help()
			}

		})

		if !hasFlags {
			cmd.Help()
		}

	},
}

var ip6Cmd = &cobra.Command{
	Use:   "ip6",
	Short: "ip6 configuration commands",
	Long:  "Use this command to configure IPv6",

	Run: func(cmd *cobra.Command, args []string) {
		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			switch f.Name {

			case "addr":

			case "routes":
				lib.Routes6()

			case "dns":
				//fmt.Println(lib.GetDNSServers("enp3s0"))
			case "mask":
			case "gate":

			default:
				cmd.Help()
			}

		})

		if !hasFlags {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(ipCmd)
	rootCmd.AddCommand(ip6Cmd)

	ipCmd.Flags().BoolP("routes", "r", false, "routing table")
	ipCmd.Flags().BoolP("dns", "d", false, "dns")
	ipCmd.Flags().BoolP("addr", "a", false, "configure address")
	ipCmd.Flags().BoolP("mask", "m", false, "configure netmask")
	ipCmd.Flags().BoolP("gate", "g", false, "configure gateway")

	ip6Cmd.Flags().BoolP("routes", "r", false, "routing table")
	ip6Cmd.Flags().BoolP("dns", "d", false, "dns")
	ip6Cmd.Flags().BoolP("addr", "a", false, "configure address")
	ip6Cmd.Flags().BoolP("mask", "m", false, "configure netmask")
	ip6Cmd.Flags().BoolP("gate", "g", false, "configure gateway")

}
