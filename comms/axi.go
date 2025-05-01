package axi

/*

#include "axi.h"
#include "config.h"
#include "ntpServer.h"
*/
import "C"

func RunConnect() {

	out := C.CString("00000000000000000000000000000000")

	enabled := C.CString("enable")

	//enabled := C.CString("enable")

	C.connect()

	C.readStatus(out, 32)
	println("status: ", C.GoString(out))

	C.writeStatus(enabled, 32)

	C.readStatus(out, 32)
	println("status: ", C.GoString(out))

	//defer C.free(unsafe.Pointer(disabled))

	//defer C.free(unsafe.Pointer(enabled))
	//defer C.free(unsafe.Pointer(out))

}
