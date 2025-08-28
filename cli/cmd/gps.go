/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

// gpsCmd represents the gps command
var gpsCmd = &cobra.Command{
	Use:   "gps",
	Short: "get gps strings",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		response := lib.ReadWriteMicro("STAT"+args[0], "STAT"+args[0])
		fmt.Println(response)

	},
}

func init() {
	rootCmd.AddCommand(gpsCmd)

	gpsCmd.GroupID = "hw"

}
