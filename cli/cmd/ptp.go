/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"

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
			//		case "show":
			//			//axi.ShowKeys()
			//		case "list":
			//			axi.ListPtpOcProperties()
			//		case "write":
			//			property := args[0]
			//			value := args[1]
			//
			//			fmt.Println(property, value)
			//			axi.WritePtpOc(property, value)
			//		case "read":
			//			property := args[0]
			//
			//			fmt.Println(property, " ", axi.ReadPtpOc(property))
			//
			//		case "test":
			//			property := args[0]
			//			value := args[1]
			//			// read - current
			//			current := axi.ReadPtpOc(property)
			//			fmt.Println(property, " ", current)
			//			// update
			//			axi.WritePtpOc(property, value)
			//			// read - check if new == requested
			//			new := axi.ReadPtpOc(property)
			//			fmt.Println("new value: ", new)
			//			if new == value {
			//				fmt.Println(property, " ", new)
			//				axi.WritePtpOc(property, current)
			//
			//				fmt.Println("TEST PASSED!!")
			//				fmt.Println("Changed back to starting value: ", property, " ", axi.ReadPtpOc(property))
			//			} else {
			//				fmt.Println("TEST FAILED")
			//			}

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
	ptpCmd.Flags().BoolP("test", "t", false, "test a ptp oc config property")
	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// ptpCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// ptpCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
