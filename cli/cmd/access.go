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
	Long:  `get and set network level access to the system`,
	Args:  cobra.ExactArgs(1),
}

var addCmd = &cobra.Command{
	Use:   "add [cidr address]",
	Short: "add a node",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		ipAddr, ipNet, err := net.ParseCIDR(args[0])
		if err != nil {
			fmt.Println("please enter valid address in CIDR form. Ex. 10.1.10.1/24, 10.1.10.1/32")
			return
		}
		fmt.Printf("adding access for %s with mask %s\n", ipAddr.String(), net.IP(ipNet.Mask).String())
		lib.AddAccessToFiles(args[0])

		fmt.Println("allowed nodes")
		for _, node := range lib.ReadAccessFromFiles() {
			fmt.Println(node)
		}

	},
}

var removeCmd = &cobra.Command{
	Use:   "remove [cidr address]",
	Short: "remove a node",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		ipAddr, ipNet, err := net.ParseCIDR(args[0])
		if err != nil {
			fmt.Println("please enter address in CIDR form. Ex. 10.1.10.1/24, 10.1.10.1/32")
			return
		}
		fmt.Printf("removing access for %s with mask %s\n", ipAddr.String(), net.IP(ipNet.Mask).String())
		lib.RemoveAccessFromFiles(args[0])

		fmt.Println("allowed nodes")
		for _, node := range lib.ReadAccessFromFiles() {
			fmt.Println(node)
		}

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
	Short: "show allowed nodes",
	Run: func(cmd *cobra.Command, args []string) {

		fmt.Println("allowed nodes")
		for _, node := range lib.ReadAccessFromFiles() {
			fmt.Println(node)
		}
	},
}

func init() {
	rootCmd.AddCommand(accessCmd)
	accessCmd.AddCommand(unrestrictCmd)
	accessCmd.AddCommand(addCmd)
	accessCmd.AddCommand(removeCmd)
	accessCmd.AddCommand(showCmd)

}
