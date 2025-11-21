/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

var readNtp = &cobra.Command{
	Use: "read",
	Run: func(cmd *cobra.Command, args []string) {

		fmt.Println(args[0])
		val := ""
		//name := ntpCmd.Name()
		//axi.GetCores()
		//axi.Read(&name, &args[0], &val)
		fmt.Println(val)

	},
}

var writeNtp = &cobra.Command{
	Use: "write",
	Run: func(cmd *cobra.Command, args []string) {

	},
}

func init() {
	rootCmd.AddCommand(ntpCmd)
	ntpCmd.AddCommand(readNtp)
	ntpCmd.AddCommand(writeNtp)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// ntpCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// ntpCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
