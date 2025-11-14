/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"

	"github.com/spf13/cobra"
)

// ntlCmd represents the ntl command
var ntlCmd = &cobra.Command{
	Use:   "ntl",
	Short: "A brief description of your command",

	Run: func(cmd *cobra.Command, args []string) {

	},
}

// cfgCmd represents the cfg command
var loadCmd = &cobra.Command{
	Use:   "load",
	Short: "load fpga config file",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		lib.LoadConfig(args[0])
	},
}

func init() {
	rootCmd.AddCommand(ntlCmd)
	ntlCmd.AddCommand(loadCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// ntlCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// ntlCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
