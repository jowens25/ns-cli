package api

import (
	"fmt"
	"log"
	"time"

	"go.bug.st/serial"
)

var FileDescriptor string = "/dev/ttymxc2"

func SendRaw(rawString string) {

	write_data := make([]byte, 0, 64)
	read_data := make([]byte, 64)

	mode := &serial.Mode{
		BaudRate: 115200,
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	}

	port, err := serial.Open(FileDescriptor, mode)
	if err != nil {
		log.Fatal("serial open err: ", err)
	}
	defer port.Close()

	port.SetReadTimeout(time.Millisecond * 100)

	write_data = append(write_data, rawString...)
	write_data = append(write_data, '\r')
	write_data = append(write_data, '\n')

	n, err := port.Write(write_data)

	if err != nil {
		log.Fatal("write error: ", err)
	}

	if n == 0 {
		log.Fatal("response: none")
	}

	for read_data[len(read_data)] != '\n' {
		n, err = port.Read(read_data)
	}

	if err != nil {
		log.Fatal("read error: ", err)
	}

	if n == 0 {
		log.Fatal("response: none")
	}
	read_string := string(read_data)
	fmt.Println(read_string)
}
