package api

import (
	"bufio"
	"bytes"
	"fmt"
	"log"
	"os"
)

func SendRaw(command string, responseMarker string, parameter ...string) {

	//buffer := make([]byte, 128)
	write_data := make([]byte, 256)
	read_data := make([]byte, 256)

	file, err := os.OpenFile("/dev/ttymxc2", os.O_RDWR, 0)
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

	_, err = file.Read(read_data)

	if err != nil {
		fmt.Println(err)
	}

	fmt.Println(string(read_data))

	scanner := bufio.NewScanner(bytes.NewReader(read_data))

	for scanner.Scan() {
		line := scanner.Text()

		fmt.Println(line) // print first line??

	}

}
