package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var serviceCmd = &cobra.Command{
	Use:   "service [protocol]",
	Short: "ssh, ftp, telnet, http",
	Long:  `Use this command to enable and disable insecure protocols.`,
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		switch args[0] {
		case "ssh":
			fmt.Println(lib.GetSshStatus())

		case "ftp":
			fmt.Println(lib.GetFtpStatus())

		case "telnet":
			fmt.Println(lib.GetTelnetStatus())
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
			lib.EnableSsh()
		case "ftp":
			lib.EnableFtp()
		case "telnet":
			lib.EnableTelnet()
		case "http":
		case "app":
			lib.StartApp()
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
			lib.DisableFtp()
		case "telnet":
			lib.DisableTelnet()
		case "http":
		case "app":
			lib.StopApp()
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
