package axi

/*
#include "axi.h"
#include "cores.h"
#include "ntpServer.h"
#include "ptpOc.h"
#include "ppsSlave.h"
*/
import "C"
import (
	"errors"
	"fmt"
	"log"
	"os"
	"strings"
	"sync"
	"unsafe"
)

const (
	FPGA_PORT = "FPGA_PORT"
)

var mutex sync.Mutex

const size = C.size_t(64)

func Connect() error {
	fmt.Println("AXI CONNECT CALLED")
	err := C.connect()
	if err != 0 {
		return errors.New("failed to connect to serial port")
	}
	return nil
}

func Reset() error {
	//err := C.reset()

	//if err != 0 {
	//	return errors.New("failed to reset device")
	//}
	return nil
}

func GetCores() error {
	err := C.getCores()
	if err != 0 {

		return errors.New("failed to read config: " + fmt.Sprint(err))
	}
	return nil
}

func LoadConfig(fileName string) {

	fmt.Println("LOAD CONFIG CALLED")

	err := Connect()
	if err != nil {
		log.Println(err)
		log.Println("load config failed")
	}

	data, err := os.ReadFile(fileName)
	if err != nil {
		log.Println("file err: ", err)
	}

	for _, line := range strings.Split(string(data), "\n") {
		//fmt.Println(line)

		if strings.Contains(line, "--") {
			// a comment
			continue

		} else if strings.Contains(line, "$WC") {

			line = strings.Trim(line, "\r\n")
			lineParts := strings.Split(line, ",")

			addr := C.CString(lineParts[1])
			data := C.CString(lineParts[2])

			C.RawWrite(addr, data)
		}

	}

}

func Operation(operation *string, module *string, property *string, value *string) error {

	fmt.Println("OPERATE CALLED")

	op := C.CString(*operation)
	mod := C.CString(*module)
	prop := C.CString(*property)
	val := C.CString(*value)

	defer C.free(unsafe.Pointer(op))
	defer C.free(unsafe.Pointer(mod))
	defer C.free(unsafe.Pointer(prop))
	defer C.free(unsafe.Pointer(val))

	axiErr := C.Axi(op, mod, prop, val)

	*value = C.GoString(val)

	if axiErr != 0 {
		return errors.New("axi failed")
	}

	return nil
}
