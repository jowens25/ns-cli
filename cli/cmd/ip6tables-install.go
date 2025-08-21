/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"log"

	"github.com/spf13/cobra"
)

// ip6tablesCmd represents the ip6tables command
var ip6tablesInstallCmd = &cobra.Command{
	Use:   "iptables-install",
	Short: "install iptables",
	Long:  `Use this command to reset the Ip6 tables.`,
	Run: func(cmd *cobra.Command, args []string) {
		if err := lib.Ip6tablesInstall(); err != nil {
			log.Fatal(err)
		}
	},
}

func init() {
	rootCmd.AddCommand(ip6tablesInstallCmd)

}
