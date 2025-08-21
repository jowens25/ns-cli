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
var iptablesRecoverCmd = &cobra.Command{
	Use:   "iptables-recover",
	Short: "recover iptables",
	Long:  `Use this command to reset the Ip4 tables.`,
	Run: func(cmd *cobra.Command, args []string) {
		if err := lib.IptablesRecover(); err != nil {
			log.Fatal(err)
		}
	},
}

func init() {
	rootCmd.AddCommand(iptablesRecoverCmd)

}
