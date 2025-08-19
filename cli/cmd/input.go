/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/api"
	"fmt"

	"github.com/spf13/cobra"
)

// inputCmd represents the input command
var inputCmd = &cobra.Command{
	Use:   "input",
	Short: "CLI Alias for $INP.",
	Long: `Get / Set the input priority setting to A, B, Auto A, Auto B.
0 = Select Input A
1 = Select Input B
2 = Auto Select (Prioritize Input A) (Default)
3 = Auto Select (Prioritize Input B). 

This command can be used to assign and query the Input Priority Setting. 
<inp> to get the current input.
<inp> <num> to set the input.
`,
	Run: func(cmd *cobra.Command, args []string) {
		if len(args) == 0 {

			response := api.ReadWriteMicro("INP", "INP")
			fmt.Println(response)

		} else if len(args) == 1 {
			response := api.ReadWriteMicro("INP", "INP", args[0])
			fmt.Println(response)
		} else {
			cmd.Help()
		}
	},
}

func init() {
	rootCmd.AddCommand(inputCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// inputCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// inputCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
