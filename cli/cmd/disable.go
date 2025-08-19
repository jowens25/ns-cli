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

// disableCmd represents the disable command
var disableCmd = &cobra.Command{
	Use:   "disable",
	Short: "Disable ports, protocols and interfaces.",
	Long: `The disable command can be used to disable
telnet, ssh and http protocols, selected ports and the ethernet interface.`,
	Run: func(cmd *cobra.Command, args []string) {
		hasFlags := false
		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true
			switch f.Name {

			case "all":
				api.StopTelnet()
				api.StopSsh()
				api.StopHttp()
			case "telnet":
				api.StopTelnet()
			case "ssh":
				api.StopSsh()
			case "http":
				api.StopHttp()

			case "port":
				if len(args) != 0 {
					port := args[0]
					api.DisablePort(port)
				} else {
					fmt.Println("missing port")

				}

			case "interface":
				if len(args) != 0 {
					intf := args[0]
					api.DisableInterface(intf)
				} else {
					fmt.Println("missing interface")

				}

			default:
				fmt.Println(cmd.Help())
			}

		})
		if !hasFlags {
			cmd.Help()
		}
	},
}

func init() {
	rootCmd.AddCommand(disableCmd)

	disableCmd.Flags().BoolP("telnet", "t", false, "disable telnet")
	disableCmd.Flags().BoolP("ssh", "s", false, "disable ssh")
	disableCmd.Flags().BoolP("http", "g", false, "disable http")
	disableCmd.Flags().BoolP("port", "p", false, "disable port")
	disableCmd.Flags().BoolP("all", "a", false, "disable insecure protocols")
	disableCmd.Flags().BoolP("interface", "i", false, "disable interface")

}
