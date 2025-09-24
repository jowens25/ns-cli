package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

var inputCmd = &cobra.Command{
	Use:   "input",
	Short: "get and setinput a, b channel settings",
	Run: func(cmd *cobra.Command, args []string) {
		cmd.Help()
	},
}

var selectCmd = &cobra.Command{
	Use:   "select",
	Short: "select input channel priority",
	Long: `Use this command to get and set the input priority 
	setting to A, B, Auto A, Auto B.
0 = Select Input A
1 = Select Input B
2 = Auto Select (Prioritize Input A) (Default)
3 = Auto Select (Prioritize Input B).`,
	Run: func(cmd *cobra.Command, args []string) {
		if len(args) == 0 {

			//response := lib.ReadWriteMicro("INP", "INP")
			//fmt.Println(response)

		} else if len(args) == 1 {
			//response := lib.ReadWriteMicro("INP", "INP", args[0])
			//fmt.Println(response)
		} else {
			cmd.Help()
		}
	},
}

var lowCmd = &cobra.Command{
	Use:   "low",
	Short: "input low threshold value",
	Long: `Use this command to get and set the absolute voltage 
threshold at which the input monitor reports input fault. 
For example, if the THR is set to "0.3", the Channel Fault 
Byte will report an error if the measured Vpp is lower 
than 0.3V. (from 0.05V to 1.00V)

($INPTHR0: Amplifier board 0 (top))
($INPTHR1: Amplifier board 1 (bottom))`,

	Run: func(cmd *cobra.Command, args []string) {

		cmd.Flags().Visit(func(f *pflag.Flag) {

			switch f.Name {
			case "threshold 0":
				//cmdRoot := "INPTHR0"

				if len(args) == 0 {

					//response := lib.ReadWriteMicro(cmdRoot, cmdRoot)
					//fmt.Println(response)

				} else if len(args) == 1 {
					//response := lib.ReadWriteMicro(cmdRoot, cmdRoot, args[0])
					//fmt.Println(response)
				} else {
					cmd.Help()
				}

			case "threshold 1":
				//cmdRoot := "INPTHR1"
				if len(args) == 0 {

					//response := lib.ReadWriteMicro(cmdRoot, cmdRoot)
					//fmt.Println(response)

				} else if len(args) == 1 {
					////response := lib.ReadWriteMicro(cmdRoot, cmdRoot, args[0])
					//fmt.Println(response)
				} else {
					cmd.Help()
				}

			default:
				fmt.Println("please select either amplifier board input <0> or <1>")
			}

		})

	},
}

var onLockCmd = &cobra.Command{
	Use:   "lock",
	Short: "prioritize input on lock status (requires CAN bus connection)",
	Long: `Use this command to set the priority input based on 
GNSS and Loop Lock status of input source (CAN connected 
Novus NR reference). When $PRLK is active, the input will
switch to secondary input source if primary input source 
indicates GNSS lock is lost and secondary input source has GNSS 
lock. If $PRLK is enabled, $PRHR is disabled. Requires CAN bus connector.`,
	ValidArgs: []string{"0", "1"},

	Run: func(cmd *cobra.Command, args []string) {
		if len(args) == 0 {

			//response := lib.ReadWriteMicro("PRLK", "PRLK")
			//fmt.Println(response)

		} else if len(args) == 1 {
			//response := lib.ReadWriteMicro("PRLK", "PRLK", args[0])
			//fmt.Println(response)
		} else {
			cmd.Help()
		}
	},
}

var onHoldoverCmd = &cobra.Command{
	Use:   "holdover",
	Short: "prioritize input on holdover status (requires can bus connection)",
	Long: `Use this command to set the priority input based on 
valid holdover indicator of input source (CAN connected 
Novus NR reference). When $PRHR is active, the input will
switch to secondary input source if primary input source 
indicates holdover period has expired. If $PRHR is enabled, 
$PRLK is disabled. Requires CAN`,
	ValidArgs: []string{"0", "1"},

	Run: func(cmd *cobra.Command, args []string) {
		if len(args) == 0 {

			response, _ := lib.ReadWriteMicro("$PRLK")
			fmt.Println(response)

		} else if len(args) == 1 {
			response, _ := lib.ReadWriteMicro("$PRLK" + args[0])
			fmt.Println(response)
		} else {
			cmd.Help()
		}
	},
}

// faultCmd represents the fault command
var faultCmd = &cobra.Command{
	Use:   "fault [flags] <n.nn>",
	Short: "input channel fault threshold factor",
	Long: `Use this command to assign and query the 
ratio at which the Channel output monitors report a fault. 
For example, if the FLTTHRA is set to "0.15", the Channel 
Fault Word will report an error if the measured value is 
greater or less than ±15% of its target value, when sourced 
from Input A. Number format must be in the form <n.nn> (from 0.05 to 0.95)`,
	DisableFlagsInUseLine: true,
	Run: func(cmd *cobra.Command, args []string) {

		cmd.Flags().Visit(func(f *pflag.Flag) {

			switch f.Name {
			case "threshold a":
				//cmdRoot := "FLTTHRA"

				if len(args) == 0 {

					//response := lib.ReadWriteMicro(cmdRoot, cmdRoot)
					//fmt.Println(response)

				} else if len(args) == 1 {
					//response := lib.ReadWriteMicro(cmdRoot, cmdRoot, args[0])
					//fmt.Println(response)
				} else {
					cmd.Help()
				}

			case "threshold b":
				//cmdRoot := "FLTTHRB"
				if len(args) == 0 {

					//response := lib.ReadWriteMicro(cmdRoot, cmdRoot)
					//fmt.Println(response)

				} else if len(args) == 1 {
					//response := lib.ReadWriteMicro(cmdRoot, cmdRoot, args[0])
					//fmt.Println(response)
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
	rootCmd.AddCommand(inputCmd)
	inputCmd.AddCommand(selectCmd)
	inputCmd.AddCommand(lowCmd)
	inputCmd.AddCommand(faultCmd)
	inputCmd.AddCommand(onLockCmd)
	inputCmd.AddCommand(onHoldoverCmd)

	selectCmd.Flags().Bool("a", false, "Select Input A")
	selectCmd.Flags().Bool("b", false, "Select Input B")
	selectCmd.Flags().Bool("auto a", false, "Auto Select (Prioritize Input A) (Default)")
	selectCmd.Flags().Bool("auto b", false, "Auto Select (Prioritize Input B)")

	lowCmd.Flags().BoolP("threshold 0", "0", false, "get / set low threashold for amplifier board input 0")
	lowCmd.Flags().BoolP("threshold 1", "1", false, "get / set low threashold for amplifier board input 1")

	faultCmd.Flags().BoolP("threshold a", "a", false, "get / set fault threashold for input channel A")
	faultCmd.Flags().BoolP("threshold b", "b", false, "get / set fault threashold for input channel B")
	inputCmd.GroupID = "hw"

}
