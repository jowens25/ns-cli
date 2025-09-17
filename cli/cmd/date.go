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
	Short:                 "set system date",
	Long:                  `Use this command to get and set system date.`,
	DisableFlagsInUseLine: true,
	Example: `date			# return current date
date <yyyy> <mm> <dd>	# sets new date`,
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {

			fmt.Println(lib.GetDate())

		} else if len(args) == 3 {

			fmt.Println(lib.SetDate(args))

		} else {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(dateCmd)

}
