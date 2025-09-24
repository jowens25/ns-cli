package lib

import "C"
import (
	"bufio"
	"fmt"
	"os"
)

func ReadWriteMicro(command string) (string, error) {

	command = command + "\r\n"

	file, err := os.OpenFile(AppConfig.Serial.Port, os.O_RDWR, 0)
	if err != nil {
		return "unable to open for write", fmt.Errorf("failed to open for write")
	}
	defer file.Close()

	_, err = file.WriteString(command)

	if err != nil {
		return "unable to write", fmt.Errorf("failed to write")
	}

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		fmt.Println(scanner.Text())

	}

	return "nothing", nil
}

// command is the actual string so ex $BAUDNV
//func ReadWriteMicro(command string) (string, error) {
//
//	command = command + "\r\n"
//
//	mode := &serial.Mode{
//		BaudRate: AppConfig.Serial.Baudrate,
//		DataBits: 8,
//		Parity:   serial.NoParity,
//		StopBits: serial.OneStopBit,
//	}
//
//	read_data := make([]byte, 1024)
//
//	port, err := serial.Open(AppConfig.Serial.Port, mode)
//
//	if err != nil {
//		log.Println(AppConfig.Serial.Port, err)
//		return "port open error", err
//	}
//	defer port.Close()
//
//	port.ResetInputBuffer()
//	port.ResetOutputBuffer()
//
//	_, err = port.Write([]byte(command))
//
//	fmt.Print(command)
//
//	if err != nil {
//		log.Println(err)
//		return "port write error", err
//
//	}
//
//	err = port.SetReadTimeout(1 * time.Second)
//	if err != nil {
//		log.Println("Failed to set read timeout:", err)
//		return "", err
//	}
//
//	for {
//
//		n, err := port.Read(read_data)
//		if err != nil {
//			log.Println(err)
//		}
//
//		lines := string(read_data[:n])
//
//		if strings.Contains(lines, command[:3]) {
//			responses := strings.Split(lines, "\r\n")
//			for _, response := range responses {
//				if strings.Contains(response, command[:3]) {
//					return strings.TrimSpace(responses[0]), nil
//
//				}
//			}
//
//		}
//	}
//	return "timeout", err
//}
