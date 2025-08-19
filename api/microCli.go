package api

/*
#include "mySerial.h"
*/
import "C"
import (
	"fmt"
	"log"
	"strings"

	"go.bug.st/serial"
)

//func ReadMicro(command *string) error {
//
//	c := C.CString(*command)
//
//	defer C.free(unsafe.Pointer(c))
//
//	axiErr := C.ReadValue(c)
//
//	//*value = C.GoString(val)
//
//	if axiErr != 0 {
//		return errors.New("axi failed")
//	}
//
//	return nil
//}
//
//func WriteMicro(command *string, parameter *string) error {
//
//	c := C.CString(*command)
//	p := C.CString(*parameter)
//
//	defer C.free(unsafe.Pointer(c))
//	defer C.free(unsafe.Pointer(p))
//
//	axiErr := C.WriteValue(c, p)
//
//	//*value = C.GoString(val)
//
//	if axiErr != 0 {
//		return errors.New("axi failed")
//	}
//
//	return nil
//}

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
		fmt.Println("wrote: ", n, " bytes")
	}

	n, err = port.Read(read_data)

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

	return string(read_data)
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
