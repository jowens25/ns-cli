/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"

	"github.com/spf13/cobra"
)

// serviceCmd represents the service command
var serviceCmd = &cobra.Command{
	Use:   "service [protocol]",
	Short: "ssh, ftp, telnet, http",
	Long:  `Use this command to enable and disable insecure protocols.`,
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		switch args[0] {
		case "ssh":
		case "ftp":
		case "telnet":
			lib.GetTelnetStatus()
		case "http":
		default:
			cmd.Help()
		}

	},
}

var serviceEnableCmd = &cobra.Command{
	Use:   "enable [protocol]",
	Short: "Enable a network protocol",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		switch args[0] {
		case "ssh":
		case "ftp":
		case "telnet":
			lib.EnableTelnet()
		case "http":
		default:
			cmd.Help()
		}
	},
}

var serviceDisableCmd = &cobra.Command{
	Use:   "disable [protocol]",
	Short: "Disable a network protocol",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		switch args[0] {
		case "ssh":
			lib.DisableSsh()
		case "ftp":
		case "telnet":
			lib.DisableTelnet()
		case "http":
		default:
			cmd.Help()
		}
	},
}

func init() {
	rootCmd.AddCommand(serviceCmd)
	serviceCmd.AddCommand(serviceEnableCmd)
	serviceCmd.AddCommand(serviceDisableCmd)
}
