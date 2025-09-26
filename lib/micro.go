package lib

import "C"
import (
	"bufio"
	"fmt"
	"log"
	"os/exec"
)

//func ReadWriteMicro(command string) (string, error) {
//
//	command = command + "\r\n"
//
//	file, err := os.OpenFile(AppConfig.Serial.Port, os.O_RDWR, 0)
//	if err != nil {
//		return "unable to open for write", fmt.Errorf("failed to open for write")
//	}
//	defer file.Close()
//
//	_, err = file.WriteString(command)
//
//	if err != nil {
//		return "unable to write", fmt.Errorf("failed to write")
//	}
//
//	buffer := make([]byte, 1024)
//
//	scanner := bufio.NewScanner(file)
//
//	for scanner.Scan() {
//		fmt.Println(scanner.Text())
//
//		buffer = append(buffer, scanner.Text()...)
//
//		if len(buffer) >= 1000 {
//			break
//		}
//	}
//	fmt.Println(string(buffer))
//
//	return "nothing", nil
//}

func ReadWriteMicro(command string) (string, error) {

	command = command + "\r\n"

	// 1. Write the command to the serial port
	writeCmd := exec.Command("sh", "-c", fmt.Sprintf("echo '%s' > '%s''", command, AppConfig.Serial.Port))
	if err := writeCmd.Run(); err != nil {
		fmt.Printf("Failed to write to serial port: %v", err)
	}

	// 2. Read from the serial port using `cat`, but limit to 5 lines
	readCmd := exec.Command("cat", AppConfig.Serial.Port)

	// Get stdout pipe
	stdout, err := readCmd.StdoutPipe()
	if err != nil {
		fmt.Printf("Failed to get stdout pipe: %v", err)
	}

	// Start the read command
	if err := readCmd.Start(); err != nil {
		fmt.Printf("Failed to start read command: %v", err)
	}

	// Read lines
	scanner := bufio.NewScanner(stdout)
	lineCount := 0
	for scanner.Scan() {
		fmt.Printf("%s", scanner.Text())
		lineCount++
		if lineCount >= 5 {
			break
		}
	}

	// Stop the cat process
	if err := readCmd.Process.Kill(); err != nil {
		log.Printf("Failed to kill read process: %v", err)
	} else {
		log.Println("Read process killed after 5 lines.")
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Scanner error: %v", err)
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
