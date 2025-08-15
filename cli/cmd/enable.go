/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/api"
	"fmt"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

// enableCmd represents the enable command
var enableCmd = &cobra.Command{
	Use:   "enable",
	Short: "enable ports and protocols",
	Long: `This command can be used to enable common protocols such as 
ssh, telnet, snmp and http and additional ports.`,
	Run: func(cmd *cobra.Command, args []string) {
		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			switch f.Name {
			case "all":
				api.StartTelnet()
				api.StartSsh()
				api.StartHttp()
			case "telnet":
				api.StartTelnet()
			case "ssh":
				api.StartSsh()
			case "http":
				api.StartHttp()

			case "port":
				if len(args) != 0 {
					port := args[1]
					api.DisablePort(port)
				} else {
					fmt.Println("missing port")

				}

			case "interface":
				if len(args) != 0 {
					intf := args[0]
					api.EnableInterface(intf)
				} else {
					fmt.Println("missing interface")

				}

			default:
			}
		})

		if !hasFlags {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(enableCmd)

	enableCmd.Flags().BoolP("telnet", "t", false, "enable telnet")
	enableCmd.Flags().BoolP("ssh", "s", false, "enable ssh")
	enableCmd.Flags().BoolP("http", "g", false, "enable http")
	enableCmd.Flags().BoolP("port", "p", false, "enable port")
	enableCmd.Flags().BoolP("all", "a", false, "enable insecure protocols")
	enableCmd.Flags().BoolP("interface", "i", false, "enable an interface")

}
