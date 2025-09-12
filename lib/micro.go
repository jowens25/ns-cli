package lib

import "C"
import (
	"fmt"
	"log"
	"strings"

	"go.bug.st/serial"
)

// command is the actual string so ex $BAUDNV
func ReadWriteMicro(command string) string {

	command = command + "\r\n"

	mode := &serial.Mode{
		BaudRate: AppConfig.Serial.Baudrate,
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	}

	read_data := make([]byte, 1024)

	port, err := serial.Open(AppConfig.Serial.Port, mode)

	if err != nil {
		log.Fatal(AppConfig.Serial.Port, err)
	}
	defer port.Close()

	port.ResetInputBuffer()
	port.ResetOutputBuffer()

	_, err = port.Write([]byte(command))

	fmt.Print(command)

	fmt.Print("--->")

	if err != nil {
		log.Fatal(err)
	}

	_, err = port.Read(read_data)

	if err != nil {
		log.Fatal(err)
	}

	lines := strings.Split(string(read_data), "\n")

	return lines[0]

}
