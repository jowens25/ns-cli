/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

// httpCmd represents the http command
var httpCmd = &cobra.Command{
	Use:   "http",
	Short: "http configuration",
	Long: `Use this command to control and configure http.
Enable, Disable, Status.`,
	Run: func(cmd *cobra.Command, args []string) {
		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			switch f.Name {

			case "enable":
				lib.StartHttp()
			case "disable":
				lib.StopHttp()
			case "status":
				fmt.Println("ssh: ", lib.GetHttpStatus())

			default:
				cmd.Help()
			}
		})

		if !hasFlags {
			cmd.Help()
		}
	},
}

func init() {
	rootCmd.AddCommand(httpCmd)

	httpCmd.Flags().BoolP("enable", "e", false, "enable protocol")
	httpCmd.Flags().BoolP("disable", "d", false, "disable protocol")
	httpCmd.Flags().BoolP("status", "s", false, "get protocol status")
}
