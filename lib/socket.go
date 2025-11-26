package lib

import "C"
import (
	"bufio"
	"fmt"
	"log"
	"net"
	"strings"
	"time"
)

func ReadWriteSocket(command string) (string, error) {
	command = command + "\r\n"

	sock, err := net.Dial("unix", "/tmp/serial.sock")
	if err != nil {
		fmt.Println("socket error?")
		return "port open error", err

	}
	defer sock.Close()

	_, err = sock.Write([]byte(command))

	if err != nil {
		log.Println(err)
		return "port write error", err
	}

	timeout := time.Now().Add(4000 * time.Millisecond)

	scanner := bufio.NewScanner(sock)

	for scanner.Scan() && time.Now().Before(timeout) {
		line := scanner.Text() // reads line until \n
		fmt.Println(line)      // prints full line
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
