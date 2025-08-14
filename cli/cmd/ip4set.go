/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/api"
	"fmt"

	"github.com/spf13/cobra"
)

// ip4setCmd represents the ip4set command
var ip4setCmd = &cobra.Command{
	Use:   "ip4set",
	Short: "ip4set ip gw",
	Long: `Use this command to update the ip address and/or gateway 
in /etc/network/interfaces ip4set <ip>, ip4set <ip> <gw>`,
	Run: func(cmd *cobra.Command, args []string) {
		if len(args) == 0 {
			cmd.Help()
		} else if len(args) == 1 {
			api.SetIpAndGw(args[0])

		} else if len(args) == 2 {
			api.SetIpAndGw(args[0], args[1])
		}
		fmt.Println("Please reboot for changes to take effect")

	},
}

func init() {
	rootCmd.AddCommand(ip4setCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// ip4setCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// ip4setCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
