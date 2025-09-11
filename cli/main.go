/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package main

import (
	"NovusTimeServer/cli/cmd"
	"NovusTimeServer/lib"
)

func main() {

	lib.GetConfig()

	cmd.Execute()

}
