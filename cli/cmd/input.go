package cmd

import (
	"NovusTimeServer/lib"
	"fmt"
	"strings"

	"github.com/spf13/cobra"
)

var inputCmd = &cobra.Command{
	Use:                   "input",
	Short:                 "get and set input a, b channel settings",
	DisableFlagsInUseLine: true,

	Run: func(cmd *cobra.Command, args []string) {
		cmd.Help()
	},
}

var selectCmd = &cobra.Command{
	Use:                   "select",
	Short:                 "get and set input channel priority",
	DisableFlagsInUseLine: true,

	Long: `Use this command to get and set the input priority

0 = Select Input A
1 = Select Input B
2 = Auto Select (Prioritize Input A) (Default)
3 = Auto Select (Prioritize Input B).`,
	Run: func(cmd *cobra.Command, args []string) {
		if len(args) == 0 {

			response, err := lib.ReadWriteMicro("$INP")
			if err != nil {
				fmt.Println(err.Error())
			}
			fmt.Println(response)

		} else if len(args) == 1 {
			response, err := lib.ReadWriteMicro("$INP=" + args[0])
			if err != nil {
				fmt.Println(err.Error())
			}
			fmt.Println(response)
		}
	},
}

var lowCmd = &cobra.Command{
	Use:                   "low [board number] [voltage]",
	Short:                 "get and set input low threshold value",
	DisableFlagsInUseLine: true,

	Long: `get and set the absolute voltage threshold 
at which the input monitor reports input fault. 
For example, if the THR is set to "0.3", the Channel Fault 
Byte will report an error if the measured Vpp is lower 
than 0.3V. (from 0.05V to 1.00V)

$INPTHR0: Amplifier board 0 (top)
$INPTHR1: Amplifier board 1 (bottom)`,

	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {

			cmd.Help()

		} else if len(args) == 1 {

			response, err := lib.ReadWriteMicro("$INPTHR" + args[0])
			if err != nil {
				fmt.Println(err.Error())
			}
			fmt.Println(response)

		} else if len(args) == 2 {
			response, err := lib.ReadWriteMicro("$INPTHR" + args[0] + "=" + args[1])
			if err != nil {
				fmt.Println(err.Error())
			}
			fmt.Println(response)
		}

	},
}

var onLockCmd = &cobra.Command{
	Use:                   "lock",
	DisableFlagsInUseLine: true,

	Short: "prioritize input on lock status (requires CAN bus connection)",
	Long: `get and set the priority input based on 
GNSS and Loop Lock status of input source (CAN connected 
Novus NR reference). When $PRLK is active, the input will
switch to secondary input source if primary input source 
indicates GNSS lock is lost and secondary input source has GNSS 
lock. If $PRLK is enabled, $PRHR is disabled. Requires CAN bus connector.`,
	ValidArgs: []string{"0", "1"},

	Run: func(cmd *cobra.Command, args []string) {
		if len(args) == 0 {

			response, err := lib.ReadWriteMicro("$PRLK")
			if err != nil {
				fmt.Println(err.Error())
			}
			fmt.Println(response)

		} else if len(args) == 1 {
			response, err := lib.ReadWriteMicro("$PRLK=" + args[0])
			if err != nil {
				fmt.Println(err.Error())
			}
			fmt.Println(response)
		}
	},
}

var onHoldoverCmd = &cobra.Command{
	Use:                   "holdover",
	DisableFlagsInUseLine: true,

	Short: "prioritize input on holdover status (requires CAN bus connection)",
	Long: `Use this command to set the priority input based on 
valid holdover indicator of input source (CAN connected 
Novus NR reference). When $PRHR is active, the input will
switch to secondary input source if primary input source 
indicates holdover period has expired. If $PRHR is enabled, 
$PRLK is disabled. Requires CAN`,
	ValidArgs: []string{"0", "1"},

	Run: func(cmd *cobra.Command, args []string) {
		if len(args) == 0 {

			response, err := lib.ReadWriteMicro("$PRHR")
			if err != nil {
				fmt.Println(err.Error())
			}
			fmt.Println(response)

		} else if len(args) == 1 {
			response, err := lib.ReadWriteMicro("$PRHR=" + args[0])
			if err != nil {
				fmt.Println(err.Error())
			}
			fmt.Println(response)
		}
	},
}

// faultCmd represents the fault command
var faultCmd = &cobra.Command{
	Use:                   "fault [channel] [n.nn]",
	DisableFlagsInUseLine: true,

	Short: "input channel fault threshold factor",
	Long: `Use this command to assign and query the 
ratio at which the Channel output monitors report a fault. 
For example, if the FLTTHRA is set to "0.15", the Channel 
Fault Word will report an error if the measured value is 
greater or less than ±15% of its target value, when sourced 
from Input A. Number format must be in the form <n.nn> (from 0.05 to 0.95)`,

	Run: func(cmd *cobra.Command, args []string) {

		if len(args) == 0 {

			cmd.Help()

		} else if len(args) == 1 {

			response, err := lib.ReadWriteMicro("$FLTTHR" + strings.ToUpper(args[0]))
			if err != nil {
				fmt.Println(err.Error())
			}
			fmt.Println(response)

		} else if len(args) == 2 {
			response, err := lib.ReadWriteMicro("$FLTTHR" + strings.ToUpper(args[0]) + "=" + args[1])
			if err != nil {
				fmt.Println(err.Error())
			}
			fmt.Println(response)
		}

	},
}

func init() {
	rootCmd.AddCommand(inputCmd)
	inputCmd.AddCommand(selectCmd)
	inputCmd.AddCommand(lowCmd)
	inputCmd.AddCommand(faultCmd)
	inputCmd.AddCommand(onLockCmd)
	inputCmd.AddCommand(onHoldoverCmd)

	inputCmd.GroupID = "hw"

}
