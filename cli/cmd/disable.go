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

// disableCmd represents the disable command
var disableCmd = &cobra.Command{
	Use:   "disable",
	Short: "Disable ports, protocols and interfaces",
	Long: `The disable command can be used to disable
telnet, ssh and http protocols, selected ports and the ethernet interface.`,
	Run: func(cmd *cobra.Command, args []string) {
		hasFlags := false
		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true
			switch f.Name {

			case "all":
				lib.StopTelnet()
				lib.StopSsh()
				lib.StopHttp()
			case "telnet":
				lib.StopTelnet()
			case "ssh":
				lib.StopSsh()
			case "http":
				lib.StopHttp()

			case "web":
				lib.StopApp()

			case "port":
				if len(args) != 0 {
					port := args[0]
					lib.DisablePort(port)
				} else {
					fmt.Println("missing port")

				}

			case "interface":
				if len(args) != 0 {
					intf := args[0]
					lib.DisableInterface(intf)
				} else {
					fmt.Println("missing interface")

				}

			case "sync":
				lib.ToggleNtpSync("no")

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

	disableCmd.Flags().Bool("telnet", false, "disable telnet")
	disableCmd.Flags().Bool("ssh", false, "disable ssh")
	disableCmd.Flags().Bool("http", false, "disable http")
	disableCmd.Flags().Bool("port", false, "disable port")
	disableCmd.Flags().Bool("all", false, "disable insecure protocols")
	disableCmd.Flags().Bool("interface", false, "disable interface")
	disableCmd.Flags().Bool("web", false, "disable web app")
	disableCmd.Flags().Bool("sync", false, "disable ntp synchronization")

}
