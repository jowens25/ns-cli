/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package main

import (
	"NovusTimeServer/cli/cmd"
)

func main() {

	cmd.Execute()

	//buf := make([]byte, 1024)
	////n := runtime.Stack(buf, true)
	////fmt.Printf("Number of bytes written: %d\n", n)
	//fmt.Printf("All goroutines:\n%s", buf[:n])
}
