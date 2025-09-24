package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var serviceCmd = &cobra.Command{
	Use:   "service [protocol]",
	Short: "manage system services",
}

var serviceEnableCmd = &cobra.Command{
	Use:       "enable [protocol]",
	Short:     "Enable a network protocol",
	Args:      cobra.MinimumNArgs(1),
	ValidArgs: []string{"all", "ssh", "ftp", "telnet", "http", "dhcp", "dhcp6", "app", "snmp"},
	Run: func(cmd *cobra.Command, args []string) {

		switch args[0] {
		case "ssh":
			lib.EnableSsh()
		case "snmp":
			lib.StartSnmpd()
		case "ftp":
			lib.EnableFtp()
		case "telnet":
			lib.EnableTelnet()
		case "http":
			lib.EnableHttp()
		case "app":
			lib.StartApp()
		default:
			cmd.Help()
		}
	},
}

var serviceDisableCmd = &cobra.Command{
	Use:       "disable [protocol]",
	Short:     "Disable a network protocol",
	Args:      cobra.MinimumNArgs(1),
	ValidArgs: []string{"all", "ssh", "ftp", "telnet", "http", "dhcp", "dhcp6", "app", "snmp"},

	Run: func(cmd *cobra.Command, args []string) {

		switch args[0] {
		case "ssh":
			lib.DisableSsh()
		case "snmp":
			lib.StopSnmpd()
		case "ftp":
			lib.DisableFtp()
		case "telnet":
			lib.DisableTelnet()
		case "http":
			lib.DisableHttp()
		case "app":
			lib.StopApp()
		default:
			cmd.Help()
		}
	},
}

var serviceStatusCmd = &cobra.Command{
	Use:       "status [protocol | all]",
	Short:     "Get status of a network protocol",
	Args:      cobra.MinimumNArgs(1),
	ValidArgs: []string{"all", "ssh", "ftp", "telnet", "http", "dhcp", "dhcp6", "app", "snmp"},

	Run: func(cmd *cobra.Command, args []string) {

		switch args[0] {
		case "all":
			fmt.Println("ftp:    ", lib.GetFtpStatus())
			fmt.Println("telnet: ", lib.GetTelnetStatus())
			fmt.Println("ssh:    ", lib.GetSshStatus())
			fmt.Println("http:   ", lib.GetHttpStatus())
			fmt.Println("snmp:   ", lib.GetSnmpdStatus())

		case "ssh":
			fmt.Println("ssh:    ", lib.GetSshStatus())
		case "ftp":
			fmt.Println("ftp:    ", lib.GetFtpStatus())
		case "telnet":
			fmt.Println("telnet: ", lib.GetTelnetStatus())
		case "http":
			fmt.Println("http:   ", lib.GetHttpStatus())
		case "snmp":
			fmt.Println("snmp:   ", lib.GetSnmpdStatus())

		default:
			cmd.Help()
		}
	},
}

func init() {
	rootCmd.AddCommand(serviceCmd)
	serviceCmd.AddCommand(serviceEnableCmd)
	serviceCmd.AddCommand(serviceDisableCmd)
	serviceCmd.AddCommand(serviceStatusCmd)
}
