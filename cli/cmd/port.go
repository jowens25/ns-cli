package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var portCmd = &cobra.Command{
	Use:   "port [port]",
	Short: "ethernet settings",
}

var portStatusCmd = &cobra.Command{
	Use:   "status [port]",
	Short: "ethernet port",
	Long:  `Use this command to get ethernet port status.`,
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		if !lib.HasInterface(args[0]) {
			return
		} else {
			fmt.Println("ethernet:", lib.GetPortPhysicalStatus(args[0]))
			fmt.Println("network:", lib.GetPortConnectionStatus(args[0]))
		}

	},
}

var portEnableCmd = &cobra.Command{
	Use:   "enable [port]",
	Short: "Enable a network connection",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		lib.PortConnect(args[0])
	},
}

var portDisableCmd = &cobra.Command{
	Use:   "disable [port]",
	Short: "Disable a network connection",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		lib.PortDisconnect(args[0])
	},
}

func init() {

	rootCmd.AddCommand(portCmd)
	portCmd.AddCommand(portEnableCmd)
	portCmd.AddCommand(portDisableCmd)
	portCmd.AddCommand(portStatusCmd)
}
