/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/api"
	"fmt"

	"github.com/spf13/cobra"
)

// baudrateCmd represents the baudrate command
var baudrateCmd = &cobra.Command{
	Use:   "baudrate <baudrates>",
	Short: "RS232 Rear Panel Baud Rate",
	Long: `Use this command to assign and query the baud 
rate on rear panel RS232. (Default = 115200). Available 
baudrates are 19200, 38400, 57600, 115200, 230400.`,
	ValidArgs:             []string{"19200", "38400", "57600", "115200", "230400"},
	DisableFlagsInUseLine: true, // This hides [flags] from the usage line

	Example: `
  # Common usage patterns
  baudrate			# return current rate
  baudrate <rate>		# sets new rate`,

	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {

			response := api.ReadWriteMicro("BAUDNV", "BAUDNV")
			fmt.Println(response)

		} else if len(args) == 1 {
			response := api.ReadWriteMicro("BAUDNV", "BAUDNV", args[0])
			fmt.Println(response)
		} else {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(baudrateCmd)
}
