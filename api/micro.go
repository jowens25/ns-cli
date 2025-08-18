package api

import (
	"bufio"
	"bytes"
	"fmt"
	"log"
	"os"
	"strings"
	"time"

	"go.bug.st/serial"
)

func MicroWrite(command string, responseMarker string, parameter ...string) string {

	write_data := make([]byte, 0, 64)
	read_data := make([]byte, 64)

	//mcu_port := "/dev/ttymxc2"
	mcu_port := os.Stdout.Name()

	mode := &serial.Mode{
		BaudRate: 115200,
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	}

	port, err := serial.Open(mcu_port, mode)
	if err != nil {
		port.Close()
		log.Fatal("serial open err: ", err)
	}
	defer port.Close()

	port.SetReadTimeout(time.Millisecond * 100)

	command = "$" + command

	if len(parameter) > 0 {
		command = command + "=" + parameter[0]
	}

	write_data = append(write_data, command...)
	checksum := CalculateChecksum(write_data)
	write_data = append(write_data, '*')
	write_data = append(write_data, checksum...)
	write_data = append(write_data, '\r')
	write_data = append(write_data, '\n')

	_, err = port.Write(write_data)
	if err != nil {
		fmt.Println(err)
	}

	_, err = port.Read(read_data)

	if err != nil {
		fmt.Println(err)
	}

	scanner := bufio.NewScanner(bytes.NewReader(read_data))

	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, responseMarker) {
			return line
		} else {
			return "No response?"
		}

	}
	return "err"
}
