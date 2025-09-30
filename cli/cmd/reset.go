/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

// resetCmd represents the reset command
var resetCmd = &cobra.Command{
	Use:   "reset",
	Short: "reset unit to factory defaults",

	Run: func(cmd *cobra.Command, args []string) {

		if !lib.IsAdminRoot() {
			fmt.Println("requires admin access")
			return
		}

		if AskForConfirmation("Are you sure you want to reset the unit?") {
			lib.StopApp()
			lib.CopyConfigs()
			lib.ResetUsers()
			lib.ResetNetworkConfig(lib.AppConfig.Network.Interface)
		}

	},
}

func init() {
	rootCmd.AddCommand(resetCmd)

}
