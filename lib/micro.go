package lib

import "C"
import (
	"fmt"
	"log"
	"strings"
	"time"

	"go.bug.st/serial"
)

// command is the actual string so ex $BAUDNV
func ReadWriteMicro(command string) (string, error) {

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
		log.Println(AppConfig.Serial.Port, err)
		return "port open error", err
	}
	defer port.Close()

	err = port.SetReadTimeout(time.Second * 1)
	if err != nil {
		panic(err)
	}

	port.ResetInputBuffer()
	port.ResetOutputBuffer()

	_, err = port.Write([]byte(command))

	fmt.Print(command)

	if err != nil {
		log.Println(err)
		return "port write error", err

	}

	for {
		n, err := port.Read(read_data)
		if err != nil {
			log.Println(err)
		}

		lines := string(read_data[:n])

		if strings.Contains(lines, command[:3]) {
			responses := strings.Split(lines, "\r\n")
			for _, response := range responses {
				if strings.Contains(response, command[:3]) {
					return strings.TrimSpace(responses[0]), nil

				}
			}

		}
	}

}
