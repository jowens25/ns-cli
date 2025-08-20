/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"
	"os/exec"

	"github.com/spf13/cobra"
)

var dateCmd = &cobra.Command{
	Use:                   "date",
	Short:                 "system date",
	Long:                  `Use this command to get and set system date.`,
	DisableFlagsInUseLine: true,
	Example: `date			# return current date
date <yyyy> <mm> <dd>	# sets new date`,
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {

			myCmd := exec.Command("timedatectl", "status")
			out, err := myCmd.CombinedOutput()
			if err != nil {
				fmt.Println(string(out), err)
			}
			fmt.Print(string(out))

		} else if len(args) == 3 {

			lib.ToggleNtpSync("no")
			fmt.Println("disabling synchronization")

			myCmd := exec.Command("timedatectl", "set-time", args[0]+"-"+args[1]+"-"+args[2])
			out, err := myCmd.CombinedOutput()
			if err != nil {
				fmt.Println(string(out), err)
			}
			fmt.Print(string(out))

		} else {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(dateCmd)

}
