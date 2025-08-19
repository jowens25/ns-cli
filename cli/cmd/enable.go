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
	Long: `Use this command to enable insecure protocols such as 
ssh, telnet, snmp and http, ports and ethernet port.`,
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

			case "app":
				api.StartApp()

			case "port":
				if len(args) != 0 {
					port := args[1]
					api.EnablePort(port)
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

	enableCmd.Flags().Bool("telnet", false, "enable telnet")
	enableCmd.Flags().Bool("ssh", false, "enable ssh")
	enableCmd.Flags().Bool("http", false, "enable http")
	enableCmd.Flags().Bool("port", false, "enable port")
	enableCmd.Flags().Bool("all", false, "enable insecure protocols")
	enableCmd.Flags().Bool("interface", false, "enable an interface")
	enableCmd.Flags().Bool("web", false, "enable web app")

}
