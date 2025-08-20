/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/axi"
	"log"

	"github.com/spf13/cobra"
)

// readCmd represents the read command
var readCmd = &cobra.Command{
	Use:   "read",
	Short: "read data from fpga",
	Long:  `This command is used to read data from the fpga over the uart / axi bus.`,
	Run: func(cmd *cobra.Command, args []string) {

		err := axi.Connect()
		if err != nil {
			log.Println(err)
		}
		err = axi.GetCores()
		if err != nil {
			log.Println(err)
		}

		op := "read"
		val := ""
		axi.Operation(&op, &args[0], &args[1], &val)

	},
}

func init() {
	rootCmd.AddCommand(readCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// readCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// readCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
