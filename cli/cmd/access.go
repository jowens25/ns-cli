/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"
	"net"

	"github.com/spf13/cobra"
)

// accessCmd represents the access command
var accessCmd = &cobra.Command{
	Use:   "access",
	Short: "define network access",
	Long: `Use this command to set the network level access to the system. 
	ex. 10.1.10.220/32 or 10.1.10.0/24.`,
	Args: cobra.ExactArgs(1),
}

var addCmd = &cobra.Command{
	Use:   "add [ip address]",
	Short: "add a node",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		ipAddr, ipNet, err := net.ParseCIDR(args[0])
		if err != nil {
			fmt.Println("please enter address in CIDR form. Ex. 10.1.10.1/24, 10.1.10.1/32")
			return
		}
		fmt.Printf("adding access for %s with mask %s\n", ipAddr.String(), net.IP(ipNet.Mask).String())
		lib.AddAccessToFiles(args[0])

	},
}

var removeCmd = &cobra.Command{
	Use:   "remove [ip address]",
	Short: "remove a node",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		lib.RemoveAccessFromFiles(args[0])

	},
}

var unrestrictCmd = &cobra.Command{
	Use:   "unrestrict",
	Short: "reset network restrictions",
	Run: func(cmd *cobra.Command, args []string) {
		lib.Unrestrict()
	},
}

var showCmd = &cobra.Command{
	Use:   "show",
	Short: "show network restrictions",
	Run: func(cmd *cobra.Command, args []string) {
		lib.readXinetdAllowedNodes()
	},
}

func init() {
	rootCmd.AddCommand(accessCmd)
	accessCmd.AddCommand(unrestrictCmd)
	accessCmd.AddCommand(addCmd)
	accessCmd.AddCommand(removeCmd)
	accessCmd.AddCommand(showCmd)

}
