package api

import (
	"fmt"
	"log"
	"os"
	"strings"
)

func MakeCommand(cmd string, param ...string) []byte {

	out := []byte("$" + cmd)

	if len(param) > 0 {
		out = append(out, '=')
	}

	checksum := CalculateChecksum(out)
	out = append(out, '*')
	out = append(out, checksum...)
	out = append(out, '\r')
	out = append(out, '\n')

	return out

}

func MicroWrite(command string, responseMarker string, parameter ...string) string {

	mcu_port := "/dev/ttymxc2"

	cmd := MakeCommand(command, parameter...)

	read_data := make([]byte, 64)
	temp_data := make([]byte, 64)

	f, err := os.OpenFile(mcu_port, os.O_RDWR, 0644)

	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	for {

		n, err := f.Read(temp_data)

		if err != nil {
			log.Fatal(err)
		}

		// reading zero i think would mean theres no nmea crap to get in the way...
		if n == 0 {

			n, err = f.Write(cmd)

			if err != nil {
				log.Fatal(err)
			}

			if n > 0 {
				fmt.Println("wrote: ", n, " bytes")
			}

			n, err = f.Read(read_data)

			if err != nil {
				log.Fatal(err)
			}

			if n > 0 {
				fmt.Println("read: ", n, " bytes")
			}

			fmt.Println(string(read_data))

			if strings.Contains(string(read_data), responseMarker) {
				return string(read_data)
			}

		}

	}

	return string(read_data)
}
