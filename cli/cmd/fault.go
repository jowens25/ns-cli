/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/api"
	"fmt"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

// faultCmd represents the fault command
var faultCmd = &cobra.Command{
	Use:   "fault",
	Short: "A CLI Alias for $FLTTHR",
	Long: `This command can be used to assign and query 
the ratio at which the Channel output monitors report a fault. 
For example, if the FLTTHRA is set to "0.15", the Channel Fault Word 
will report an error if the measured value is greater or less than
±15% of its target value, when sourced from Input A. 
Number format must be in the form <n.nn> (from 0.05 to 0.95)
<fault> <-b>
<fault> <-a> <0.06>`,

	Run: func(cmd *cobra.Command, args []string) {

		cmd.Flags().Visit(func(f *pflag.Flag) {

			switch f.Name {
			case "threshold a":
				cmdRoot := "FLTTHRA"

				if len(args) == 0 {

					response := api.ReadWriteMicro(cmdRoot, cmdRoot)
					fmt.Println(response)

				} else if len(args) == 1 {
					response := api.ReadWriteMicro(cmdRoot, cmdRoot, args[0])
					fmt.Println(response)
				} else {
					cmd.Help()
				}

			case "threshold b":
				cmdRoot := "FLTTHRB"
				if len(args) == 0 {

					response := api.ReadWriteMicro(cmdRoot, cmdRoot)
					fmt.Println(response)

				} else if len(args) == 1 {
					response := api.ReadWriteMicro(cmdRoot, cmdRoot, args[0])
					fmt.Println(response)
				} else {
					cmd.Help()
				}

			default:
				fmt.Println("please select either input <a> or <b>")
			}

		})

	},
}

func init() {
	rootCmd.AddCommand(faultCmd)

	faultCmd.Flags().BoolP("threshold a", "a", false, "get / set fault threashold for input channel A")
	faultCmd.Flags().BoolP("threshold b", "b", false, "get / set fault threashold for input channel B")

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// faultCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// faultCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
