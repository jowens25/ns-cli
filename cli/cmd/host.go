/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

// hostCmd represents the host command
var hostCmd = &cobra.Command{
	Use:   "host",
	Short: "get hostname",
	Long:  `Use this command to get and set the hostname.`,
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {
			response := lib.GetHostname()
			fmt.Print(response)

		} else if len(args) == 1 {
			lib.SetHostname(args[0])
			response := lib.GetHostname()
			fmt.Print(response)

		} else {
			cmd.Help()
		}
	},
}

func init() {
	rootCmd.AddCommand(hostCmd)

}
