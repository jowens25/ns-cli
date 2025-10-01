/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"

	"github.com/spf13/cobra"
)

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:     "ns",
	Short:   "NovuS Configuraton Tool",
	Long:    "Novus Power Products Configuration Tool",
	Version: "1.5.420",
	Run: func(cmd *cobra.Command, args []string) {
		cmd.Help()
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
	//rootCmd.AddGroup(&cobra.Group{ID: "hw", Title: "Hardware Commands"})

}

// AskForConfirmation prompts the user for a yes/no response.
func AskForConfirmation(prompt string) bool {
	reader := bufio.NewReader(os.Stdin)

	for {
		fmt.Printf("%s [y/n]: ", prompt)

		response, err := reader.ReadString('\n')
		if err != nil {
			// handle error
			return false
		}

		response = strings.ToLower(strings.TrimSpace(response))

		if response == "y" || response == "yes" {
			return true
		} else if response == "n" || response == "no" {
			return false
		}
	}
}
