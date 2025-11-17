/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

// ntlCmd represents the ntl command
var ntlCmd = &cobra.Command{
	Use:   "ntl",
	Short: "A brief description of your command",

	Run: func(cmd *cobra.Command, args []string) {

	},
}

// loadCmd represents the load command
var loadCmd = &cobra.Command{
	Use:   "load",
	Short: "load fpga config file",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		lib.LoadConfig(args[0])
	},
}

var clkCmd = &cobra.Command{
	Use:   "clk",
	Short: "clock module",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		if args[0] == "all" {
			lib.ReadAllClk()
			return
		}

		rsp, err := lib.ReadNtlProperty("clk", args[0])

		if len(args) > 1 {
			rsp, err = lib.WriteNtlProperty("clk", args[0], args[1])
		}
		if err != nil {
			fmt.Println(err.Error())
		}
		fmt.Println(rsp[0], rsp[1], rsp[2])

	},
}

var ptpCmd = &cobra.Command{
	Use:   "ptp",
	Short: "ptp module",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		if args[0] == "all" {
			lib.ReadAllPtp()
			return
		}

		rsp, err := lib.ReadNtlProperty("ptp", args[0])

		if len(args) > 1 {
			rsp, err = lib.WriteNtlProperty("ptp", args[0], args[1])
		}
		if err != nil {
			fmt.Println(err.Error())
		}
		fmt.Println(rsp[0], rsp[1], rsp[2])

	},
}

var ntpCmd = &cobra.Command{
	Use:   "ntp",
	Short: "network time protocol module",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		if args[0] == "all" {
			lib.ReadAllNtp()
			return
		}

		rsp, err := lib.ReadNtlProperty("ntp", args[0])

		if len(args) > 1 {
			rsp, err = lib.WriteNtlProperty("ntp", args[0], args[1])
		}
		if err != nil {
			fmt.Println(err.Error())
		}
		fmt.Println(rsp[0], rsp[1], rsp[2])

	},
}

var ppsCmd = &cobra.Command{
	Use:   "pps",
	Short: "pps module",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		if args[0] == "all" {
			lib.ReadAllPps()
			return
		}

		rsp, err := lib.ReadNtlProperty("pps", args[0])

		if len(args) > 1 {
			rsp, err = lib.WriteNtlProperty("pps", args[0], args[1])
		}
		if err != nil {
			fmt.Println(err.Error())
		}
		fmt.Println(rsp[0], rsp[1], rsp[2])

	},
}

var todCmd = &cobra.Command{
	Use:   "tod",
	Short: "time of day module",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		if args[0] == "all" {
			lib.ReadAllTod()
			return
		}

		rsp, err := lib.ReadNtlProperty("tod", args[0])

		if len(args) > 1 {
			rsp, err = lib.WriteNtlProperty("tod", args[0], args[1])
		}
		if err != nil {
			fmt.Println(err.Error())
		}
		fmt.Println(rsp[0], rsp[1], rsp[2])

	},
}

func init() {
	rootCmd.AddCommand(ntlCmd)
	ntlCmd.AddCommand(loadCmd)
	rootCmd.AddCommand(ntpCmd)
	rootCmd.AddCommand(clkCmd)
	rootCmd.AddCommand(ppsCmd)
	rootCmd.AddCommand(ptpCmd)
	rootCmd.AddCommand(todCmd)

}
