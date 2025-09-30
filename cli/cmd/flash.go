/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

// flashCmd represents the flash command
var flashCmd = &cobra.Command{
	Use:   "flash",
	Short: "save and reset flash",
	Run: func(cmd *cobra.Command, args []string) {
		cmd.Help()

	},
}

// flashCmd represents the flash command
var saveFlashCmd = &cobra.Command{
	Use:   "save",
	Short: "save settings to flash",
	Run: func(cmd *cobra.Command, args []string) {

		response, _ := lib.ReadWriteMicro("$SAVEFL")
		fmt.Println(response)

	},
}

// flashCmd represents the flash command
var resetFlashCmd = &cobra.Command{
	Use:   "reset",
	Short: "reset settings to default and save to flash",
	Run: func(cmd *cobra.Command, args []string) {

		if !lib.IsAdminRoot() {
			fmt.Println("requires admin access")
			return
		}

		if AskForConfirmation("Are you sure you want to reset all flash variables?") {
			response, _ := lib.ReadWriteMicro("$RESETALL")
			fmt.Println(response)
			fmt.Println("flash reset")
			return
		}

		fmt.Println("canceled")

	},
}

func init() {
	rootCmd.AddCommand(flashCmd)
	flashCmd.AddCommand(resetFlashCmd)
	flashCmd.AddCommand(saveFlashCmd)
	flashCmd.GroupID = "hw"

}
