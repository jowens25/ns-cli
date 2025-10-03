/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

var all bool

// calibrationCmd represents the calibration command
var calibrationCmd = &cobra.Command{
	Use:   "cal",
	Short: "get and set calibration factors",
	Long: `Query or set Cal Factors for specific ADC conversions. 
See list of Cal Factors numbered for appropriate measurement 
parameters. These settings should only be changed by an 
authorized technician.`,
	DisableFlagsInUseLine: true, // This hides [flags] from the usage line

	Example: `  cal <channel>			# return channel factor
  cal <channel> <factor>	# sets new rate`,

	Run: func(cmd *cobra.Command, args []string) {

		if all {
			for i := range 10 {
				response, _ := lib.ReadWriteMicro("$CAL" + fmt.Sprint(i+1))
				fmt.Println(response)
			}

		} else if len(args) == 1 {
			response, _ := lib.ReadWriteMicro("$CAL" + args[0])
			fmt.Println(response)

		} else if len(args) == 2 {

			if !lib.IsAdminRoot() {
				fmt.Println("requires admin access")
				return
			}

			response, _ := lib.ReadWriteMicro("$CAL" + args[0] + "=" + args[1])
			fmt.Println(response)
		} else {
			cmd.Help()
		}

	},
}

func init() {
	if INC_HW_CMD {

		rootCmd.AddCommand(calibrationCmd)
		calibrationCmd.Flags().BoolVarP(&all, "all", "a", false, "read all calibration factors")
		calibrationCmd.GroupID = "hw"
	}
}
