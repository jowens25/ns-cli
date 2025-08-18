package api

import (
	"bufio"
	"bytes"
	"fmt"
	"log"
	"os"
	"strings"
)

func SendRaw(rawString string) {

	//buffer := make([]byte, 128)
	write_data := make([]byte, 0, 64)
	read_data := make([]byte, 0, 64)

	file, err := os.OpenFile("/dev/ttymxc2", os.O_RDWR, 0)
	if err != nil {
		file.Close()
		log.Fatal(err)
	}
	defer file.Close()

	write_data = append(write_data, rawString...)

	//checksum := CalculateChecksum(write_data)
	//write_data = append(write_data, '*')
	//write_data = append(write_data, checksum...)
	write_data = append(write_data, '\r')
	write_data = append(write_data, '\n')

	n, err := file.Write(write_data)
	fmt.Println(err)
	fmt.Println("wrote: ", n)

	for {
		n, err := file.Read(read_data)
		if err != nil {
			break
		}
		if n > 0 {
			//fmt.Print(string(buffer[:n]))
			break
		}
	}

	scanner := bufio.NewScanner(bytes.NewReader(read_data))

	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, rawString) {
			fmt.Println(line)
		} else {
			fmt.Println("No response?")
		}

	}

}
