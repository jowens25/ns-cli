/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"log"

	"github.com/spf13/cobra"
)

// iptablesCmd represents the iptables command
var ip6tablesRecoverCmd = &cobra.Command{
	Use:   "ip6tables-recover",
	Short: "recover iptables",
	Long:  `Use this command to reset the Ip4 tables.`,
	Run: func(cmd *cobra.Command, args []string) {
		if err := lib.IptablesRecover(); err != nil {
			log.Fatal(err)
		}
	},
}

func init() {
	rootCmd.AddCommand(ip6tablesRecoverCmd)

}
