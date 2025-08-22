/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

// timeCmd represents the time command
var timeCmd = &cobra.Command{
	Use:                   "time",
	Short:                 "display or set the system time",
	Long:                  `Use this command to get and set system time.`,
	DisableFlagsInUseLine: true,
	Example: `time			# return current time
time <hh> <mm> <ss>	# sets new time`,
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {

			fmt.Println(lib.GetTime())

		} else if len(args) == 3 {

			fmt.Println(lib.SetTime(args))

		} else {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(timeCmd)

}
