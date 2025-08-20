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

// timeCmd represents the time command
var timeCmd = &cobra.Command{
	Use:                   "time",
	Short:                 "Display or set the system time",
	Long:                  `Use this command to get and set system time.`,
	DisableFlagsInUseLine: true,
	Example: `time			# return current time
time <hh> <mm> <ss>	# sets new time`,
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

			myCmd := exec.Command("timedatectl", "set-time", args[0]+":"+args[1]+":"+args[2])
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
	rootCmd.AddCommand(timeCmd)

}
