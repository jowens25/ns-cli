package lib

import "C"
import (
	"fmt"
	"log"
	"strings"

	"go.bug.st/serial"
)

func ReadWriteMicro(command string) string {

	command = command + "\r\n"

	mode := &serial.Mode{
		BaudRate: 38400, // Adjust to match your device
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	}

	mcu_port := "/dev/ttymxc2"

	read_data := make([]byte, 1024)

	port, err := serial.Open(mcu_port, mode)
	if err != nil {
		log.Fatal(err)
	}
	defer port.Close()

	port.ResetInputBuffer()
	port.ResetOutputBuffer()

	n, err := port.Write([]byte(command))

	fmt.Println(command)

	if err != nil {
		log.Fatal(err)
	}

	if n > 0 {
		//fmt.Println("wrote: ", n, " bytes")
	}

	n, err = port.Read(read_data)

	if err != nil {
		log.Fatal(err)
	}

	if n > 0 {
		//fmt.Println("read: ", n, " bytes")
	}

	//fmt.Println(string(read_data))

	lines := strings.Split(string(read_data), "\n")

	return lines[0]

}
