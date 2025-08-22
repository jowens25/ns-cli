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
	Args: cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		myInterface := args[0]
		cmd.Flags().Visit(func(f *pflag.Flag) {

			switch f.Name {
			case "enable":
				lib.EnableInterface(myInterface)
			case "disable":
				lib.DisableInterface(myInterface)
			case "phy":
				fmt.Println(lib.GetInterfacePhysicalStatus(myInterface))
			default:
				fmt.Println(lib.GetInterfaceNetworkStatus(myInterface))

			}

		})

	},
}

func init() {
	rootCmd.AddCommand(interfaceCmd)

	interfaceCmd.Flags().BoolP("enable", "e", false, "enable network interface")
	interfaceCmd.Flags().BoolP("disable", "d", false, "disable network interface")
	interfaceCmd.Flags().BoolP("phy", "p", false, "network interface (physical) status")

}
