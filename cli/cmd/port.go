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

// portCmd represents the port command
var portCmd = &cobra.Command{
	Use:   "port",
	Short: "network port",
	Long:  `Use this command to enable or disable specific ports.`,
	Run: func(cmd *cobra.Command, args []string) {

		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			if len(args) > 0 {

				switch f.Name {

				case "enable":
					lib.EnablePort(args[0])
				case "disable":
					lib.DisablePort(args[0])
				case "status":
					fmt.Println(lib.GetPortStatus(args[0]))

				default:
					cmd.Help()
				}
			} else {
				fmt.Println("please enter an interface (eth0)")
			}
		})

		if !hasFlags {
			cmd.Help()
		}
	},
}

func init() {
	rootCmd.AddCommand(portCmd)

	portCmd.Flags().BoolP("enable", "e", false, "enable network port")
	portCmd.Flags().BoolP("disable", "d", false, "disable network port")
	portCmd.Flags().BoolP("status", "s", false, "network port status")
}
