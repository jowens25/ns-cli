/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"log"
	"os"

	"github.com/spf13/cobra"
)

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:     "ns",
	Short:   "NovuS Configuraton Tool",
	Long:    "Novus Power Products Configuration Tool",
	Version: "1.5.408",
	Run: func(cmd *cobra.Command, args []string) {
	},
}

func Execute() {
	rootCmd.CompletionOptions.DisableDefaultCmd = true

	if _, err := os.Stat("/etc/bash_completion.d/ns.bash"); err != nil {
		bashFile, err := os.Create("/etc/bash_completion.d/ns.bash")
		if err != nil {
			log.Println("unable to create bash completion script: ", err.Error())
		}
		defer bashFile.Close()
		rootCmd.GenBashCompletion(bashFile)
	}

	if err := rootCmd.Execute(); err != nil {
		os.Exit(1)
	}
}

func init() {
	rootCmd.AddGroup(&cobra.Group{ID: "hw", Title: "Hardware Commands"})

}
