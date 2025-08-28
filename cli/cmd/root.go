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
	Long:    "Novus Power Products Configuration Tool.",
	Version: "0.8.1",
	Run: func(cmd *cobra.Command, args []string) {
	},
}

func Execute() {
	rootCmd.CompletionOptions.DisableDefaultCmd = true

	bashFile, err := os.Create("/etc/bash_completion.d/ns.bash")

	if err != nil {
		log.Fatal("unable to open bash completion location")
	}
	defer bashFile.Close()

	rootCmd.Root().GenBashCompletion(bashFile)

	err = rootCmd.Execute()
	if err != nil {
		os.Exit(1)
	}
}

func init() {

}
