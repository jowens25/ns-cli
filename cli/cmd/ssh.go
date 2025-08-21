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

// sshCmd represents the ssh command
var sshCmd = &cobra.Command{
	Use:   "ssh",
	Short: "ssh configuration",
	Long: `Use this command to control and configure ssh.
Enable, Disable, Status, Configure.`,
	Run: func(cmd *cobra.Command, args []string) {
		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			switch f.Name {

			case "enable":
				lib.StartSsh()
			case "disable":
				lib.StopSsh()
			case "status":
				fmt.Print("ssh: ", lib.GetSshStatus())

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
	rootCmd.AddCommand(sshCmd)
	sshCmd.Flags().BoolP("enable", "e", false, "enable protocol")
	sshCmd.Flags().BoolP("disable", "d", false, "disable protocol")
	sshCmd.Flags().BoolP("status", "s", false, "get protocol status")

}
