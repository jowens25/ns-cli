/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"github.com/spf13/cobra"
)

// flashCmd represents the flash command
var flashCmd = &cobra.Command{
	Use:   "flash",
	Short: "onboard flash",
	Run: func(cmd *cobra.Command, args []string) {

	},
}

// flashCmd represents the flash command
var saveFlashCmd = &cobra.Command{
	Use:   "save",
	Short: "save settings to flash",
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {
			//response := lib.ReadWriteMicro("SAVEFL", "SAVED")
			//fmt.Println(response)

		} else {
			cmd.Help()
		}

	},
}

// flashCmd represents the flash command
var resetFlashCmd = &cobra.Command{
	Use:   "reset",
	Short: "reset settings to default and save to flash",
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {
			//response := lib.ReadWriteMicro("RESETALL", "RESETALL")
			//fmt.Println(response)

		} else {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(flashCmd)
	flashCmd.AddCommand(resetFlashCmd)
	flashCmd.AddCommand(saveFlashCmd)
	flashCmd.GroupID = "hw"

}
