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
	port.SetReadTimeout(1 * time.Second)

	if err != nil {
		log.Println(AppConfig.Serial.Port, err)
		return "port open error", err
	}
	defer port.Close()

	port.ResetInputBuffer()
	port.ResetOutputBuffer()

	//time.Sleep(500 * time.Millisecond)

	_, err = port.Write([]byte(command))

	fmt.Print(command)

	if err != nil {
		log.Println(err)
		return "port write error", err

	}
	time.Sleep(100 * time.Millisecond)
	//port.SetReadTimeout(1 * time.Second)
	_, err = port.Read(read_data)

	if err != nil {
		log.Println(err)
		return "port read error", err

	}

	lines := strings.Split(string(read_data), "\r\n")

	fmt.Println(strings.TrimSpace(lines[0]))

	return lines[0], nil

}
