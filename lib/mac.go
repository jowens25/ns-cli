package lib

import (
	"io"
	"log"
	"os"
)

func GetMacAddress(i string) string {

	file, err := os.Open("/sys/class/net/" + i + "/address")
	if err != nil {
		log.Fatal("failed to open sys class net file", file.Name())
	}
	defer file.Close()

	buffer := make([]byte, 1024)

	n, err := file.Read(buffer)

	if err == io.EOF {
		log.Fatalf("mac eof")
	}
	if err != nil {
		log.Fatalf("Error reading file: %v", err)
	}

	// Process the read bytes (e.g., print them)
	return string(buffer[:n])
}
