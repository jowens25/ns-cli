/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

// baudrateCmd represents the baudrate command
var baudrateCmd = &cobra.Command{
	Use:                   "baud <rate>",
	Short:                 "get and set baudrate (rear panel serial port)",
	ValidArgs:             []string{"19200", "38400", "57600", "115200", "230400"},
	DisableFlagsInUseLine: true, // This hides [flags] from the usage line

	Example: `baud <rate> 	# sets new rate`,

	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {

			response, _ := lib.ReadWriteMicro("$BAUDNV")

			fmt.Println(response)

		} else if len(args) == 1 {

			if !lib.IsAdminRoot() {
				fmt.Println("requires admin access")
				return
			}

			response, _ := lib.ReadWriteMicro("$BAUDNV=" + args[0])
			fmt.Println(response)

		} else {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(baudrateCmd)
	baudrateCmd.GroupID = "hw"
}
