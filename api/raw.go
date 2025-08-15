package api

import (
	"fmt"
	"log"

	"go.bug.st/serial"
)

func SendRaw(r []byte) {

	mode := &serial.Mode{
		BaudRate: 9600,
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	}

	port, err := serial.Open("/dev/ttymxc2", mode)
	if err != nil {
		log.Println("failed to open serial device", err)
	}
	defer port.Close()

	r = append(r, 0x0D)

	r = append(r, 0x0A)

	_, err = port.Write(r)

	if err != nil {
		log.Println("failed to write to serial device: %w", err)
	}

	readBuffer := make([]byte, 64)

	for {
		n, err := port.Read(readBuffer)
		if err != nil {
			log.Fatalf("failed to read from serial port: %v", err)
		}
		if n == 0 {
			fmt.Println("No data received, exiting read loop.")
			break
		}
		fmt.Printf("Received %d bytes: %s\n", n, string(readBuffer[:n]))
		// Add logic to process received data or break from the loop based on your needs
	}
}
