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
	"sync"
	"unsafe"
)

var mutex sync.Mutex

const size = C.size_t(64)

func Connect() error {
	fmt.Println("AXI CONNECT CALLED")
	err := C.axiConnect()
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
