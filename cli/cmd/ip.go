/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

// ip4setCmd represents the ip4set command
var ip = &cobra.Command{
	Use:   "ip",
	Short: "IP4V Address",
	Long: `Use this command to get or set the ip address and/or gateway 
in /etc/network/interfaces ip set <ip>, ip set <ip> <gw>`,
	Run: func(cmd *cobra.Command, args []string) {
		//if len(args) == 0 {
		cmd.Help()
		//} else if len(args) == 1 {
		//	lib.SetIpAndGw(args[0])
		//
		//} else if len(args) == 2 {
		//	lib.SetIpAndGw(args[0], args[1])
		//}
		//fmt.Println("Please reboot for changes to take effect")

	},
}

// ip4setCmd represents the ip4set command
var get = &cobra.Command{
	Use:   "get",
	Short: "get device ip address and gateway",
	Long: `Use this command to get the ip address and/or gateway 
in /etc/network/interfaces ip get`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println(lib.GetIpAndGw())

	},
}

// ip4setCmd represents the ip4set command
var set = &cobra.Command{
	Use:   "set",
	Short: "set device ip address and gateway",
	Long: `Use this command to set the ip address and/or gateway 
in /etc/network/interfaces ip set <ip>, ip set <ip> <gw>`,
	Run: func(cmd *cobra.Command, args []string) {
		if len(args) == 0 {
			cmd.Help()
			return
		} else if len(args) == 1 {
			lib.SetIpAndGw(args[0])

		} else if len(args) == 2 {
			lib.SetIpAndGw(args[0], args[1])
		}
		fmt.Println("Please reboot for changes to take effect")

	},
}

func init() {
	rootCmd.AddCommand(ip)
	ip.AddCommand(get)
	ip.AddCommand(set)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// ip4setCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// ip4setCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
