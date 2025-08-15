package api

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

func SendRaw(r string) {

	ser, err := os.OpenFile("/dev/ttymxc2", os.O_WRONLY, 0)
	if err != nil {
		log.Println("failed to open serial device", err)
	}
	defer ser.Close()

	_, err = ser.WriteString(r + "\r\n")

	if err != nil {
		log.Println("failed to write to serial device: %w", err)
	}

	scanner := bufio.NewScanner(os.Stdin)

	for scanner.Scan() {
		response := scanner.Text()
		fmt.Println(response)
		break
	}

}
