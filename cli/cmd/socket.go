/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

// socketCmd represents the socket command
var socketCmd = &cobra.Command{
	Use:   "socket",
	Short: "A brief description of your command",
	Args:  cobra.MinimumNArgs(1),
	Run: func(cmd *cobra.Command, args []string) {

		command := args[0]

		response := "default response"
		response, err := lib.ReadWriteSocket(command)

		fmt.Println(response)

		if err != nil {
			fmt.Println("serial error")
			return
		}

	},
}

func init() {
	rootCmd.AddCommand(socketCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// socketCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// socketCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
