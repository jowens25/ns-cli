/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"

	//"github.com/jowens25/axi"

	"github.com/spf13/cobra"
)

// ipCmd represents the ip command
var ntpIp = &cobra.Command{
	Use:   "ip",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("ip called from ", cmd.Parent().Name())

		//axi.ToggleNtpIpMode(args[0])
		//axi.SetNtpIpAddress(args[1])
		//axi.SetNtpReferenceId(args[0])
		// axi.SetNtpSmearingStatus(args[0])
		//axi.SetNtpLeap61Status(args[0])
		//axi.SetNtpLeap59Status(args[0])
		//axi.SetNtpOffsetStatus(args[0])
		//axi.SetNtpOffsetValue(args[0])
		//axi.ClearNtpCounters(args[0])
		//axi.WriteNtpServer("clearcounters", args[0])
	},
}

// ipCmd represents the ip command
var ptpIp = &cobra.Command{
	Use:   "ip",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("ip called from ", cmd.Parent().Name())
	},
}

func init() {
	ntpCmd.AddCommand(ntpIp)
	ptpCmd.AddCommand(ptpIp)
	ntpIp.Flags().BoolP("mode", "m", false, "use")
	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// ipCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// ipCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
