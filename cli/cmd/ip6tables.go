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
var ip6tablesCmd = &cobra.Command{
	Use:   "ip6tables",
	Short: "ip6tables management",
	Long:  `Use this command to save or reset the ip6tables`,
	Run: func(cmd *cobra.Command, args []string) {

		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			switch f.Name {
			case "recover":
				if err := lib.Ip6tablesRecover(); err != nil {
					log.Fatal(err)
				}
			case "install":
				if err := lib.Ip6tablesInstall(); err != nil {
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
	rootCmd.AddCommand(ip6tablesCmd)

	ip6tablesCmd.Flags().BoolP("recover", "r", false, "reset ip6tables to default")
	ip6tablesCmd.Flags().BoolP("install", "i", false, "make ip6table changes persist")
}
