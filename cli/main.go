/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package main

import (
	"NovusTimeServer/cli/cmd"
	"NovusTimeServer/lib"
	"log"
	"os"
)

func main() {

	lib.GetConfig()

	logFile, err := os.OpenFile(lib.AppConfig.App.Log, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		log.Fatalf("error opening log file: %v", err)
	}
	defer logFile.Close()

	log.SetOutput(logFile)

	cmd.Execute()

}
