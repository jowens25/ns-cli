/*
Copyright © 2025 NAME HERE <EMAIL ADDRESS>
*/
package main

import (
	"NovusTimeServer/cli/cmd"
	"log"
	"os"
)

func main() {

	logFile, err := os.OpenFile("app.log", os.O_RDWR|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		log.Fatalf("error opening log file: %v", err)
	}
	defer logFile.Close()

	log.SetOutput(logFile)

	cmd.Execute()

}
