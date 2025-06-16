/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"

	"github.com/jowens25/axi"
	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

// axiCmd represents the axi command
var axiCmd = &cobra.Command{
	Use:   "axi",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("axi called")

		cmd.Flags().Visit(func(f *pflag.Flag) {

			switch f.Name {
			case "read":
				axi.Operate("read", "ntp-server", "status", "000000000000000000000000000000000")
				//axi.Read("ntp-server", "status")
			case "write":
				axi.Operate("write", "ntp-server", "status", "enabled")
			default:
				fmt.Println("Please pass the ntp command a valid flag")
			}
		})
	},
}

func init() {
	rootCmd.AddCommand(axiCmd)
	axiCmd.Flags().BoolP("read", "r", false, "run prototype axi read")
	axiCmd.Flags().BoolP("write", "w", false, "run prototype axi read")

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// axiCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// axiCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
