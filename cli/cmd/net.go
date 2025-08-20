package cmd

import (
	"github.com/spf13/cobra"
)

var networkCmd = &cobra.Command{
	Use:   "net",
	Short: "Display network configuration",
	Long:  `Use this command to see a network configuration overview.`,
	Run: func(cmd *cobra.Command, args []string) {
	},
}

func init() {
	rootCmd.AddCommand(networkCmd)
}
