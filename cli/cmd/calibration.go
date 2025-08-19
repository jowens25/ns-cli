/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/api"
	"fmt"

	"github.com/spf13/cobra"
)

// calibrationCmd represents the calibration command
var calibrationCmd = &cobra.Command{
	Use:   "cal",
	Short: "Calibration Factors",
	Long: `Query or set Cal Factors for specific ADC conversions. 
See list of Cal Factors numbered for appropriate measurement 
parameters. These settings should only be changed by an 
authorized technician.`,
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {
			cmd.Help()

		} else if len(args) == 1 {
			response := api.ReadWriteMicro("CAL"+args[0], "CAL")
			fmt.Println(response)

		} else if len(args) == 2 {
			response := api.ReadWriteMicro("CAL"+args[0], "CAL", args[1])
			fmt.Println(response)
		} else {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(calibrationCmd)

}
