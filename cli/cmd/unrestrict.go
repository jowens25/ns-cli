/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"

	"github.com/spf13/cobra"
)

// unrestrictCmd represents the unrestrict command
var unrestrictCmd = &cobra.Command{
	Use:   "unrestrict",
	Short: "clear restriction table",
	Long:  `clear the network access restriction table`,
	Run: func(cmd *cobra.Command, args []string) {
		lib.Unrestrict()
	},
}

func init() {
	rootCmd.AddCommand(unrestrictCmd)

}
