/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"NovusTimeServer/lib"
	"fmt"

	"github.com/spf13/cobra"
	"github.com/spf13/pflag"
)

// statusCmd represents the status command
var statusCmd = &cobra.Command{
	Use:   "status",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		hasFlags := false

		cmd.Flags().Visit(func(f *pflag.Flag) {
			hasFlags = true

			switch f.Name {

			case "interface":

				if len(args) != 0 {
					//intf := args[0]

					//	fmt.Println(lib.GetInterfaceStatus(intf))
				} else {
					fmt.Println("missing interface")

				}

			case "telnet":
				fmt.Println(lib.GetTelnetStatus())
			case "ssh":
				fmt.Println(lib.GetSshStatus())
			case "http":
				fmt.Println(lib.GetHttpStatus())

			case "port":

				if len(args) != 0 {
					port := args[0]
					fmt.Println(lib.GetPortStatus(port))
				} else {
					fmt.Println("missing port")

				}

			default:
			}

		})

		if !hasFlags {
			cmd.Help()
		}

	},
}

func init() {
	rootCmd.AddCommand(statusCmd)

	statusCmd.Flags().BoolP("telnet", "t", false, "show status of telnet")
	statusCmd.Flags().BoolP("ssh", "s", false, "show status of ssh")
	statusCmd.Flags().BoolP("http", "g", false, "show status of http")
	statusCmd.Flags().BoolP("port", "p", false, "show status of port")
	statusCmd.Flags().BoolP("interface", "i", false, "show status of interfaces")

}
