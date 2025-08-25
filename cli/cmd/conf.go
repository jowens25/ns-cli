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

// confCmd represents the conf command
var confCmd = &cobra.Command{
	Use:   "conf",
	Short: "configuration overview",
	Long:  `Use this command can be used to view network config, and others.`,
	Run: func(cmd *cobra.Command, args []string) {

		hasFlags := false
		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			switch f.Name {

			case "network":
				fmt.Println("Hostname:     ", lib.GetHostname())
				fmt.Println("Ip address:   ", lib.GetIPv4Address("enp3s0"))
				fmt.Println("Gateway:      ", lib.GetGateway("enp3s0"))
				fmt.Println("Interface:    ", lib.GetPortPhysicalStatus("enp3s0"))
				fmt.Println("MAC:          ", lib.GetMacAddress("enp3s0"))
				fmt.Println("IPv4 address: ", lib.GetIPv4Address("enp3s0"))

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
	rootCmd.AddCommand(confCmd)

	confCmd.Flags().BoolP("network", "n", false, "network overview")

}
