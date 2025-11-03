package lib

import "C"
import (
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

	err = port.SetReadTimeout(1 * time.Millisecond)
	if err != nil {
		log.Println("Failed to set read timeout:", err)
		return "", err
	}

	port.ResetInputBuffer()

	_, err = port.Write([]byte(command))
	if err != nil {
		log.Println("Write error:", err)
		return "", err
	}

	var response strings.Builder

	buff := make([]byte, 1024)
	timeout := time.Now().Add(500 * time.Millisecond) // Overall timeout

	for time.Now().Before(timeout) {

		n, err := port.Read(buff)
		if err != nil {
			break
		}
		if n > 0 {
			response.Write(buff[:n])
		}

		for _, marker := range []string{"$ER", "$CR", "$WR", "$RR"} {
			if strings.Contains(response.String(), marker) && strings.HasSuffix(response.String(), "\n") {
				return response.String(), nil
			}
		}

	}

	return "timeout", nil
}
