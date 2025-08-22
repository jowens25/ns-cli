package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var interfaceCmd = &cobra.Command{
	Use:   "int",
	Short: "network interface",
	Long:  `Use this command to manage network ports and see their status.`,
}

var enableCmd = &cobra.Command{
	Use:   "up [interface]",
	Short: "Enable a network interface",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		lib.EnableInterface(args[0])
	},
}

var disableCmd = &cobra.Command{
	Use:   "down [interface]",
	Short: "Disable a network interface",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		lib.DisableInterface(args[0])
	},
}

var statusCmd = &cobra.Command{
	Use:   "stat [interface]",
	Short: "Show network interface status",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		if phy {
			fmt.Println(lib.GetInterfacePhysicalStatus(args[0]))
		} else {
			fmt.Println(lib.GetInterfaceNetworkStatus(args[0]))

		}

	},
}

var listCmd = &cobra.Command{
	Use:   "ls",
	Short: "List network interfaces",
	Args:  cobra.ExactArgs(0),
	Run: func(cmd *cobra.Command, args []string) {

		fmt.Println(lib.GetInterfaces())

	},
}

var phy bool

func init() {

	rootCmd.AddCommand(interfaceCmd)
	interfaceCmd.AddCommand(listCmd)
	interfaceCmd.AddCommand(enableCmd)
	interfaceCmd.AddCommand(disableCmd)
	interfaceCmd.AddCommand(statusCmd)
	statusCmd.Flags().BoolVarP(&phy, "phy", "p", false, "show physical status")

}
