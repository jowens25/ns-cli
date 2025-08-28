/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
)

// calibrationCmd represents the calibration command
var calibrationCmd = &cobra.Command{
	Use:   "cal",
	Short: "calibration factors",
	Long: `Query or set Cal Factors for specific ADC conversions. 
See list of Cal Factors numbered for appropriate measurement 
parameters. These settings should only be changed by an 
authorized technician.`,
	DisableFlagsInUseLine: true, // This hides [flags] from the usage line

	Example: `
  # Common usage patterns
  cal <channel>			# return channel factor
  cal <channel> <factor>	# sets new rate`,

	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {
			cmd.Help()

		} else if len(args) == 1 {
			response := lib.ReadWriteMicro("CAL"+args[0], "CAL")
			fmt.Println(response)

		} else if len(args) == 2 {
			response := lib.ReadWriteMicro("CAL"+args[0], "CAL", args[1])
			fmt.Println(response)
		} else {
			cmd.Help()
		}

	},
}

// calibrationCmd represents the calibration command
var calSaveCmd = &cobra.Command{
	Use:   "save",
	Short: "save calibration factors",
	Long: `This command will translate all Calibration Factors
to flash string and write. Data is then read back for
verification, and result reported. This will update
Cal Factors in flash to the current Cal Settings.`,
	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {
			response := lib.ReadWriteMicro("SAVECAL", "CAL")
			fmt.Println(response)
		} else {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(calibrationCmd)
	calibrationCmd.AddCommand(calSaveCmd)
	calibrationCmd.GroupID = "hw"

}
