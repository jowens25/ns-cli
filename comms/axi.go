package axi

/*

#include "axi.h"
#include "config.h"
#include "ntpServer.h"
*/
import "C"
import "unsafe"

func RunConnect() {

	cs := C.CString("disable")
	defer C.free(unsafe.Pointer(cs))

	out := C.CString("")
	defer C.free(unsafe.Pointer(out))

	C.connect()

	C.readStatus(out)
	println(*out)

	C.writeStatus(cs)

	C.readStatus(out)
	println(*out)

	//C.connect()
	//
	//C.readConfig()
	//
	//println("end of read config")
	//C.CString
	//
	//C.readStatus(temp_data)
	//
	//print(temp_data)
}
