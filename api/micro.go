package api

import (
	"bufio"
	"bytes"
	"fmt"
	"log"
	"os"
	"strings"
)

func MicroWrite(command string, responseMarker string, parameter ...string) string {

	write_data := make([]byte, 0, 64)
	read_data := make([]byte, 64)

	mcu_port := "/dev/ttymxc2"
	//test_port := os.Stdout.Name()

	file, err := os.OpenFile(mcu_port, os.O_RDWR, 0)
	if err != nil {
		file.Close()
		log.Fatal(err)
	}
	defer file.Close()

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

	_, err = file.Write(write_data)
	if err != nil {
		fmt.Println(err)
	}

	for {

		_, err = file.Read(read_data)

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
	}

	//return "mcu error"

}
