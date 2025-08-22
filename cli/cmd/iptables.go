/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"log"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

// iptablesCmd represents the iptables command
var iptablesCmd = &cobra.Command{
	Use:   "iptables",
	Short: "iptables management",
	Long:  `Use this command to save or reset the iptables`,
	Run: func(cmd *cobra.Command, args []string) {

		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			switch f.Name {
			case "recover":
				if err := lib.IptablesRecover(); err != nil {
					log.Fatal(err)
				}
			case "install":
				if err := lib.IptablesInstall(); err != nil {
					log.Fatal(err)
				}

			default:
			}
		})

		if !hasFlags {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(iptablesCmd)

	iptablesCmd.Flags().BoolP("recover", "r", false, "reset ip tables to default")
	iptablesCmd.Flags().BoolP("install", "i", false, "make ip table changes persist")
}
