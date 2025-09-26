/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"fmt"
	"strconv"
	"unsafe"

	"github.com/spf13/cobra"
	"golang.org/x/sys/unix"
)

// readCmd represents the read command
var readCmd = &cobra.Command{
	Use:   "read",
	Short: "read shared memory",
	Args:  cobra.MinimumNArgs(1),

	Run: func(cmd *cobra.Command, args []string) {

		// Define shared memory key and size (must match the creating process)
		key, _ := strconv.ParseInt(args[0], 10, 64) // Example key
		size := 25600                               // Example size in bytes

		// Get shared memory segment ID
		shmid, _, errno := unix.Syscall(unix.SYS_SHMGET, uintptr(key), uintptr(size), uintptr(0666))
		if errno != 0 {
			fmt.Printf("shmget failed: %v", errno)
		}

		// Attach to the shared memory segment
		shmaddr, _, errno := unix.Syscall(unix.SYS_SHMAT, shmid, 0, 0)
		if errno != 0 {
			fmt.Printf("shmat failed: %v", errno)
		}

		// Convert the shared memory address to a Go byte slice
		// This allows you to read and write data as a byte array
		shmSlice := (*[1 << 30]byte)(unsafe.Pointer(shmaddr))[:size:size]

		// Read data from shared memory (example: reading a string)
		// Ensure the data is properly null-terminated if it's a C-style string
		readData := make([]byte, size)
		copy(readData, shmSlice)
		fmt.Printf("Data read from shared memory: %s\n", string(readData))

		// Detach from the shared memory segment
		_, _, errno = unix.Syscall(unix.SYS_SHMDT, shmaddr, 0, 0)
		if errno != 0 {
			fmt.Printf("shmdt failed: %v", errno)
		}

		// Optionally, remove the shared memory segment (usually done by the creator)
		// _, _, errno = unix.Syscall(unix.SYS_SHMCTL, shmid, uintptr(syscall.IPC_RMID), 0)
		// if errno != 0 {
		// 	fmt.Printf("shmctl IPC_RMID failed: %v", errno)
		// }

	},
}

func init() {
	rootCmd.AddCommand(readCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// readCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// readCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
