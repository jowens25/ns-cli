package axi

/*

#include "axi.h"
#include "config.h"
#include "ntpServer.h"
#include "coreConfig.h"
*/
import "C"
import (
	"fmt"
	"unsafe"
)

func ListNtpProperties() {

	C.connect()

	C.readConfig()

	out := C.CString("0000000000000000000000000000000000000000000000000000000000000000")
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
	//println("Leap 59 Progress ERROR: ", err)

	err = C.readNtpServerLeap61Status(out, size)
	println("Leap 61 Status: ", C.GoString(out))
	//println("Leap 61 Status ERROR: ", err)

	err = C.readNtpServerLeap59Status(out, size)
	println("Leap 59 Status: ", C.GoString(out))
	//println("Leap 59 Status ERROR: ", err)

	err = C.readNtpServerUtcOffsetStatus(out, size)
	println("UTC OFFSET Status: ", C.GoString(out))
	//println("UTC OFFSET Status ERROR: ", err)

	err = C.readNtpServerUtcOffsetValue(out, size)
	println("UTC OFFSET Value: ", C.GoString(out))
	//println("UTC OFFSET Value ERROR: ", err)

	err = C.readNtpServerRequestsValue(out, size)
	println("REQUESTS Value: ", C.GoString(out))
	//println("REQUESTS Value ERROR: ", err)

	err = C.readNtpServerResponsesValue(out, size)
	println("RESPONSES Value: ", C.GoString(out))
	//println("RESPONSES Value ERROR: ", err)

	err = C.readNtpServerRequestsDroppedValue(out, size)
	println("Requests Dropped Value", C.GoString(out))
	//println("Requests Dropped Value ERROR: ", err)
	err = C.readNtpServerBroadcastsValue(out, size)
	println("Broadcasts Value", C.GoString(out))
	//println("Broadcasts Value ERROR: ", err)
	err = C.readNtpServerClearCountersStatus(out, size)
	println("Clear Counters Status", C.GoString(out))
	//println("Clear Counters Status ERROR: ", err)
	err = C.readNtpServerVersion(out, size)
	println("Version", C.GoString(out))
	//println("Version ERROR: ", err)
	err = C.readNtpServerInstanceNumber(out, size)
	println("InstanceNumber", C.GoString(out))
	println("InstanceNumber ERROR: ", err)
	//
	defer C.free(unsafe.Pointer(out))
}

func ToggleNtpIpMode(addr string) {
	C.connect()

	C.readConfig()

	in := C.CString(addr)
	var size C.size_t = 64
	//ListNtpProperties()
	err := C.writeNtpServerIpMode(in, size)
	//ListNtpProperties()

	println("err: ", err)
	defer C.free(unsafe.Pointer(in))

}

func SetNtpIpAddress(addr string) {
	C.connect()

	C.readConfig()
	fmt.Println("this is the ip you are trying to use: ", addr)
	in := C.CString(addr)
	var size C.size_t = 64
	err := C.writeNtpServerIpAddress(in, size)

	println("err: ", err)
	defer C.free(unsafe.Pointer(in))

}

func SetNtpReferenceId(id string) {
	C.connect()

	C.readConfig()
	fmt.Println("this is the id you are trying to use: ", id)
	in := C.CString(id)
	var size C.size_t = 64
	err := C.writeNtpServerReferenceIdValue(in, size)

	println("err: ", err)
	defer C.free(unsafe.Pointer(in))

}

func SetNtpSmearingStatus(status string) {
	C.connect()

	C.readConfig()
	fmt.Println("this is the status you are trying to use: ", status)
	in := C.CString(status)
	var size C.size_t = 64
	err := C.writeNtpServerUtcSmearingStatus(in, size)

	println("err: ", err)
	defer C.free(unsafe.Pointer(in))

}

func SetNtpLeap61Status(status string) {
	C.connect()

	C.readConfig()
	fmt.Println("this is the leap status you are trying to use: ", status)
	in := C.CString(status)
	var size C.size_t = 64
	err := C.writeNtpServerLeap61Status(in, size)

	println("err: ", err)
	defer C.free(unsafe.Pointer(in))

}

func SetNtpLeap59Status(status string) {
	C.connect()

	C.readConfig()
	fmt.Println("this is the leap 59 status you are trying to use: ", status)
	in := C.CString(status)
	var size C.size_t = 64
	err := C.writeNtpServerLeap59Status(in, size)

	println("err: ", err)
	defer C.free(unsafe.Pointer(in))

}

func SetNtpOffsetStatus(status string) {
	C.connect()

	C.readConfig()
	fmt.Println("this is the offset status you are trying to use: ", status)
	in := C.CString(status)
	var size C.size_t = 64
	err := C.writeNtpServerUtcOffsetStatus(in, size)

	println("err: ", err)
	defer C.free(unsafe.Pointer(in))

}

func SetNtpOffsetValue(value string) {
	C.connect()

	C.readConfig()
	fmt.Println("this is the offset status you are trying to use: ", value)
	in := C.CString(value)
	var size C.size_t = 64
	err := C.writeNtpServerUtcOffsetValue(in, size)

	println("err: ", err)
	defer C.free(unsafe.Pointer(in))

}

func ClearNtpCounters(value string) {
	C.connect()

	C.readConfig()
	fmt.Println("this is the offset status you are trying to use: ", value)
	in := C.CString(value)
	var size C.size_t = 64
	err := C.writeNtpServerClearCountersStatus(in, size)

	println("err: ", err)
	defer C.free(unsafe.Pointer(in))

}
