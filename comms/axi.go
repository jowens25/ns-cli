package axi

/*

#include "axi.h"
#include "config.h"
#include "ntpServer.h"
#include "coreConfig.h"
*/
import "C"
import "unsafe"

func test_unicast() {
	C.connect()
	C.connect()
	C.readConfig()
	out := C.CString("00000000000000000000000000000000")
	in := C.CString("disabled")

	C.readNtpServerUnicastMode(out, 32)
	println("unicast mode: ", C.GoString(out))

	C.writeNtpServerUnicastMode(in, 32)

	C.readNtpServerUnicastMode(out, 32)
	println("unicast mode: ", C.GoString(out))

	defer C.free(unsafe.Pointer(in))
	defer C.free(unsafe.Pointer(out))

}
func test_multicast() {
	C.connect()
	C.connect()
	C.readConfig()
	out := C.CString("00000000000000000000000000000000")
	in := C.CString("enabled")

	C.readNtpServerMulticastMode(out, 32)
	println("multicast mode: ", C.GoString(out))

	C.writeNtpServerMulticastMode(in, 32)

	C.readNtpServerMulticastMode(out, 32)
	println("multicast mode: ", C.GoString(out))

	defer C.free(unsafe.Pointer(in))
	defer C.free(unsafe.Pointer(out))

}

func test_broadcast() {
	C.connect()
	C.connect()
	C.readConfig()
	out := C.CString("00000000000000000000000000000000")
	in := C.CString("disabled")

	C.readNtpServerBroadcastMode(out, 32)
	println("Broadcast mode: ", C.GoString(out))

	C.writeNtpServerBroadcastMode(in, 32)

	C.readNtpServerBroadcastMode(out, 32)
	println("Broadcast mode: ", C.GoString(out))

	defer C.free(unsafe.Pointer(in))
	defer C.free(unsafe.Pointer(out))

}

func ListNtpProperties() {
	C.connect()
	C.connect()
	C.readConfig()

	out := C.CString("00000000000000000000000000000000")
	var size C.size_t = 64

	err := C.readNtpServerStatus(out, size)
	println("STATUS: ", C.GoString(out))
	//println("STATUS ERROR: ", err)
	err = C.readNtpServerMacAddress(out, size)
	println("MAC ADDRESS: ", C.GoString(out))
	//println("MAC ADDRESS ERROR: ", err)
	err = C.readNtpServerVlanStatus(out, size)
	println("VLAN STATUS: ", C.GoString(out))
	//println("VLAN STATUS ERROR: ", err)

	err = C.readNtpServerVlanAddress(out, size)
	println("VLAN ADDRESS: ", C.GoString(out))
	//println("VLAN ADDRESS ERROR: ", err)

	err = C.readNtpServerIpMode(out, size)
	println("IP MODE: ", C.GoString(out))
	//println("IP MODE ERROR: ", err)

	err = C.readNtpServerUnicastMode(out, size)
	println("UNICAST MODE: ", C.GoString(out))
	//println("UNICAST MODE ERROR: ", err)

	err = C.readNtpServerMulticastMode(out, size)
	println("MULTICAST MODE: ", C.GoString(out))
	//println("MULTICAST MODE ERROR: ", err)

	err = C.readNtpServerBroadcastMode(out, size)
	println("BROADCAST MODE: ", C.GoString(out))
	//println("BROADCAST MODE ERROR: ", err)

	err = C.readNtpServerPrecisionValue(out, size)
	println("PRECISION VALUE: ", C.GoString(out))
	//println("PRECISION VALUE ERROR: ", err)

	err = C.readNtpServerPollIntervalValue(out, size)
	println("POLL INTERVAL VALUE: ", C.GoString(out))
	//println("POLL INTERVAL VALUE ERROR: ", err)

	err = C.readNtpServerStratumValue(out, size)
	println("STRATUM VALUE: ", C.GoString(out))
	//println("STRATUM VALUE ERROR: ", err)

	err = C.readNtpServerReferenceId(out, size)
	println("REFERENCE ID VALUE: ", C.GoString(out))
	//println("REFERENCE ID VALUE ERROR: ", err)

	err = C.readNtpServerIpAddress(out, size)
	println("IP ADDRESS: ", C.GoString(out))
	//println("IP ADDRESS ERROR: ", err)

	err = C.readNtpServerSmearingStatus(out, size)
	println("Smearing Status: ", C.GoString(out))
	//println("Smearing Status ERROR: ", err)
	err = C.readNtpServerLeap61Progress(out, size)
	println("Leap 61 Progress: ", C.GoString(out))
	//println("Leap 61 Progress ERROR: ", err)

	err = C.readNtpServerLeap59Progress(out, size)
	println("Leap 59 Progress: ", C.GoString(out))
	println("Leap 59 Progress ERROR: ", err)
	defer C.free(unsafe.Pointer(out))
}

