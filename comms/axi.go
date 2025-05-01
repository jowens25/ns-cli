package axi

/*

#include "axi.h"
#include "config.h"
#include "ntpServer.h"
*/
import "C"
import "unsafe"

func RunConnect() {

	out := C.CString("00000000000000000000000000000000")

	C.connect()
	C.connect()

	C.readNtpServerStatus(out, 32)
	println("status: ", C.GoString(out))

	C.readNtpServerInstanceNumber(out, 32)
	println("instance #: ", C.GoString(out))

	C.readNtpServerIpMode(out, 32)
	println("ip mode: ", C.GoString(out))
	C.readNtpServerIpAddress(out, 32)
	println("ip addr: ", C.GoString(out))

	C.readNtpServerMacAddress(out, 32)
	println("mac addr: ", C.GoString(out))

	C.readNtpServerStatus(out, 32)
	println("vlan status: ", C.GoString(out))

	defer C.free(unsafe.Pointer(out))

}
