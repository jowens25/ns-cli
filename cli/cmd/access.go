/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"

	"github.com/spf13/cobra"
)

// accessCmd represents the access command
var accessCmd = &cobra.Command{
	Use:   "access",
	Short: "network access",
	Long: `Use this command to set the network level access to the system. 
	ex. 10.1.10.220/32 or 10.1.10.0/24.`,
	Args: cobra.ExactArgs(1),
}

var addCmd = &cobra.Command{
	Use:   "add [ip address]",
	Short: "add a node",
	Long:  `Use this command to add an additional allows ip address.`,
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		lib.AddAccess(args[0])
		lib.AddNginxAccess(args[0])
	},
}

var removeCmd = &cobra.Command{
	Use:   "remove [ip address]",
	Short: "remove a node",
	Long:  `Use this command to remove an additional allows ip address.`,
	Args:  cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		lib.RemoveAccess(args[0])
		lib.RemoveNginxAccess(args[0])
	},
}

var unrestrictCmd = &cobra.Command{
	Use:   "unrestrict",
	Short: "reset network protocols",
	Long:  `Use this command to reload the default configs that allow all network access`,
	Run: func(cmd *cobra.Command, args []string) {
		lib.Unrestrict()
	},
}

func init() {
	rootCmd.AddCommand(accessCmd)
	accessCmd.AddCommand(unrestrictCmd)
	accessCmd.AddCommand(addCmd)
	accessCmd.AddCommand(removeCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// accessCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// accessCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
