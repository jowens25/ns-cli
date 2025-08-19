package api

/*
#include "mySerial.h"
*/
import "C"
import (
	"bufio"
	"bytes"
	"log"
	"strings"

	"go.bug.st/serial"
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

func ReadWriteMicro(command string, responseMarker string, parameter ...string) string {

	mode := &serial.Mode{
		BaudRate: 38400, // Adjust to match your device
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	}

	mcu_port := "/dev/ttymxc2"

	cmd := MakeCommand(command, parameter...)

	read_data := make([]byte, 64)

	port, err := serial.Open(mcu_port, mode)
	if err != nil {
		log.Fatal(err)
	}
	defer port.Close()

	port.ResetInputBuffer()
	port.ResetOutputBuffer()

	n, err := port.Write(cmd)

	if err != nil {
		log.Fatal(err)
	}

	if n > 0 {
		//fmt.Println("wrote: ", n, " bytes")
	}

	n, err = port.Read(read_data)

	if err != nil {
		log.Fatal(err)
	}

	if n > 0 {
		//fmt.Println("read: ", n, " bytes")
	}

	//fmt.Println(string(read_data))

	scanner := bufio.NewScanner(bytes.NewReader(read_data))
	// read all the lines, find placements
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, responseMarker) {
			return line
		}

	}

	return command + " error"
}

//func echoPort(cmd string, param ...string) {
//
//	exec.Command("echo", string(MakeCommand(cmd)))
//
//}
//
//func catPort(cmd string) {
//
//}
//
//func GrepIt(cmd string) {
//
//}
