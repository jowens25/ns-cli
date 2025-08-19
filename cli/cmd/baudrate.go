/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/api"
	"fmt"

	"github.com/spf13/cobra"
)

// baudrateCmd represents the baudrate command
var baudrateCmd = &cobra.Command{
	Use:   "baudrate",
	Short: "CLI Alias for $BAUDNV.",
	Long: `Get / Set The RS232 Rear Panel Baud Rate. 

This command can be used to assign and query the baud rate on rear panel RS232. (Default = 115200). 
Available baudrates are 19200, 38400, 57600, 115200, 230400. 

<baudrate> to get the current rate. 
<baudrate=115200> to set the rate.
`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("baudrate called")

		//baudCmd := "BAUDNV"

		if len(args) == 0 {

			response := api.ReadWriteMicro("BAUDNV", "BAUDNV")
			fmt.Println(response)

		} else if len(args) == 1 {
			response := api.ReadWriteMicro("BAUDNV", "BAUDNV", args[0])
			fmt.Println(response)
		} else {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(baudrateCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// baudrateCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// baudrateCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
