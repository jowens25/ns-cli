package axi

/*
#include "axi.h"
#include "config.h"
#include "ntpServer.h"
#include "ptpOc.h"
#include "ppsSlave.h"
#include "coreConfig.h"
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

func init() {

	err := Connect()
	if err != nil {
		panic(err)
	}

	err = ReadConfig()
	if err != nil {
		panic(err)
	}

	//LoadConfig("PtpGmNtpServer.ucm")
}

func Connect() error {
	err := C.connect()
	if err != 0 {
		return errors.New("failed to connect to serial port")
	}
	return nil
}

func ReadConfig() error {
	err := C.readConfig()
	if err != 0 {

		return errors.New("failed to read config: " + fmt.Sprint(err))
	}
	return nil
}

func LoadConfig(fileName string) {

	err := Connect()
	if err != nil {
		panic(err)
	}

	data, err := os.ReadFile(fileName)
	if err != nil {
		log.Fatal("file err: ", err)
	}

	for _, line := range strings.Split(string(data), "\n") {
		//fmt.Println(i, line)

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

func Operate(operation *string, module *string, property *string, value *string) error {
	err := Connect()
	if err != nil {
		fmt.Println(err)
	}
	op := C.CString(*operation)
	mod := C.CString(*module)
	prop := C.CString(*property)
	val := C.CString(*value)

	defer C.free(unsafe.Pointer(op))
	defer C.free(unsafe.Pointer(mod))
	defer C.free(unsafe.Pointer(prop))
	defer C.free(unsafe.Pointer(val))

	//mutex.Lock()
	//defer mutex.Unlock()

	//err := C.connect()
	//C.readConfig()
	//
	//if err != 0 {
	//	return errors.New("connection failed")
	//}

	axiErr := C.Axi(op, mod, prop, val)

	*value = C.GoString(val)

	if axiErr != 0 {
		return errors.New("axi failed")
	}

	return nil
}

//func Write(module string, property string, value *string) {
//	fmt.Println("module: ", module)
//	fmt.Println("property: ", property)
//
//	C.Axi(C.CString(module), C.CString(property), value)
//
//}

//func ReadPpsSlave(property string) string {
//	start := time.Now()
//	out := (*C.char)(C.calloc(size, 1))
//	mutex.Lock()
//
//	switch property {
//
//	case PpsSlave.Version:
//		C.readPpsSlaveVersion(out, size)
//	case PpsSlave.InstanceNumber:
//		C.readPpsSlaveInstanceNumber(out, size)
//	case PpsSlave.EnableStatus:
//		C.readPpsSlaveEnableStatus(out, size)
//	case PpsSlave.InvertedStatus:
//		C.readPpsSlaveInvertedStatus(out, size)
//	case PpsSlave.InputOkStatus:
//		C.readPpsSlaveInputOkStatus(out, size)
//	case PpsSlave.PulseWidthValue:
//		C.readPpsSlavePulseWidthValue(out, size)
//	case PpsSlave.CableDelayValue:
//		C.readPpsSlaveCableDelayValue(out, size)
//	default:
//		fmt.Println("no such property")
//	}
//	mutex.Unlock()
//	defer C.free(unsafe.Pointer(out))
//
//	fmt.Println(property, "r : ", time.Since(start))
//
//	return C.GoString(out)
//}
//
//func WritePpsSlave(property string, value string) {
//	start := time.Now()
//	in := C.CString(value)
//	mutex.Lock()
//
//	switch property {
//
//	case PpsSlave.EnableStatus:
//		err := C.writePpsSlaveEnableStatus(in, size)
//		if err != 0 {
//			fmt.Println("write error: 	PpsSlave.EnableStatus,")
//		}
//	case PpsSlave.InvertedStatus:
//		fmt.Println("inverted")
//		err := C.writePpsSlaveInvertedStatus(in, size)
//		if err != 0 {
//			fmt.Println("write error: 	PpsSlave.InvertedStatus,")
//		}
//
//	case PpsSlave.CableDelayValue:
//		err := C.writePpsSlaveCableDelayValue(in, size)
//		if err != 0 {
//			fmt.Println("write error: 	PpsSlave.CableDelayValue,")
//		}
//	default:
//		fmt.Println("no such property / read only")
//	}
//	mutex.Unlock()
//	defer C.free(unsafe.Pointer(in))
//	fmt.Println(property, "w : ", time.Since(start))
//
//}
//
//func ReadNtpServer(property string) string {
//	start := time.Now()
//	out := (*C.char)(C.calloc(size, 1))
//	mutex.Lock()
//
//	switch property {
//
//	case NtpServer.Status:
//		C.readNtpServerStatus(out, size)
//	case NtpServer.InstanceNumber:
//		C.readNtpServerInstanceNumber(out, size)
//	case NtpServer.IpMode:
//		C.readNtpServerIpMode(out, size)
//	case NtpServer.IpAddress:
//		C.readNtpServerIpAddress(out, size)
//	case NtpServer.MacAddress:
//		C.readNtpServerMacAddress(out, size)
//	case NtpServer.VlanStatus:
//		C.readNtpServerVlanStatus(out, size)
//	case NtpServer.VlanAddress:
//		C.readNtpServerVlanAddress(out, size)
//	case NtpServer.UnicastMode:
//		C.readNtpServerUnicastMode(out, size)
//	case NtpServer.MulticastMode:
//		C.readNtpServerMulticastMode(out, size)
//	case NtpServer.BroadcastMode:
//		C.readNtpServerBroadcastMode(out, size)
//	case NtpServer.PrecisionValue:
//		C.readNtpServerPrecisionValue(out, size)
//	case NtpServer.PollIntervalValue:
//		C.readNtpServerPollIntervalValue(out, size)
//	case NtpServer.StratumValue:
//		C.readNtpServerStratumValue(out, size)
//	case NtpServer.ReferenceId:
//		C.readNtpServerReferenceId(out, size)
//	case NtpServer.SmearingStatus:
//		C.readNtpServerSmearingStatus(out, size)
//	case NtpServer.Leap61Progress:
//		C.readNtpServerLeap61Progress(out, size)
//	case NtpServer.Leap59Progress:
//		C.readNtpServerLeap59Progress(out, size)
//	case NtpServer.Leap61Status:
//		C.readNtpServerLeap61Status(out, size)
//	case NtpServer.Leap59Status:
//		C.readNtpServerLeap59Status(out, size)
//	case NtpServer.UtcOffsetStatus:
//		C.readNtpServerUtcOffsetStatus(out, size)
//	case NtpServer.UtcOffsetValue:
//		C.readNtpServerUtcOffsetValue(out, size)
//	case NtpServer.RequestsValue:
//		C.readNtpServerRequestsValue(out, size)
//	case NtpServer.ResponsesValue:
//		C.readNtpServerResponsesValue(out, size)
//	case NtpServer.RequestsDroppedValue:
//		C.readNtpServerRequestsDroppedValue(out, size)
//	case NtpServer.BroadcastsValue:
//		C.readNtpServerBroadcastsValue(out, size)
//	case NtpServer.ClearCountersStatus:
//		C.readNtpServerClearCountersStatus(out, size)
//	case NtpServer.Version:
//		C.readNtpServerVersion(out, size)
//
//	default:
//		fmt.Println("no such property")
//	}
//	mutex.Unlock()
//	defer C.free(unsafe.Pointer(out))
//
//	fmt.Println(property, "r : ", time.Since(start))
//
//	return C.GoString(out)
//
//}
//
//func WriteNtpServer(property string, value string) {
//	start := time.Now()
//	in := C.CString(value)
//	mutex.Lock()
//
//	switch property {
//	case NtpServer.Status:
//		err := C.writeNtpServerStatus(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerStatus ERROR: ", err)
//		}
//
//	case NtpServer.MacAddress:
//		err := C.writeNtpServerMacAddress(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerMacAddress ERROR: ", err)
//		}
//	case NtpServer.VlanStatus:
//		err := C.writeNtpServerVlanStatus(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerVlanStatus ERROR: ", err)
//		}
//	case NtpServer.VlanAddress:
//		err := C.writeNtpServerVlanAddress(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerVlanAddress ERROR: ", err)
//		}
//	case NtpServer.IpMode:
//		err := C.writeNtpServerIpMode(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerIpMode ERROR: ", err)
//		}
//	case NtpServer.IpAddress:
//		err := C.writeNtpServerIpAddress(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerIpAddress ERROR: ", err)
//		}
//	case NtpServer.UnicastMode:
//		err := C.writeNtpServerUnicastMode(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerUnicastMode ERROR: ", err)
//		}
//	case NtpServer.MulticastMode:
//		err := C.writeNtpServerMulticastMode(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerMulticastMode ERROR: ", err)
//		}
//	case NtpServer.BroadcastMode:
//		err := C.writeNtpServerBroadcastMode(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerBroadcastMode ERROR: ", err)
//		}
//	case NtpServer.PrecisionValue:
//		err := C.writeNtpServerPrecisionValue(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerPrecisionValue ERROR: ", err)
//		}
//	case NtpServer.PollIntervalValue:
//		err := C.writeNtpServerPollIntervalValue(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerPollIntervalValue ERROR: ", err)
//		}
//	case NtpServer.StratumValue:
//		err := C.writeNtpServerStratumValue(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerStratumValue ERROR: ", err)
//		}
//	case NtpServer.ReferenceId:
//		err := C.writeNtpServerReferenceIdValue(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerReferenceIdValue ERROR: ", err)
//		}
//	case NtpServer.SmearingStatus:
//		err := C.writeNtpServerUtcSmearingStatus(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerUtcSmearingStatus ERROR: ", err)
//		}
//	case NtpServer.Leap61Status:
//		err := C.writeNtpServerLeap61Status(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerLeap61Status ERROR: ", err)
//		}
//	case NtpServer.Leap59Status:
//		err := C.writeNtpServerLeap59Status(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerLeap59Status ERROR: ", err)
//		}
//	case NtpServer.UtcOffsetStatus:
//		err := C.writeNtpServerUtcOffsetStatus(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerUtcOffsetStatus ERROR: ", err)
//		}
//	case NtpServer.UtcOffsetValue:
//		err := C.writeNtpServerUtcOffsetValue(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerUtcOffsetValue ERROR: ", err)
//		}
//	case NtpServer.ClearCountersStatus:
//		err := C.writeNtpServerClearCountersStatus(in, size)
//		if err != 0 {
//			fmt.Println("writeNtpServerClearCountersStatus ERROR: ", err)
//		}
//
//	default:
//		fmt.Println("no such property / read only")
//	}
//	mutex.Unlock()
//	defer C.free(unsafe.Pointer(in))
//	fmt.Println(property, "w : ", time.Since(start))
//
//}
//
////	func TestPpsSlaveModule(property string) {
////		buf := (*C.char)(C.calloc(size, 1))
////		mutex.Lock()
////		switch property {
////
////		case PpsSlave.Version:
////			C.readPpsSlaveVersion(buf, size)
////		case PpsSlave.InstanceNumber:
////			C.readPpsSlaveInstanceNumber(buf, size)
////		case PpsSlave.EnableStatus:
////			C.readPpsSlaveEnableStatus(buf, size)
////		case PpsSlave.InvertedStatus:
////			C.readPpsSlaveInvertedStatus(buf, size)
////		case PpsSlave.InputOkStatus:
////			C.readPpsSlaveInputOkStatus(buf, size)
////		case PpsSlave.PulseWidthValue:
////			C.readPpsSlavePulseWidthValue(buf, size)
////		case PpsSlave.CableDelayValue:
////			C.readPpsSlaveCableDelayValue(buf, size)
////		case PpsSlave.CableDelayValue:
////			C.writePpsSlaveCableDelayValue(buf, size)
////		case PpsSlave.CableDelayValue:
////			C.writePpsSlaveCableDelayValue(buf, size)
////		case PpsSlave.InvertedStatus:
////			C.writePpsSlaveInvertedStatus(buf, size)
////		case PpsSlave.EnableStatus:
////			C.writePpsSlaveEnableStatus(buf, size)
////		default:
////			fmt.Println("no such pps slave property / read only")
////		}
////
////		mutex.Unlock()
////		defer C.free(unsafe.Pointer(buf))
////
////		updatedData, err := json.MarshalIndent(PtpOc, "", "  ")
////		if err != nil {
////			log.Fatalf("Error marshaling: %v", err)
////		}
//
//		err = os.WriteFile("/home/jowens/Projects/NovusTimeServer/comms/ptpOc.json", updatedData, 0644)
//		if err != nil {
//			log.Fatalf("Error writing file: %v", err)
//		}
//
//		return C.GoString(out)
//	}
//func ReadPtpOc(property string) string {
//	out := (*C.char)(C.calloc(size, 1))
//	mutex.Lock()
//
//	switch property {
//
//	case PtpOc.Version:
//		C.readPtpOcVersion(out, size)
//	//	//PtpOc.Version = C.GoString(out)
//
//	case PtpOc.Status:
//		C.readPtpOcStatus(out, size)
//	//	PtpOc.Status = C.GoString(out)
//
//	case PtpOc.VlanStatus:
//		C.readPtpOcVlanStatus(out, size)
//	//	PtpOc.VlanStatus = C.GoString(out)
//
//	case PtpOc.VlanAddress:
//		C.readPtpOcVlanAddress(out, size)
//	//	PtpOc.VlanAddress = C.GoString(out)
//
//	case PtpOc.Profile:
//		C.readPtpOcProfile(out, size)
//	//	PtpOc.Profile = C.GoString(out)
//
//	case PtpOc.DefaultDsTwoStepStatus:
//		C.readPtpOcDefaultDsTwoStepStatus(out, size)
//	//	PtpOc.DefaultDsTwoStepStatus = C.GoString(out)
//
//	case PtpOc.DefaultDsSignalingStatus:
//		C.readPtpOcDefaultDsSignalingStatus(out, size)
//	//	PtpOc.DefaultDsSignalingStatus = C.GoString(out)
//
//	case PtpOc.Layer:
//		C.readPtpOcLayer(out, size)
//	//	PtpOc.Layer = C.GoString(out)
//
//	case PtpOc.DefaultDsSlaveOnlyStatus:
//		C.readPtpOcDefaultDsSlaveOnlyStatus(out, size)
//	//	PtpOc.DefaultDsSlaveOnlyStatus = C.GoString(out)
//
//	case PtpOc.DefaultDsMasterOnlyStatus:
//		C.readPtpOcDefaultDsMasterOnlyStatus(out, size)
//	//	PtpOc.DefaultDsMasterOnlyStatus = C.GoString(out)
//
//	case PtpOc.DefaultDsDisableOffsetCorrectionStatus:
//		C.readPtpOcDefaultDsDisableOffsetCorrectionStatus(out, size)
//	//	PtpOc.DefaultDsDisableOffsetCorrectionStatus = C.GoString(out)
//
//	case PtpOc.DefaultDsListedUnicastSlavesOnlyStatus:
//		C.readPtpOcDefaultDsListedUnicastSlavesOnlyStatus(out, size)
//	//	PtpOc.DefaultDsListedUnicastSlavesOnlyStatus = C.GoString(out)
//
//	case PtpOc.DelayMechanismValue:
//		C.readPtpOcDelayMechanismValue(out, size)
//	//	PtpOc.DelayMechanismValue = C.GoString(out)
//
//	case PtpOc.IpAddress:
//		C.readPtpOcIpAddress(out, size)
//	//	PtpOc.IpAddress = C.GoString(out)
//
//	case PtpOc.DefaultDsClockId:
//		C.readPtpOcDefaultDsClockId(out, size)
//	//	PtpOc.DefaultDsClockId = C.GoString(out)
//
//	case PtpOc.DefaultDsDomain:
//		C.readPtpOcDefaultDsDomain(out, size)
//	//	PtpOc.DefaultDsDomain = C.GoString(out)
//
//	case PtpOc.DefaultDsPriority1:
//		C.readPtpOcDefaultDsPriority1(out, size)
//	//	PtpOc.DefaultDsPriority1 = C.GoString(out)
//
//	case PtpOc.DefaultDsPriority2:
//		C.readPtpOcDefaultDsPriority2(out, size)
//	//	PtpOc.DefaultDsPriority2 = C.GoString(out)
//
//	case PtpOc.DefaultDsVariance:
//		C.readPtpOcDefaultDsVariance(out, size)
//	//	PtpOc.DefaultDsVariance = C.GoString(out)
//
//	case PtpOc.DefaultDsAccuracy:
//		C.readPtpOcDefaultDsAccuracy(out, size)
//	//	PtpOc.DefaultDsAccuracy = C.GoString(out)
//
//	case PtpOc.DefaultDsClass:
//		C.readPtpOcDefaultDsClass(out, size)
//	//	PtpOc.DefaultDsClass = C.GoString(out)
//
//	case PtpOc.DefaultDsShortId:
//		C.readPtpOcDefaultDsShortId(out, size)
//	//	PtpOc.DefaultDsShortId = C.GoString(out)
//
//	case PtpOc.DefaultDsInaccuracy:
//		C.readPtpOcDefaultDsInaccuracy(out, size)
//	//	PtpOc.DefaultDsInaccuracy = C.GoString(out)
//
//	case PtpOc.DefaultDsNumberOfPorts:
//		C.readPtpOcDefaultDsNumberOfPorts(out, size)
//	//	PtpOc.DefaultDsNumberOfPorts = C.GoString(out)
//
//	//
//	//
//	case PtpOc.PortDsPeerDelayValue:
//		C.readPtpOcPortDsPeerDelayValue(out, size)
//	//	PtpOc.PortDsPeerDelayValue = C.GoString(out)
//
//	case PtpOc.PortDsState:
//		C.readPtpOcPortDsState(out, size)
//	//	PtpOc.PortDsState = C.GoString(out)
//
//	case PtpOc.PortDsPDelayReqLogMsgIntervalValue:
//		C.readPtpOcPortDsPDelayReqLogMsgIntervalValue(out, size)
//	//	PtpOc.PortDsPDelayReqLogMsgIntervalValue = C.GoString(out)
//
//	case PtpOc.PortDsDelayReqLogMsgIntervalValue:
//		C.readPtpOcPortDsDelayReqLogMsgIntervalValue(out, size)
//	//	PtpOc.PortDsDelayReqLogMsgIntervalValue = C.GoString(out)
//
//	case PtpOc.PortDsAnnounceReceiptTimeoutValue:
//		C.readPtpOcPortDsAnnounceReceiptTimeoutValue(out, size)
//
//	case PtpOc.PortDsAnnounceLogMsgIntervalValue:
//		C.readPtpOcPortDsAnnounceLogMsgIntervalValue(out, size)
//
//	case PtpOc.PortDsSyncReceiptTimeoutValue:
//		C.readPtpOcPortDsSyncReceiptTimeoutValue(out, size)
//
//	case PtpOc.PortDsSyncLogMsgIntervalValue:
//		C.readPtpOcPortDsSyncLogMsgIntervalValue(out, size)
//
//	case PtpOc.PortDsDelayReceiptTimeoutValue:
//		C.readPtpOcPortDsDelayReceiptTimeoutValue(out, size)
//
//	case PtpOc.PortDsAsymmetryValue:
//		C.readPtpOcPortDsAsymmetryValue(out, size)
//
//	case PtpOc.PortDsMaxPeerDelayValue:
//		C.readPtpOcPortDsMaxPeerDelayValue(out, size)
//
//	case PtpOc.CurrentDsStepsRemovedValue:
//		C.readPtpOcCurrentDsStepsRemovedValue(out, size)
//
//	case PtpOc.CurrentDsOffsetValue:
//		C.readPtpOcCurrentDsOffsetValue(out, size)
//
//	case PtpOc.CurrentDsDelayValue:
//		C.readPtpOcCurrentDsDelayValue(out, size)
//
//	// parent dataset
//	case PtpOc.ParentDsParentClockIdValue:
//		C.readPtpOcParentDsParentClockIdValue(out, size)
//
//	case PtpOc.ParentDsGmClockIdValue:
//		C.readPtpOcParentDsGmClockIdValue(out, size)
//	//	PtpOc.ParentDsGmClockIdValue = C.GoString(out)
//
//	case PtpOc.ParentDsGmPriority1Value:
//		C.readPtpOcParentDsGmPriority1Value(out, size)
//	//	PtpOc.ParentDsGmPriority1Value = C.GoString(out)
//
//	case PtpOc.ParentDsGmPriority2Value:
//		C.readPtpOcParentDsGmPriority2Value(out, size)
//	//	PtpOc.ParentDsGmPriority2Value = C.GoString(out)
//
//	case PtpOc.ParentDsGmVarianceValue:
//		C.readPtpOcParentDsGmVarianceValue(out, size)
//	//	PtpOc.ParentDsGmVarianceValue = C.GoString(out)
//
//	case PtpOc.ParentDsGmAccuracyValue:
//		C.readPtpOcParentDsGmAccuracyValue(out, size)
//	//	PtpOc.ParentDsGmAccuracyValue = C.GoString(out)
//
//	case PtpOc.ParentDsGmClassValue:
//		C.readPtpOcParentDsGmClassValue(out, size)
//	//	PtpOc.ParentDsGmClassValue = C.GoString(out)
//
//	case PtpOc.ParentDsGmShortIdValue:
//		C.readPtpOcParentDsGmShortIdValue(out, size)
//	//	PtpOc.ParentDsGmShortIdValue = C.GoString(out)
//
//	case PtpOc.ParentDsGmInaccuracyValue:
//		C.readPtpOcParentDsGmInaccuracyValue(out, size)
//	//	PtpOc.ParentDsGmInaccuracyValue = C.GoString(out)
//
//	case PtpOc.ParentDsNwInaccuracyValue:
//		C.readPtpOcParentDsNwInaccuracyValue(out, size)
//	//	PtpOc.ParentDsNwInaccuracyValue = C.GoString(out)
//
//	// time properties
//	case PtpOc.TimePropertiesDsTimeSourceValue:
//		C.readPtpOcTimePropertiesDsTimeSourceValue(out, size)
//	//	PtpOc.TimePropertiesDsTimeSourceValue = C.GoString(out)
//
//	case PtpOc.TimePropertiesDsPtpTimescaleStatus:
//		C.readPtpOcTimePropertiesDsPtpTimescaleStatus(out, size)
//	//	PtpOc.TimePropertiesDsPtpTimescaleStatus = C.GoString(out)
//	case PtpOc.TimePropertiesDsFreqTraceableStatus:
//		C.readPtpOcTimePropertiesDsFreqTraceableStatus(out, size)
//	//	PtpOc.TimePropertiesDsFreqTraceableStatus = C.GoString(out)
//	case PtpOc.TimePropertiesDsTimeTraceableStatus:
//		C.readPtpOcTimePropertiesDsTimeTraceableStatus(out, size)
//	//	PtpOc.TimePropertiesDsTimeTraceableStatus = C.GoString(out)
//	case PtpOc.TimePropertiesDsLeap61Status:
//		C.readPtpOcTimePropertiesDsLeap61Status(out, size)
//	//	PtpOc.TimePropertiesDsLeap61Status = C.GoString(out)
//	case PtpOc.TimePropertiesDsLeap59Status:
//		C.readPtpOcTimePropertiesDsLeap59Status(out, size)
//	//	PtpOc.TimePropertiesDsLeap59Status = C.GoString(out)
//	case PtpOc.TimePropertiesDsUtcOffsetValStatus:
//		C.readPtpOcTimePropertiesDsUtcOffsetValStatus(out, size)
//	//	PtpOc.TimePropertiesDsUtcOffsetValStatus = C.GoString(out)
//	case PtpOc.TimePropertiesDsUtcOffsetValue:
//		C.readPtpOcTimePropertiesDsUtcOffsetValue(out, size)
//	//	PtpOc.TimePropertiesDsUtcOffsetValue = C.GoString(out)
//	case PtpOc.TimePropertiesDsCurrentOffsetValue:
//		C.readPtpOcTimePropertiesDsCurrentOffsetValue(out, size)
//	//	PtpOc.TimePropertiesDsCurrentOffsetValue = C.GoString(out)
//	case PtpOc.TimePropertiesDsJumpSecondsValue:
//		C.readPtpOcTimePropertiesDsJumpSecondsValue(out, size)
//	//	PtpOc.TimePropertiesDsJumpSecondsValue = C.GoString(out)
//	case PtpOc.TimePropertiesDsNextJumpValue:
//		C.readPtpOcTimePropertiesDsNextJumpValue(out, size)
//	//	PtpOc.TimePropertiesDsNextJumpValue = C.GoString(out)
//	case PtpOc.TimePropertiesDsDisplayNameValue:
//		C.readPtpOcTimePropertiesDsDisplayNameValue(out, size)
//	//	PtpOc.TimePropertiesDsDisplayNameValue = C.GoString(out)
//
//	default:
//		fmt.Println("no such property")
//	}
//
//	mutex.Unlock()
//	defer C.free(unsafe.Pointer(out))
//
//	updatedData, err := json.MarshalIndent(PtpOc, "", "  ")
//	if err != nil {
//		log.Fatalf("Error marshaling: %v", err)
//	}
//
//	err = os.WriteFile("/home/jowens/Projects/NovusTimeServer/comms/ptpOc.json", updatedData, 0644)
//	if err != nil {
//		log.Fatalf("Error writing file: %v", err)
//	}
//
//	return C.GoString(out)
//}
//
//func WritePtpOc(property string, value string) {
//	start := time.Now()
//
//	in := C.CString(value)
//
//	mutex.Lock()
//
//	switch property {
//
//	case PtpOc.Profile:
//		err := C.writePtpOcProfile(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcProfile ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsTwoStepStatus:
//		err := C.writePtpOcDefaultDsTwoStepStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsTwoStepStatus ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsSignalingStatus:
//		err := C.writePtpOcDefaultDsSignalingStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsSignalingStatus ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsSlaveOnlyStatus:
//		err := C.writePtpOcDefaultDsSlaveOnlyStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsSlaveOnlyStatus ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsMasterOnlyStatus:
//		err := C.writePtpOcDefaultDsMasterOnlyStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsMasterOnlyStatus ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsDisableOffsetCorrectionStatus:
//		err := C.writePtpOcDefaultDsDisableOffsetCorrectionStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsDisableOffsetCorStatus ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsListedUnicastSlavesOnlyStatus:
//		err := C.writePtpOcDefaultDsListedUnicastSlavesOnlyStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsListedUnicastSlavesOnlyStatus ERROR: ", err)
//		}
//
//	case PtpOc.Layer:
//		err := C.writePtpOcLayer(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcLayer ERROR: ", err)
//		}
//
//	case PtpOc.DelayMechanismValue:
//		err := C.writePtpOcDelayMechanismValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDelayMechanismValue ERROR: ", err)
//		}
//
//	case PtpOc.VlanAddress:
//		err := C.writePtpOcVlanAddress(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcVlanAddress ERROR: ", err)
//		}
//
//	case PtpOc.VlanStatus:
//		err := C.writePtpOcVlanStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcVlanStatus ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsClockId:
//		err := C.writePtpOcDefaultDsClockIdValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsClockIdValue ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsDomain:
//		err := C.writePtpOcDefaultDsDomainValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsDomainValue ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsPriority1:
//		err := C.writePtpOcDefaultDsPriority1Value(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsPriority1Value ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsPriority2:
//		err := C.writePtpOcDefaultDsPriority2Value(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsPriority2Value ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsClass:
//		err := C.writePtpOcDefaultDsClassValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsClassValue ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsAccuracy:
//		err := C.writePtpOcDefaultDsAccuracyValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsAccuracyValue ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsVariance:
//		err := C.writePtpOcDefaultDsVarianceValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsVarianceValue ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsShortId:
//		err := C.writePtpOcDefaultDsShortIdValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsShortIdValue ERROR: ", err)
//		}
//
//	case PtpOc.DefaultDsInaccuracy:
//		err := C.writePtpOcDefaultDsInaccuracyValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcDefaultDsInaccuracyValue ERROR: ", err)
//		}
//
//	case PtpOc.PortDsDelayReceiptTimeoutValue:
//		err := C.writePtpOcPortDsDelayReceiptTimeoutValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcPortDsDelayReceiptTimeoutValue ERROR: ", err)
//		}
//		//
//		//
//	case PtpOc.PortDsDelayReqLogMsgIntervalValue:
//		err := C.writePtpOcPortDsDelayReqLogMsgIntervalValue(in, size)
//		if err != 0 {
//			fmt.Println("writePortDsDelayReqLogMsgIntervalValue ERROR: ", err)
//		}
//
//	case PtpOc.PortDsPDelayReqLogMsgIntervalValue:
//		err := C.writePtpOcPortDsPDelayReqLogMsgIntervalValue(in, size)
//		if err != 0 {
//			fmt.Println("writePortDsPDelayReqLogMsgIntervalValue ERROR: ", err)
//		}
//	case PtpOc.PortDsAnnounceReceiptTimeoutValue:
//		err := C.writePtpOcPortDsAnnounceReceiptTimeoutValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcPortDsAnnounceReceiptTimeoutValue ERROR: ", err)
//		}
//
//	case PtpOc.PortDsAnnounceLogMsgIntervalValue:
//		err := C.writePtpOcPortDsAnnounceLogMsgIntervalValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcPtpOcPortDsAnnounceLogMsgIntervalValue ERROR: ", err)
//		}
//
//	case PtpOc.PortDsSyncReceiptTimeoutValue:
//		err := C.writePtpOcPortDsSyncReceiptTimeoutValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcPortDsSyncReceiptTimeoutValue ERROR: ", err)
//		}
//
//	case PtpOc.PortDsSyncLogMsgIntervalValue:
//		err := C.writePtpOcPortDsSyncLogMsgIntervalValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcPortDsSyncLogMsgIntervalValue ERROR: ", err)
//		}
//
//	case PtpOc.PortDsAsymmetryValue:
//		err := C.writePtpOcPortDsAsymmetryValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcPortDsAsymmetryValue ERROR: ", err)
//		}
//
//	case PtpOc.PortDsMaxPeerDelayValue:
//		err := C.writePtpOcPortDsMaxPeerDelayValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcPortDsMaxPeerDelayValue ERROR: ", err)
//		}
//	case PtpOc.TimePropertiesDsTimeSourceValue:
//		err := C.writePtpOcTimePropertiesDsTimeSourceValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsTimeSourceValue ERROR: ", err)
//		}
//	case PtpOc.TimePropertiesDsPtpTimescaleStatus:
//		err := C.writePtpOcTimePropertiesDsPtpTimescaleStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsPtpTimescaleStatus ERROR: ", err)
//		}
//
//	case PtpOc.TimePropertiesDsFreqTraceableStatus:
//		err := C.writePtpOcTimePropertiesDsFreqTraceableStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsFreqTraceableStatus ERROR: ", err)
//		}
//	case PtpOc.TimePropertiesDsTimeTraceableStatus:
//		err := C.writePtpOcTimePropertiesDsTimeTraceableStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsTimeTraceableStatus ERROR: ", err)
//		}
//
//	case PtpOc.TimePropertiesDsLeap61Status:
//		err := C.writePtpOcTimePropertiesDsLeap61Status(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsLeap61Status ERROR: ", err)
//		}
//	case PtpOc.TimePropertiesDsLeap59Status:
//		err := C.writePtpOcTimePropertiesDsLeap59Status(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsLeap59Status ERROR: ", err)
//		}
//	case PtpOc.TimePropertiesDsUtcOffsetValStatus:
//		err := C.writePtpOcTimePropertiesDsUtcOffsetValStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsUtcOffsetValStatus ERROR: ", err)
//		}
//	case PtpOc.TimePropertiesDsUtcOffsetValue:
//		err := C.writePtpOcTimePropertiesDsUtcOffsetValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsUtcOffsetValue ERROR: ", err)
//		}
//	case PtpOc.TimePropertiesDsCurrentOffsetValue:
//		err := C.writePtpOcTimePropertiesDsCurrentOffsetValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsCurrentOffsetValue ERROR: ", err)
//		}
//	case PtpOc.TimePropertiesDsJumpSecondsValue:
//		err := C.writePtpOcTimePropertiesDsJumpSecondsValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsJumpSecondsValue ERROR: ", err)
//		}
//	case PtpOc.TimePropertiesDsNextJumpValue:
//		err := C.writePtpOcTimePropertiesDsNextJumpValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsNextJumpValue ERROR: ", err)
//		}
//	case PtpOc.TimePropertiesDsDisplayNameValue:
//		err := C.writePtpOcTimePropertiesDsDisplayNameValue(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcTimePropertiesDsDisplayNameValue ERROR: ", err)
//		}
//
//	case PtpOc.Status:
//		err := C.writePtpOcStatus(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcStatus ERROR: ", err)
//		}
//
//	case PtpOc.IpAddress:
//		err := C.writePtpOcIpAddress(in, size)
//		if err != 0 {
//			fmt.Println("writePtpOcIpAddress ERROR: ", err)
//		}
//		//
//		//
//	default:
//		fmt.Println("NO SUCH WRITE PROPERTY")
//	}
//	mutex.Unlock()
//	defer C.free(unsafe.Pointer(in))
//	fmt.Println(property, "w : ", time.Since(start))
//
//}
//
//func ListPtpOcProperties() error {
//
//	properties := []string{
//		PtpOc.Version,
//		PtpOc.Status,
//		PtpOc.VlanStatus,
//		PtpOc.VlanAddress,
//		PtpOc.Profile,
//		PtpOc.DefaultDsTwoStepStatus,
//		PtpOc.DefaultDsSignalingStatus,
//		PtpOc.Layer,
//		PtpOc.DefaultDsSlaveOnlyStatus,
//		PtpOc.DefaultDsMasterOnlyStatus,
//		PtpOc.DefaultDsDisableOffsetCorrectionStatus,
//		PtpOc.DefaultDsListedUnicastSlavesOnlyStatus,
//		PtpOc.DelayMechanismValue,
//		PtpOc.IpAddress,
//		PtpOc.DefaultDsClockId,
//		PtpOc.DefaultDsDomain,
//		PtpOc.DefaultDsPriority1,
//		PtpOc.DefaultDsPriority2,
//		PtpOc.DefaultDsVariance,
//		PtpOc.DefaultDsAccuracy,
//		PtpOc.DefaultDsClass,
//		PtpOc.DefaultDsShortId,
//		PtpOc.DefaultDsInaccuracy,
//		PtpOc.DefaultDsNumberOfPorts,
//		PtpOc.PortDsPeerDelayValue,
//		PtpOc.PortDsState,
//		PtpOc.PortDsPDelayReqLogMsgIntervalValue,
//		PtpOc.PortDsDelayReqLogMsgIntervalValue,
//		PtpOc.PortDsAnnounceReceiptTimeoutValue,
//		PtpOc.PortDsAnnounceLogMsgIntervalValue,
//		PtpOc.PortDsSyncReceiptTimeoutValue,
//		PtpOc.PortDsSyncLogMsgIntervalValue,
//		PtpOc.PortDsDelayReceiptTimeoutValue,
//		PtpOc.PortDsAsymmetryValue,
//		PtpOc.PortDsMaxPeerDelayValue,
//		PtpOc.CurrentDsStepsRemovedValue,
//		PtpOc.CurrentDsOffsetValue,
//		PtpOc.CurrentDsDelayValue,
//		PtpOc.ParentDsParentClockIdValue,
//		PtpOc.ParentDsGmClockIdValue,
//		PtpOc.ParentDsGmPriority1Value,
//		PtpOc.ParentDsGmPriority2Value,
//		PtpOc.ParentDsGmVarianceValue,
//		PtpOc.ParentDsGmAccuracyValue,
//		PtpOc.ParentDsGmClassValue,
//		PtpOc.ParentDsGmShortIdValue,
//		PtpOc.ParentDsGmInaccuracyValue,
//		PtpOc.ParentDsNwInaccuracyValue,
//		PtpOc.TimePropertiesDsTimeSourceValue,
//		PtpOc.TimePropertiesDsPtpTimescaleStatus,
//		PtpOc.TimePropertiesDsFreqTraceableStatus,
//		PtpOc.TimePropertiesDsTimeTraceableStatus,
//		PtpOc.TimePropertiesDsLeap61Status,
//		PtpOc.TimePropertiesDsLeap59Status,
//		PtpOc.TimePropertiesDsUtcOffsetValStatus,
//		PtpOc.TimePropertiesDsUtcOffsetValue,
//		PtpOc.TimePropertiesDsCurrentOffsetValue,
//		PtpOc.TimePropertiesDsJumpSecondsValue,
//		PtpOc.TimePropertiesDsNextJumpValue,
//		PtpOc.TimePropertiesDsDisplayNameValue}
//
//	file, err := os.Create("ptp_properties.txt")
//	if err != nil {
//		return fmt.Errorf("failed to create file: %w", err)
//	}
//	defer file.Close()
//
//	for _, property := range properties {
//		current := ReadPtpOc(property)
//		// Write property name and value to file
//		_, err := fmt.Fprintf(file, "%s: %v\n", property, current)
//		if err != nil {
//			return fmt.Errorf("failed to write to file: %w", err)
//		}
//	}
//	return nil
//}
//
//func TestPtpProperties() {
//
//	properties := []string{
//		PtpOc.Version,
//		PtpOc.Status,
//		PtpOc.VlanStatus,
//		PtpOc.VlanAddress,
//		PtpOc.Profile,
//		PtpOc.DefaultDsTwoStepStatus,
//		PtpOc.DefaultDsSignalingStatus,
//		PtpOc.Layer,
//		PtpOc.DefaultDsSlaveOnlyStatus,
//		PtpOc.DefaultDsMasterOnlyStatus,
//		PtpOc.DefaultDsDisableOffsetCorrectionStatus,
//		PtpOc.DefaultDsListedUnicastSlavesOnlyStatus,
//		PtpOc.DelayMechanismValue,
//		PtpOc.IpAddress,
//		PtpOc.DefaultDsClockId,
//		PtpOc.DefaultDsDomain,
//		PtpOc.DefaultDsPriority1,
//		PtpOc.DefaultDsPriority2,
//		PtpOc.DefaultDsVariance,
//		PtpOc.DefaultDsAccuracy,
//		PtpOc.DefaultDsClass,
//		PtpOc.DefaultDsShortId,
//		PtpOc.DefaultDsInaccuracy,
//		PtpOc.DefaultDsNumberOfPorts,
//		PtpOc.PortDsPeerDelayValue,
//		PtpOc.PortDsState,
//		PtpOc.PortDsPDelayReqLogMsgIntervalValue,
//		PtpOc.PortDsDelayReqLogMsgIntervalValue,
//		PtpOc.PortDsAnnounceReceiptTimeoutValue,
//		PtpOc.PortDsAnnounceLogMsgIntervalValue,
//		PtpOc.PortDsSyncReceiptTimeoutValue,
//		PtpOc.PortDsSyncLogMsgIntervalValue,
//		PtpOc.PortDsDelayReceiptTimeoutValue,
//		PtpOc.PortDsAsymmetryValue,
//		PtpOc.PortDsMaxPeerDelayValue,
//		PtpOc.CurrentDsStepsRemovedValue,
//		PtpOc.CurrentDsOffsetValue,
//		PtpOc.CurrentDsDelayValue,
//		PtpOc.ParentDsParentClockIdValue,
//		PtpOc.ParentDsGmClockIdValue,
//		PtpOc.ParentDsGmPriority1Value,
//		PtpOc.ParentDsGmPriority2Value,
//		PtpOc.ParentDsGmVarianceValue,
//		PtpOc.ParentDsGmAccuracyValue,
//		PtpOc.ParentDsGmClassValue,
//		PtpOc.ParentDsGmShortIdValue,
//		PtpOc.ParentDsGmInaccuracyValue,
//		PtpOc.ParentDsNwInaccuracyValue,
//		PtpOc.TimePropertiesDsTimeSourceValue,
//		PtpOc.TimePropertiesDsPtpTimescaleStatus,
//		PtpOc.TimePropertiesDsFreqTraceableStatus,
//		PtpOc.TimePropertiesDsTimeTraceableStatus,
//		PtpOc.TimePropertiesDsLeap61Status,
//		PtpOc.TimePropertiesDsLeap59Status,
//		PtpOc.TimePropertiesDsUtcOffsetValStatus,
//		PtpOc.TimePropertiesDsUtcOffsetValue,
//		PtpOc.TimePropertiesDsCurrentOffsetValue,
//		PtpOc.TimePropertiesDsJumpSecondsValue,
//		PtpOc.TimePropertiesDsNextJumpValue,
//		PtpOc.TimePropertiesDsDisplayNameValue}
//
//	for _, property := range properties {
//		//fmt.Println(p, " : ", ReadPtpOc(p))
//		value := "0"
//		// read - current
//		current := ReadPtpOc(property)
//		fmt.Println(property, " ", current)
//		// update
//		WritePtpOc(property, value)
//		// read - check if new == requested
//		new := ReadPtpOc(property)
//		fmt.Println("new value: ", new)
//		if new == value {
//			fmt.Println(property, " ", new)
//			WritePtpOc(property, current)
//
//			fmt.Println("TEST PASSED!!")
//			fmt.Println("Changed back to starting value: ", property, " ", ReadPtpOc(property))
//		} else {
//			fmt.Println("TEST FAILED")
//		}
//	}
//}
//
