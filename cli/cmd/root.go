/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"os"

	"github.com/spf13/cobra"
)

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:     "ns",
	Short:   "NovuS Configuraton Tool",
	Long:    "Novus Power Products Configuration Tool.",
	Version: "0.8.1",
	Run: func(cmd *cobra.Command, args []string) {
	},
}

func Execute() {
	rootCmd.CompletionOptions.DisableDefaultCmd = true

	err := rootCmd.Execute()
	if err != nil {
		os.Exit(1)
	}
}

func init() {

}
