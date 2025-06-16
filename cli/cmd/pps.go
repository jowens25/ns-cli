/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

// ppsCmd represents the pps command
var ppsCmd = &cobra.Command{
	Use:   "pps",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {

		cmd.Flags().Visit(func(f *pflag.Flag) {

			switch f.Name {

			//case "read":
			//	//axi.ModuleOperation("ntp-server", "r", args[0])
			//	axi.ReadPpsSlave(args[0])
			//case "write":
			//	//axi.ModuleOperation("ntp-server", "w", args[0], args[1])
			//	axi.WritePpsSlave(args[0], args[1])
			//case "test":
			//
			//	property := args[0]
			//	value := args[1]
			//	// read - current
			//	current := axi.ReadPpsSlave(property)
			//	fmt.Println(property, " ", current)
			//	// update
			//	axi.WritePpsSlave(property, value)
			//	// read - check if new == requested
			//	new := axi.ReadPpsSlave(property)
			//	fmt.Println("new value: ", new)
			//	if new == value {
			//		fmt.Println(property, " ", new)
			//		axi.WritePpsSlave(property, current)
			//
			//		fmt.Println("TEST PASSED!!")
			//		fmt.Println("Changed back to starting value: ", property, " ", axi.ReadPpsSlave(property))
			//	} else {
			//		fmt.Println("TEST FAILED")
			//	}
			//case "list":
			//	for _, p := range axi.PpsSlaveProperties {
			//		fmt.Println(p)
			//	}
			default:
				fmt.Println("Please pass the ntp command a valid flag")
			}
		})

	},
}

func init() {
	rootCmd.AddCommand(ppsCmd)
	ppsCmd.Flags().BoolP("list", "l", false, "list pps slave properties")
	ppsCmd.Flags().BoolP("read", "r", false, "read pps slave rproperty")
	ppsCmd.Flags().BoolP("write", "w", false, "write pps slave property")
	ppsCmd.Flags().BoolP("test", "t", false, "property test")
	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// ppsCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// ppsCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
