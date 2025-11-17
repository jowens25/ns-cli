package lib

import "C"
import (
	"bufio"
	"log"
	"strings"
	"time"

	"go.bug.st/serial"
)

func ReadWriteMicro(command string) (string, error) {
	command = command + "\r\n"

	mode := &serial.Mode{
		BaudRate: AppConfig.Mcu.Baudrate,
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	}

	port, err := serial.Open(AppConfig.Mcu.Port, mode)

	if err != nil {
		log.Println(AppConfig.Mcu.Port, err)
		return "port open error", err
	}
	defer port.Close()

	port.ResetInputBuffer()
	port.ResetOutputBuffer()

	_, err = port.Write([]byte(command))

	//fmt.Print(command)

	if err != nil {
		log.Println(err)
		return "port write error", err

	}

	err = port.SetReadTimeout(1 * time.Second)
	if err != nil {
		log.Println("Failed to set read timeout:", err)
		return "", err
	}

	timeout := time.Now().Add(4000 * time.Millisecond)

	scanner := bufio.NewScanner(port)

	for scanner.Scan() && time.Now().Before(timeout) {
		line := scanner.Text() // reads line until \n
		//fmt.Println(line)      // prints full line
		if strings.Contains(line, "$ER") {
			return line, nil
		}
		if strings.Contains(line, "$RR") {
			return line, nil
		}
		if strings.Contains(line, "$WR") {
			return line, nil
		}
		if strings.Contains(line, "$GPNTL") {
			return line, nil
		}
	}
	if err := scanner.Err(); err != nil {
		log.Println("Error reading from serial port:", err)
	}
	return "timeout", nil
}
