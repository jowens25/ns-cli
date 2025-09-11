/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"github.com/spf13/cobra"
)

// statCmd represents the gps command
var statCmd = &cobra.Command{
	Use:   "stat",
	Short: "get status strings",
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		//response := lib.ReadWriteMicro("STAT"+args[0], "GP")
		//fmt.Println(response)

	},
}

func init() {
	rootCmd.AddCommand(statCmd)
	statCmd.Flags().BoolP("all", "a", false, "read all status strings")
	statCmd.Flags().BoolP("channel", "c", false, "")

	statCmd.GroupID = "hw"

}
