/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

/*
#include "serialInterface.h"
#include "axi.h"
*/
import (
	"NovusTimeServer/axi"
	"fmt"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

// deviceCmd represents the device command
var deviceCmd = &cobra.Command{
	Use:     "device",
	Aliases: []string{"d"},
	Short:   "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		cmd.Flags().Visit(func(f *pflag.Flag) {

			switch f.Name {
			case "load":
				axi.LoadConfig(args[0])

			case "connect":
				fmt.Println("connect cmd called")
				fmt.Println(axi.Connect())

			case "read":
				fmt.Println("read config")
				fmt.Println(axi.Connect())

				fmt.Println(axi.GetCores())

			default:
				fmt.Println("only load works right now")
			}
		})

	},
}

func init() {
	rootCmd.AddCommand(deviceCmd)

	deviceCmd.Flags().BoolP("load", "l", false, "load a config file")
	deviceCmd.Flags().BoolP("dump", "d", false, "dump a config file")
	deviceCmd.Flags().BoolP("connect", "c", false, "attempt to connect to FPGA")
	deviceCmd.Flags().BoolP("read", "r", false, "read config")
	deviceCmd.Flags().BoolP("reset", "s", false, "reset device")

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// deviceCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// deviceCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
