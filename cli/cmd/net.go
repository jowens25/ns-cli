package cmd

import (
	"github.com/spf13/cobra"
)

var networkCmd = &cobra.Command{
	Use:   "net",
	Short: "network configuraton",
	Long:  `Use this command to configure ip address, gateway, connection status and more.`,
	Run: func(cmd *cobra.Command, args []string) {
	},
}

func init() {
	rootCmd.AddCommand(networkCmd)
}
