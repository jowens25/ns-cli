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
	Short: "reset network protocols",
	Long:  `Use this command to reload the default configs that allow all network access`,
	Run: func(cmd *cobra.Command, args []string) {
		lib.Unrestrict()
	},
}

func init() {
	rootCmd.AddCommand(unrestrictCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// unrestrictCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// unrestrictCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
