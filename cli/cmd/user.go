/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"
	"os/exec"
	"strings"

	"github.com/spf13/cobra"
)

// userCmd represents the user command
var userCmd = &cobra.Command{
	Use:   "user",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("user called")

		// Method 1: Use chpasswd (requires root, but more reliable for automation)
		password := "mynovus123"
		thiscmd := exec.Command("chpasswd")
		thiscmd.Stdin = strings.NewReader(fmt.Sprintf("%s:%s", "novus", password))

		output, err := thiscmd.CombinedOutput()
		fmt.Println(string(output))
		if err != nil {
			fmt.Printf("Error running chpasswd: %v\n", err)
			fmt.Printf("Output: %s\n", output)

		}
	},
}

func init() {
	rootCmd.AddCommand(userCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// userCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// userCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
