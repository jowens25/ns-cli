/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

// telnetCmd represents the telnet command
var telnetCmd = &cobra.Command{
	Use:   "telnet",
	Short: "telnet configuration",
	Long: `Use this command to control and configure telnet.
Enable, Disable, Status, Configure.`,
	Run: func(cmd *cobra.Command, args []string) {
		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			switch f.Name {

			case "enable":
				lib.EnableTelnet()
			case "disable":
				lib.DisableTelnet()
			case "status":
				fmt.Println("telnet: ", lib.GetTelnetStatus())

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
	rootCmd.AddCommand(telnetCmd)
	telnetCmd.Flags().BoolP("enable", "e", false, "enable protocol")
	telnetCmd.Flags().BoolP("disable", "d", false, "disable protocol")
	telnetCmd.Flags().BoolP("status", "s", false, "get protocol status")

}
