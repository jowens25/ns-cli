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

	C.readNtpServerVlanStatus(out, 32)
	println("vlan status: ", C.GoString(out))

	C.readNtpServerVlanAddress(out, 32)
	println("vlan status: ", C.GoString(out))

	C.readNtpServerUnicastMode(out, 32)
	println("unicast mode: ", C.GoString(out))
	C.readNtpServerMulticastMode(out, 32)
	println("multicast mode: ", C.GoString(out))
	C.readNtpServerBroadcastMode(out, 32)
	println("broadcast mode: ", C.GoString(out))

	C.readNtpServerPrecisionValue(out, 32)
	println("precision value: ", C.GoString(out))

	C.readNtpServerPollIntervalValue(out, 32)
	println("poll interval value: ", C.GoString(out))

	C.readNtpServerStratumValue(out, 32)
	println("stratum value: ", C.GoString(out))

	C.readNtpServerReferenceId(out, 32)
	println("Reference Id Value: ", C.GoString(out))

	C.readNtpServerSmearingStatus(out, 32)
	println("Smearing Status: ", C.GoString(out))

	C.readNtpServerLeap61Progress(out, 32)
	println("Leap 61 Progress: ", C.GoString(out))
	C.readNtpServerLeap59Progress(out, 32)
	println("Leap 59 Progress: ", C.GoString(out))
	C.readNtpServerLeap61Status(out, 32)
	println("Leap 61 Status: ", C.GoString(out))
	C.readNtpServerLeap59Status(out, 32)
	println("Leap 59 Status: ", C.GoString(out))
	C.readNtpServerUtcOffsetStatus(out, 32)
	println("Utc Offset Status: ", C.GoString(out))
	C.readNtpServerUtcOffsetValue(out, 32)
	println("Utc Offset Value: ", C.GoString(out))

	defer C.free(unsafe.Pointer(out))

}
