/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

/*
#include "serialInterface.h"
#include "axi.h"
*/
import (
	"fmt"

	"github.com/jowens25/axi"
	"github.com/spf13/cobra"
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
		fmt.Println("device called")
		//C.connect()
		axi.RunConnect()
	},
}

func init() {
	rootCmd.AddCommand(deviceCmd)

	deviceCmd.Flags().StringP("load", "l", "", "load a config file")
	deviceCmd.Flags().StringP("dump", "d", "", "dump a config file")
	deviceCmd.Flags().BoolP("connect", "c", false, "attempt to connect to FPGA")
	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// deviceCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// deviceCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
