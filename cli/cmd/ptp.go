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

// ptpCmd represents the ptp command
var ptpCmd = &cobra.Command{
	Use:   "ptp",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {

		cmd.Flags().Visit(func(f *pflag.Flag) {

			switch f.Name {
			case "show":
				axi.ShowKeys()
			case "list":
				axi.ListPtpOcProperties()
			case "write":

				fmt.Println(args[0], args[1])
				axi.WritePtpOc(args[0], args[1])
			case "read":
				fmt.Println(args[0], " ", axi.ReadPtpOc(args[0]))

			default:
				fmt.Println("Please pass the ntp command a valid flag")
			}
		})

	},
}

func init() {
	rootCmd.AddCommand(ptpCmd)
	ptpCmd.Flags().BoolP("show", "s", false, "show keys")
	ptpCmd.Flags().BoolP("list", "l", false, "list ptp oc properties and values")
	ptpCmd.Flags().BoolP("write", "w", false, "write ptp oc property")
	ptpCmd.Flags().BoolP("read", "r", false, "read ptp oc property")
	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// ptpCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// ptpCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
