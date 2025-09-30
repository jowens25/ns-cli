/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"

	"github.com/spf13/cobra"
)

// resetpwCmd represents the resetpw command
var resetpwCmd = &cobra.Command{
	Use:   "resetpw",
	Short: "reset default admin account password",
	Run: func(cmd *cobra.Command, args []string) {

		var user lib.User

		user.Username = lib.AppConfig.User.DefaultUsername
		user.Password = lib.AppConfig.User.DefaultPassword

		lib.SetPasswordEnforcement(false)

		lib.ChangePassword(user)

		lib.SetPasswordEnforcement(true)
	},
}

func init() {
	rootCmd.AddCommand(resetpwCmd)

}
