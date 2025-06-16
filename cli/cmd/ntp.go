/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

// ntpCmd represents the ntp command
var ntpCmd = &cobra.Command{
	Use:   "ntp",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {

		cmd.Flags().Visit(func(f *pflag.Flag) {

			switch f.Name {
			//	case "module":
			//		//axi.ModuleOperation()
			//
			//	case "list":
			//		//axi.ListNtpProperties()
			//	case "version":
			//		axi.ReadNtpServer("version")
			//	case "ip":
			//		fmt.Println("ip flag called")
			//
			//	case "read":
			//		//axi.ModuleOperation("ntp-server", "r", args[0])
			//		fmt.Println(axi.ReadNtpServer(args[0]))
			//	case "write":
			//		//axi.ModuleOperation("ntp-server", "w", args[0], args[1])
			//
			//	case "test":
			//		property := args[0]
			//		value := args[1]
			//		// read - current
			//		current := axi.ReadNtpServer(property)
			//		fmt.Println(property, " ", current)
			//		// update
			//		axi.WriteNtpServer(property, value)
			//		// read - check if new == requested
			//		new := axi.ReadNtpServer(property)
			//		fmt.Println("new value: ", new)
			//		if new == value {
			//			fmt.Println(property, " ", new)
			//			axi.WriteNtpServer(property, current)
			//
			//			fmt.Println("TEST PASSED!!")
			//			fmt.Println("Changed back to starting value: ", property, " ", axi.ReadNtpServer(property))
			//		} else {
			//			fmt.Println("TEST FAILED")
			//		}

			default:
				fmt.Println("Please pass the ntp command a valid flag")
			}
		})

	},
}

func init() {
	rootCmd.AddCommand(ntpCmd)
	ntpCmd.Flags().BoolP("list", "l", false, "list ntp properties and values")
	ntpCmd.Flags().BoolP("version", "v", false, "list ntp server version")
	ntpCmd.Flags().BoolP("read", "r", false, "read ntp serve rproperty")
	ntpCmd.Flags().BoolP("write", "w", false, "write ntp server property")
	ntpCmd.Flags().BoolP("test", "t", false, "property test")
	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// ntpCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// ntpCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