func RunConnect() {

	test_unicast()

	test_multicast()

	test_broadcast()

	//C.readNtpServerStatus(out, 32)
	//println("status: ", C.GoString(out))
	//
	//C.readNtpServerInstanceNumber(out, 32)
	//println("instance #: ", C.GoString(out))
	//
	//C.readNtpServerIpMode(out, 32)
	//println("ip mode: ", C.GoString(out))
	//C.readNtpServerIpAddress(out, 32)
	//println("ip addr: ", C.GoString(out))
	//
	//C.readNtpServerMacAddress(out, 32)
	//println("mac addr: ", C.GoString(out))
	//
	//C.readNtpServerVlanStatus(out, 32)
	//println("vlan status: ", C.GoString(out))
	//
	//C.readNtpServerVlanAddress(out, 32)
	//println("vlan status: ", C.GoString(out))
	//
	//C.readNtpServerUnicastMode(out, 32)
	//println("unicast mode: ", C.GoString(out))
	//C.readNtpServerMulticastMode(out, 32)
	//println("multicast mode: ", C.GoString(out))
	//C.readNtpServerBroadcastMode(out, 32)
	//println("broadcast mode: ", C.GoString(out))
	//
	//C.readNtpServerPrecisionValue(out, 32)
	//println("precision value: ", C.GoString(out))
	//
	//C.readNtpServerPollIntervalValue(out, 32)
	//println("poll interval value: ", C.GoString(out))
	//
	//C.readNtpServerStratumValue(out, 32)
	//println("stratum value: ", C.GoString(out))
	//
	//C.readNtpServerReferenceId(out, 32)
	//println("Reference Id Value: ", C.GoString(out))
	//
	//C.readNtpServerSmearingStatus(out, 32)
	//println("Smearing Status: ", C.GoString(out))
	//
	//C.readNtpServerLeap61Progress(out, 32)
	//println("Leap 61 Progress: ", C.GoString(out))
	//C.readNtpServerLeap59Progress(out, 32)
	//println("Leap 59 Progress: ", C.GoString(out))
	//C.readNtpServerLeap61Status(out, 32)
	//println("Leap 61 Status: ", C.GoString(out))
	//C.readNtpServerLeap59Status(out, 32)
	//println("Leap 59 Status: ", C.GoString(out))
	//C.readNtpServerUtcOffsetStatus(out, 32)
	//println("Utc Offset Status: ", C.GoString(out))
	//C.readNtpServerUtcOffsetValue(out, 32)
	//println("Utc Offset Value: ", C.GoString(out))
	//
	//C.readNtpServerRequestsValue(out, 32)
	//println("Requests Value: ", C.GoString(out))
	//C.readNtpServerResponsesValue(out, 32)
	//println("Responses Value: ", C.GoString(out))
	//C.readNtpServerRequestsDroppedValue(out, 32)
	//println("Requests Dropped Value: ", C.GoString(out))
	//C.readNtpServerBroadcastsValue(out, 32)
	//println("Broadcasts Value: ", C.GoString(out))
	//C.readNtpServerClearCountersStatus(out, 32)
	//println("Clear Counters Status: ", C.GoString(out))
	//C.readNtpServerVersion(out, 32)
	//println("Version: ", C.GoString(out))

}
