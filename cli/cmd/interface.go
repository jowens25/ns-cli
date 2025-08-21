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
var interfaceCmd = &cobra.Command{
	Use:   "interface",
	Short: "network interface",
	Long: `Use this command to enable or disable network ports
and see their status.`,
	Run: func(cmd *cobra.Command, args []string) {

		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			if len(args) > 0 {

				switch f.Name {

				case "enable":
					lib.EnableInterface(args[0])
				case "disable":
					lib.DisableInterface(args[0])
				case "status":
					fmt.Println(lib.GetInterfaceNetworkStatus(args[0]))
				case "phy":
					fmt.Println(lib.GetInterfacePhysicalStatus(args[0]))

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
	rootCmd.AddCommand(interfaceCmd)

	interfaceCmd.Flags().BoolP("enable", "e", false, "enable network interface")
	interfaceCmd.Flags().BoolP("disable", "d", false, "disable network interface")
	interfaceCmd.Flags().BoolP("status", "s", false, "network interface status")
	interfaceCmd.Flags().BoolP("phy", "p", false, "network interface (physical) status")

}
