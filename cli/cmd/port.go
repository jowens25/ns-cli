package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var portCmd = &cobra.Command{
	Use:   "port",
	Short: "network port",
	Long:  `Use this command to manage network ports and see their status.`,
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		if phy {
			fmt.Println(lib.GetPortPhysicalStatus(args[0]))
		} else {
			fmt.Println(lib.GetPortConnectionStatus(args[0]))

		}
	},
}

var enableCmd = &cobra.Command{
	Use:   "enable [port]",
	Short: "Enable a network connection",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		lib.PortConnect(args[0])
	},
}

var disableCmd = &cobra.Command{
	Use:   "disable [port]",
	Short: "Disable a network connection",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		lib.PortDisconnect(args[0])
	},
}

var phy bool

func init() {

	rootCmd.AddCommand(portCmd)
	portCmd.AddCommand(enableCmd)
	portCmd.AddCommand(disableCmd)
	portCmd.Flags().BoolVarP(&phy, "phy", "p", false, "show physical status")

}
