package api

import (
	"fmt"
	"log"
	"os"
)

func SendRaw(rawString string) {

	//buffer := make([]byte, 128)
	write_data := make([]byte, 0, 64)

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

	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	buffer := make([]byte, 0, 256)
	for {
		n, err := file.Read(buffer)
		if err != nil {
			break
		}
		if n > 0 {
			fmt.Print(string(buffer[:n]))
		}
	}

}
