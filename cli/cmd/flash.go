/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/api"
	"fmt"

	"github.com/spf13/cobra"
)

// flashCmd represents the flash command
var flashCmd = &cobra.Command{
	Use:   "flash",
	Short: "Save and reset flash",
	Long:  `Use this command to save settings to flash or reset the flash.`,
	Run: func(cmd *cobra.Command, args []string) {

	},
}

// flashCmd represents the flash command
var saveFlashCmd = &cobra.Command{
	Use:   "save",
	Short: "Save settings",
	Long:  `Saves all user settings to flash.`,
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {
			response := api.ReadWriteMicro("SAVEFL", "SAVED")
			fmt.Println(response)

		} else {
			cmd.Help()
		}

	},
}

// flashCmd represents the flash command
var resetFlashCmd = &cobra.Command{
	Use:   "reset",
	Short: "Reset all flash settings to default",
	Long: `Resets all user settings to default values and
overwrites flash memory with defaults.`,
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {
			response := api.ReadWriteMicro("RESETALL", "FLASH")
			fmt.Println(response)

		} else {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(flashCmd)
	flashCmd.AddCommand(resetFlashCmd)
	flashCmd.AddCommand(saveFlashCmd)
}
