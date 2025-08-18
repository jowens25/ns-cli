package api

import (
	"fmt"
	"os"
)

func MicroWrite(command string, responseMarker string, parameter ...string) string {

	//write_data := make([]byte, 256)
	//read_data := make([]byte, 256)

	mcu_port := "/dev/ttymxc2"
	//mcu_port = os.Stdout.Name()

	file, err := os.OpenFile(mcu_port, os.O_WRONLY, 0644)

	if err != nil {
		fmt.Println(err)
		return "err"
	}
	defer file.Close()

	command = "$" + command

	if len(parameter) > 0 {
		command = command + "=" + parameter[0]
	}

	write_data := []byte(command)

	checksum := CalculateChecksum(write_data)
	write_data = append(write_data, '*')
	write_data = append(write_data, checksum...)
	write_data = append(write_data, '\r')
	write_data = append(write_data, '\n')

	_, err = file.Write([]byte(write_data))
	if err != nil {
		fmt.Println(err)
	}

	file.Close()

	//var read_byte byte
	//
	//_, err = port.Read(read_byte)
	//
	//if err != nil {
	//	fmt.Println(err)
	//}
	//
	//scanner := bufio.NewScanner(bytes.NewReader(read_data))
	//
	//for scanner.Scan() {
	//	line := scanner.Text()
	//
	//	if strings.Contains(line, responseMarker) {
	//		return line
	//	} else {
	//		return "No response?"
	//	}
	//
	//}
	//return "err"

	return "end"
}
