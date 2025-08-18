package api

import (
	"fmt"
	"log"
	"os"
)

func SendRaw(rawString string) {
	file, err := os.OpenFile("/dev/ttymxc2", os.O_RDWR, 0)
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	buffer := make([]byte, 256)
	for {
		n, err := file.Read(buffer)
		if err != nil {
			break
		}
		if n > 0 {
			fmt.Print(string(buffer[:n]))
		}
	}
}
