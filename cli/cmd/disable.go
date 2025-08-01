/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

// disableCmd represents the disable command
var disableCmd = &cobra.Command{
	Use:   "disable",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {

		//cmd.Flags().Visit(func(f *pflag.Flag) {
		//
		//	switch f.Name {
		//	case "ssh":
		//		axi.LoadConfig(args[0])
		//
		//	default:
		//		fmt.Println("only load works right now")
		//	}
		//})

		switch args[0] {
		case "ssh":
			fmt.Println("disable ssh")
		case "telnet":
			fmt.Println("disable telnet")
		}

	},
}

func init() {
	rootCmd.AddCommand(disableCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// disableCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// disableCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
