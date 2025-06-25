/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/api"
	"fmt"
	"log"

	"github.com/spf13/cobra"
	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
	"gorm.io/gorm/logger"
)

// dumpCmd represents the dump command
var dumpCmd = &cobra.Command{
	Use:   "dump",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("dump called")

		Db, err := gorm.Open(sqlite.Open("./app.db"), &gorm.Config{
			Logger: logger.Default.LogMode(logger.Info),
		})
		if err != nil {
			log.Fatal("Failed to connect to database: ", err)
		}

		var users []api.User

		result := Db.Select("id, role, username, email, password").Find(&users)
		if result.Error != nil {
			fmt.Println("Error")
			return
		}
		fmt.Println(users)

		for _, user := range users {
			fmt.Printf("ID: %v, Role: %s, Username: %s, Email: %s, Password: %s\n",
				user.ID, user.Role, user.Username, user.Email, user.Password)
		}

	},
}

func init() {
	rootCmd.AddCommand(dumpCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// dumpCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// dumpCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
