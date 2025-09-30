/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var dateCmd = &cobra.Command{
	Use:                   "date",
	Short:                 "get and set system date",
	DisableFlagsInUseLine: true,
	Example: `date			# return current date
date <yyyy> <mm> <dd>	# sets new date`,
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {

			fmt.Println(lib.GetDate())

		} else if len(args) == 3 {

			if !lib.IsAdminRoot() {
				fmt.Println("requires admin access")
				return
			}

			fmt.Println(lib.SetDate(args))

		} else {
			cmd.Help()
		}

	},
}

var latestCmd = &cobra.Command{
	Use:   "latest",
	Short: "get and set time from google.com",
	Run: func(cmd *cobra.Command, args []string) {

		fmt.Println(lib.SetLatest())

	},
}

func init() {
	rootCmd.AddCommand(dateCmd)
	dateCmd.AddCommand(latestCmd)

}
