package axi

/*

#include "axi.h"
#include "config.h"
#include "ntpServer.h"
#include "ptpOc.h"
#include "coreConfig.h"
*/
import "C"
import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"sync"
	"time"
	"unsafe"
)

type NtpServerApi struct {
	Status               string `json:"status"`
	InstanceNumber       string `json:"instance"`
	IpMode               string `json:"ip-mode"`
	IpAddress            string `json:"ip-address"`
	MacAddress           string `json:"mac-address"`
	VlanStatus           string `json:"vlan-status"`
	VlanAddress          string `json:"vlan-address"`
	UnicastMode          string `json:"unicast"`
	MulticastMode        string `json:"multicast"`
	BroadcastMode        string `json:"broadcast"`
	PrecisionValue       string `json:"precision"`
	PollIntervalValue    string `json:"poll-interval"`
	StratumValue         string `json:"stratum"`
	ReferenceId          string `json:"reference-id"`
	SmearingStatus       string `json:"smearing-status"`
	Leap61Progress       string `json:"leap61-progress"`
	Leap59Progress       string `json:"leap59-progress"`
	Leap61Status         string `json:"leap61-status"`
	Leap59Status         string `json:"leap59-status"`
	UtcOffsetStatus      string `json:"utc-offset-status"`
	UtcOffsetValue       string `json:"utc-offset"`
	RequestsValue        string `json:"requests"`
	ResponsesValue       string `json:"responses"`
	RequestsDroppedValue string `json:"requestsdropped"`
	BroadcastsValue      string `json:"broadcasts"`
	ClearCountersStatus  string `json:"clearcounters"`
	Version              string `json:"version"`
	Root                 string `json:"ntp-server"`
}

var PtpOcMap map[string]string

type PtpOcApi struct {
	Root                                   string `json:"ptp-oc"`
	Version                                string `json:"version"`
	Status                                 string `json:"status"`
	VlanStatus                             string `json:"vlan-status"`
	VlanAddress                            string `json:"vlan-address"`
	Profile                                string `json:"profile"`
	DefaultDsTwoStepStatus                 string `json:"default-ds-two-step-status"`
	DefaultDsSignalingStatus               string `json:"default-ds-signaling-status"`
	Layer                                  string `json:"layer"`
	DefaultDsSlaveOnlyStatus               string `json:"default-ds-slave-only-status"`
	DefaultDsMasterOnlyStatus              string `json:"default-ds-master-only-status"`
	DefaultDsDisableOffsetCorrectionStatus string `json:"default-ds-disable-offset-correction-status"`
	DefaultDsListedUnicastSlavesOnlyStatus string `json:"default-ds-listed-unicast-slaves-only-status"`
	DelayMechanismValue                    string `json:"delay-mechanism-value"`
	IpAddress                              string `json:"ip-address"`
	DefaultDsClockId                       string `json:"default-ds-clock-id"`
	DefaultDsDomain                        string `json:"default-ds-domain"`
	DefaultDsPriority1                     string `json:"default-ds-priority1"`
	DefaultDsPriority2                     string `json:"default-ds-priority2"`
	DefaultDsVariance                      string `json:"default-ds-variance"`
	DefaultDsAccuracy                      string `json:"default-ds-accuracy"`
	DefaultDsClass                         string `json:"default-ds-class"`
	DefaultDsShortId                       string `json:"default-ds-shortid"`
	DefaultDsInaccuracy                    string `json:"default-ds-inaccuracy"`
	DefaultDsNumberOfPorts                 string `json:"default-ds-numberofports"`
	PortDsPeerDelayValue                   string `json:"port-ds-peer-delay-value"`
	PortDsState                            string `json:"port-ds-state"`
	PortDsPDelayReqLogMsgIntervalValue     string `json:"port-ds-p-delay-req-log-msg-interval-value"`
	PortDsDelayReqLogMsgIntervalValue      string `json:"port-ds-delay-req-log-msg-interval-value"`
	PortDsDelayReceiptTimeoutValue         string `json:"port-ds-delay-receipt-timeout-value"`
	PortDsAsymmetryValue                   string `json:"port-ds-asymmetry-value"`
	PortDsMaxPeerDelayValue                string `json:"port-ds-max-peer-delay-value"`
	CurrentDsStepsRemovedValue             string `json:"current-ds-steps-removed-value"`
	CurrentDsOffsetValue                   string `json:"current-ds-offset-value"`
	CurrentDsDelayValue                    string `json:"current-ds-delay-value"`
	ParentDsParentClockIdValue             string `json:"parent-ds-parent-clock-id-value"`
	ParentDsGmClockIdValue                 string `json:"parent-ds-gm-clock-id-value"`
	ParentDsGmPriority1Value               string `json:"parent-ds-gm-priority-1-value"`
	ParentDsGmPriority2Value               string `json:"parent-ds-gm-priority-2-value"`
	ParentDsGmVarianceValue                string `json:"parent-ds-gm-variance-value"`
	ParentDsGmAccuracyValue                string `json:"parent-ds-gm-accuracy-value"`
	ParentDsGmClassValue                   string `json:"parent-ds-gm-class-value"`
	ParentDsGmShortIdValue                 string `json:"parent-ds-gm-short-id-value"`
	ParentDsGmInaccuracyValue              string `json:"parent-ds-gm-inaccuracy-value"`
	ParentDsNwInaccuracyValue              string `json:"parent-ds-nw-inaccuracy-value"`
	TimePropertiesDsTimeSourceValue        string `json:"time-properties-ds-time-source-value"`
	TimePropertiesDsPtpTimescaleStatus     string `json:"time-properties-ds-ptp-time-scale-status"`
	TimePropertiesDsFreqTraceableStatus    string `json:"time-properties-ds-freq-traceable-status"`
	TimePropertiesDsTimeTraceableStatus    string `json:"time-properties-ds-time-traceable-status"`
	TimePropertiesDsLeap61Status           string `json:"time-properties-ds-leap61-status"`
	TimePropertiesDsLeap59Status           string `json:"time-properties-ds-leap59-status"`
	TimePropertiesDsUtcOffsetValStatus     string `json:"time-properties-ds-ut-coffset-val-status"`
	TimePropertiesDsUtcOffsetValue         string `json:"time-properties-ds-utc-offset-value"`
	TimePropertiesDsCurrentOffsetValue     string `json:"time-properties-ds-current-offset-value"`
	TimePropertiesDsJumpSecondsValue       string `json:"time-properties-ds-jump-seconds-value"`
	TimePropertiesDsNextJumpValue          string `json:"time-properties-ds-next-jump-value"`
	TimePropertiesDsDisplayNameValue       string `json:"time-properties-ds-display-name-value"`
}

var NtpServer = NtpServerApi{}

var PtpOc = PtpOcApi{}

var mutex sync.Mutex

const size = C.size_t(64)

func init() {
	jsonFile, err := os.ReadFile("/home/jowens/Projects/NovusTimeServer/comms/ptpOc.json")
	if err != nil {
		log.Fatalf("Error reading file: %v", err)
	}

	json.Unmarshal(jsonFile, &PtpOc)

	mutex.Lock()
	C.connect()
	C.readConfig()
	mutex.Unlock()

}

func ShowKeys() {
	fmt.Println(PtpOc)
	fmt.Println(PtpOc.Version["version"])
	//fmt.Println(ptpOcMap["ip-address"])
	//fmt.Println("version: ", ptpOcMap["ptp-oc"]["version"])

}

func ReadNtpServer(property string) string {
	start := time.Now()
	out := (*C.char)(C.calloc(size, 1))
	mutex.Lock()

	switch property {

	case NtpServer.Root:
		fmt.Println("readNtpServerRoot???: ")
		out = C.CString("NtpServer")
	case NtpServer.Status:
		C.readNtpServerStatus(out, size)
	case NtpServer.InstanceNumber:
		C.readNtpServerInstanceNumber(out, size)
	case NtpServer.IpMode:
		C.readNtpServerIpMode(out, size)
	case NtpServer.IpAddress:
		C.readNtpServerIpAddress(out, size)
	case NtpServer.MacAddress:
		C.readNtpServerMacAddress(out, size)
	case NtpServer.VlanStatus:
		C.readNtpServerVlanStatus(out, size)
	case NtpServer.VlanAddress:
		C.readNtpServerVlanAddress(out, size)
	case NtpServer.UnicastMode:
		C.readNtpServerUnicastMode(out, size)
	case NtpServer.MulticastMode:
		C.readNtpServerMulticastMode(out, size)
	case NtpServer.BroadcastMode:
		C.readNtpServerBroadcastMode(out, size)
	case NtpServer.PrecisionValue:
		C.readNtpServerPrecisionValue(out, size)
	case NtpServer.PollIntervalValue:
		C.readNtpServerPollIntervalValue(out, size)
	case NtpServer.StratumValue:
		C.readNtpServerStratumValue(out, size)
	case NtpServer.ReferenceId:
		C.readNtpServerReferenceId(out, size)
	case NtpServer.SmearingStatus:
		C.readNtpServerSmearingStatus(out, size)
	case NtpServer.Leap61Progress:
		C.readNtpServerLeap61Progress(out, size)
	case NtpServer.Leap59Progress:
		C.readNtpServerLeap59Progress(out, size)
	case NtpServer.Leap61Status:
		C.readNtpServerLeap61Status(out, size)
	case NtpServer.Leap59Status:
		C.readNtpServerLeap59Status(out, size)
	case NtpServer.UtcOffsetStatus:
		C.readNtpServerUtcOffsetStatus(out, size)
	case NtpServer.UtcOffsetValue:
		C.readNtpServerUtcOffsetValue(out, size)
	case NtpServer.RequestsValue:
		C.readNtpServerRequestsValue(out, size)
	case NtpServer.ResponsesValue:
		C.readNtpServerResponsesValue(out, size)
	case NtpServer.RequestsDroppedValue:
		C.readNtpServerRequestsDroppedValue(out, size)
	case NtpServer.BroadcastsValue:
		C.readNtpServerBroadcastsValue(out, size)
	case NtpServer.ClearCountersStatus:
		C.readNtpServerClearCountersStatus(out, size)
	case NtpServer.Version:
		C.readNtpServerVersion(out, size)

	default:
		fmt.Println("no such property")
	}
	mutex.Unlock()
	defer C.free(unsafe.Pointer(out))

	fmt.Println(property, "r : ", time.Since(start))

	return C.GoString(out)

}

func WriteNtpServer(property string, value string) {
	start := time.Now()

	in := C.CString(value)

	mutex.Lock()
	//C.connect()
	//C.readConfig()
	switch property {

	case NtpServer.Root:
		fmt.Println("writeNtpServerRoot???: ")

	case NtpServer.Status:
		err := C.writeNtpServerStatus(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerStatus ERROR: ", err)
		}

	case NtpServer.MacAddress:
		err := C.writeNtpServerMacAddress(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerMacAddress ERROR: ", err)
		}
	case NtpServer.VlanStatus:
		err := C.writeNtpServerVlanStatus(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerVlanStatus ERROR: ", err)
		}
	case NtpServer.VlanAddress:
		err := C.writeNtpServerVlanAddress(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerVlanAddress ERROR: ", err)
		}
	case NtpServer.IpMode:
		err := C.writeNtpServerIpMode(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerIpMode ERROR: ", err)
		}
	case NtpServer.IpAddress:
		err := C.writeNtpServerIpAddress(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerIpAddress ERROR: ", err)
		}
	case NtpServer.UnicastMode:
		err := C.writeNtpServerUnicastMode(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerUnicastMode ERROR: ", err)
		}
	case NtpServer.MulticastMode:
		err := C.writeNtpServerMulticastMode(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerMulticastMode ERROR: ", err)
		}
	case NtpServer.BroadcastMode:
		err := C.writeNtpServerBroadcastMode(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerBroadcastMode ERROR: ", err)
		}
	case NtpServer.PrecisionValue:
		err := C.writeNtpServerPrecisionValue(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerPrecisionValue ERROR: ", err)
		}
	case NtpServer.PollIntervalValue:
		err := C.writeNtpServerPollIntervalValue(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerPollIntervalValue ERROR: ", err)
		}
	case NtpServer.StratumValue:
		err := C.writeNtpServerStratumValue(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerStratumValue ERROR: ", err)
		}
	case NtpServer.ReferenceId:
		err := C.writeNtpServerReferenceIdValue(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerReferenceIdValue ERROR: ", err)
		}
	case NtpServer.SmearingStatus:
		err := C.writeNtpServerUtcSmearingStatus(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerUtcSmearingStatus ERROR: ", err)
		}
	case NtpServer.Leap61Status:
		err := C.writeNtpServerLeap61Status(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerLeap61Status ERROR: ", err)
		}
	case NtpServer.Leap59Status:
		err := C.writeNtpServerLeap59Status(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerLeap59Status ERROR: ", err)
		}
	case NtpServer.UtcOffsetStatus:
		err := C.writeNtpServerUtcOffsetStatus(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerUtcOffsetStatus ERROR: ", err)
		}
	case NtpServer.UtcOffsetValue:
		err := C.writeNtpServerUtcOffsetValue(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerUtcOffsetValue ERROR: ", err)
		}
	case NtpServer.ClearCountersStatus:
		err := C.writeNtpServerClearCountersStatus(in, size)
		if err != 0 {
			fmt.Println("writeNtpServerClearCountersStatus ERROR: ", err)
		}

	default:
		fmt.Println("no such property")
	}
	mutex.Unlock()
	defer C.free(unsafe.Pointer(in))
	fmt.Println(property, "w : ", time.Since(start))

}

func ReadPtpOc(property string) string {
	out := (*C.char)(C.calloc(size, 1))
	mutex.Lock()

	switch property {

	//case PtpOc.Version:
	//	C.readPtpOcVersion(out, size)
	//	PtpOc.Version = C.GoString(out)

	case PtpOc.Status:
		C.readPtpOcStatus(out, size)
		PtpOc.Status = C.GoString(out)

	case PtpOc.VlanStatus:
		C.readPtpOcVlanStatus(out, size)
		PtpOc.VlanStatus = C.GoString(out)

	case PtpOc.VlanAddress:
		C.readPtpOcVlanAddress(out, size)
		PtpOc.VlanAddress = C.GoString(out)

	case PtpOc.Profile:
		C.readPtpOcProfile(out, size)
		PtpOc.Profile = C.GoString(out)

	case PtpOc.DefaultDsTwoStepStatus:
		C.readPtpOcDefaultDsTwoStepStatus(out, size)
		PtpOc.DefaultDsTwoStepStatus = C.GoString(out)

	case PtpOc.DefaultDsSignalingStatus:
		C.readPtpOcDefaultDsSignalingStatus(out, size)
		PtpOc.DefaultDsSignalingStatus = C.GoString(out)

	case PtpOc.Layer:
		C.readPtpOcLayer(out, size)
		PtpOc.Layer = C.GoString(out)

	case PtpOc.DefaultDsSlaveOnlyStatus:
		C.readPtpOcDefaultDsSlaveOnlyStatus(out, size)
		PtpOc.DefaultDsSlaveOnlyStatus = C.GoString(out)

	case PtpOc.DefaultDsMasterOnlyStatus:
		C.readPtpOcDefaultDsMasterOnlyStatus(out, size)
		PtpOc.DefaultDsMasterOnlyStatus = C.GoString(out)

	case PtpOc.DefaultDsDisableOffsetCorrectionStatus:
		C.readPtpOcDefaultDsDisableOffsetCorrectionStatus(out, size)
		PtpOc.DefaultDsDisableOffsetCorrectionStatus = C.GoString(out)

	case PtpOc.DefaultDsListedUnicastSlavesOnlyStatus:
		C.readPtpOcDefaultDsListedUnicastSlavesOnlyStatus(out, size)
		PtpOc.DefaultDsListedUnicastSlavesOnlyStatus = C.GoString(out)

	case PtpOc.DelayMechanismValue:
		C.readPtpOcDelayMechanismValue(out, size)
		PtpOc.DelayMechanismValue = C.GoString(out)

	case PtpOc.IpAddress:
		C.readPtpOcIpAddress(out, size)
		PtpOc.IpAddress = C.GoString(out)

	case PtpOc.DefaultDsClockId:
		C.readPtpOcDefaultDsClockId(out, size)
		PtpOc.DefaultDsClockId = C.GoString(out)

	case PtpOc.DefaultDsDomain:
		C.readPtpOcDefaultDsDomain(out, size)
		PtpOc.DefaultDsDomain = C.GoString(out)

	case PtpOc.DefaultDsPriority1:
		C.readPtpOcDefaultDsPriority1(out, size)
		PtpOc.DefaultDsPriority1 = C.GoString(out)

	case PtpOc.DefaultDsPriority2:
		C.readPtpOcDefaultDsPriority2(out, size)
		PtpOc.DefaultDsPriority2 = C.GoString(out)

	case PtpOc.DefaultDsVariance:
		C.readPtpOcDefaultDsVariance(out, size)
		PtpOc.DefaultDsVariance = C.GoString(out)

	case PtpOc.DefaultDsAccuracy:
		C.readPtpOcDefaultDsAccuracy(out, size)
		PtpOc.DefaultDsAccuracy = C.GoString(out)

	case PtpOc.DefaultDsClass:
		C.readPtpOcDefaultDsClass(out, size)
		PtpOc.DefaultDsClass = C.GoString(out)

	case PtpOc.DefaultDsShortId:
		C.readPtpOcDefaultDsShortId(out, size)
		PtpOc.DefaultDsShortId = C.GoString(out)

	case PtpOc.DefaultDsInaccuracy:
		C.readPtpOcDefaultDsInaccuracy(out, size)
		PtpOc.DefaultDsInaccuracy = C.GoString(out)

	case PtpOc.DefaultDsNumberOfPorts:
		C.readPtpOcDefaultDsNumberOfPorts(out, size)
		PtpOc.DefaultDsNumberOfPorts = C.GoString(out)

		//
		//
	case PtpOc.PortDsPeerDelayValue:
		C.readPtpOcPortDsPeerDelayValue(out, size)
		PtpOc.PortDsPeerDelayValue = C.GoString(out)

	case PtpOc.PortDsState:
		C.readPtpOcPortDsState(out, size)
		PtpOc.PortDsState = C.GoString(out)

	case PtpOc.PortDsPDelayReqLogMsgIntervalValue:
		C.readPtpOcPortDsPDelayReqLogMsgIntervalValue(out, size)
		PtpOc.PortDsPDelayReqLogMsgIntervalValue = C.GoString(out)

	case PtpOc.PortDsDelayReqLogMsgIntervalValue:
		C.readPtpOcPortDsDelayReqLogMsgIntervalValue(out, size)
		PtpOc.PortDsDelayReqLogMsgIntervalValue = C.GoString(out)

	case PtpOc.PortDsDelayReceiptTimeoutValue:
		C.readPtpOcPortDsDelayReceiptTimeoutValue(out, size)
		PtpOc.PortDsDelayReceiptTimeoutValue = C.GoString(out)

	case PtpOc.PortDsAsymmetryValue:
		C.readPtpOcPortDsAsymmetryValue(out, size)
		PtpOc.PortDsAsymmetryValue = C.GoString(out)

	case PtpOc.PortDsMaxPeerDelayValue:
		C.readPtpOcPortDsMaxPeerDelayValue(out, size)
		PtpOc.PortDsMaxPeerDelayValue = C.GoString(out)

		//
	case PtpOc.CurrentDsStepsRemovedValue:
		C.readPtpOcCurrentDsStepsRemovedValue(out, size)
		PtpOc.CurrentDsStepsRemovedValue = C.GoString(out)

	case PtpOc.CurrentDsOffsetValue:
		C.readPtpOcCurrentDsOffsetValue(out, size)
		PtpOc.CurrentDsOffsetValue = C.GoString(out)

	case PtpOc.CurrentDsDelayValue:
		C.readPtpOcCurrentDsDelayValue(out, size)
		PtpOc.CurrentDsDelayValue = C.GoString(out)

	// parent dataset
	case PtpOc.ParentDsParentClockIdValue:
		C.readPtpOcParentDsParentClockIdValue(out, size)
		PtpOc.ParentDsParentClockIdValue = C.GoString(out)

	case PtpOc.ParentDsGmClockIdValue:
		C.readPtpOcParentDsGmClockIdValue(out, size)
		PtpOc.ParentDsGmClockIdValue = C.GoString(out)

	case PtpOc.ParentDsGmPriority1Value:
		C.readPtpOcParentDsGmPriority1Value(out, size)
		PtpOc.ParentDsGmPriority1Value = C.GoString(out)

	case PtpOc.ParentDsGmPriority2Value:
		C.readPtpOcParentDsGmPriority2Value(out, size)
		PtpOc.ParentDsGmPriority2Value = C.GoString(out)

	case PtpOc.ParentDsGmVarianceValue:
		C.readPtpOcParentDsGmVarianceValue(out, size)
		PtpOc.ParentDsGmVarianceValue = C.GoString(out)

	case PtpOc.ParentDsGmAccuracyValue:
		C.readPtpOcParentDsGmAccuracyValue(out, size)
		PtpOc.ParentDsGmAccuracyValue = C.GoString(out)

	case PtpOc.ParentDsGmClassValue:
		C.readPtpOcParentDsGmClassValue(out, size)
		PtpOc.ParentDsGmClassValue = C.GoString(out)

	case PtpOc.ParentDsGmShortIdValue:
		C.readPtpOcParentDsGmShortIdValue(out, size)
		PtpOc.ParentDsGmShortIdValue = C.GoString(out)

	case PtpOc.ParentDsGmInaccuracyValue:
		C.readPtpOcParentDsGmInaccuracyValue(out, size)
		PtpOc.ParentDsGmInaccuracyValue = C.GoString(out)

	case PtpOc.ParentDsNwInaccuracyValue:
		C.readPtpOcParentDsNwInaccuracyValue(out, size)
		PtpOc.ParentDsNwInaccuracyValue = C.GoString(out)

		// time properties
	case PtpOc.TimePropertiesDsTimeSourceValue:
		C.readPtpOcTimePropertiesDsTimeSourceValue(out, size)
		PtpOc.TimePropertiesDsTimeSourceValue = C.GoString(out)

	case PtpOc.TimePropertiesDsPtpTimescaleStatus:
		C.readPtpOcTimePropertiesDsPtpTimescaleStatus(out, size)
		PtpOc.TimePropertiesDsPtpTimescaleStatus = C.GoString(out)
	case PtpOc.TimePropertiesDsFreqTraceableStatus:
		C.readPtpOcTimePropertiesDsFreqTraceableStatus(out, size)
		PtpOc.TimePropertiesDsFreqTraceableStatus = C.GoString(out)
	case PtpOc.TimePropertiesDsTimeTraceableStatus:
		C.readPtpOcTimePropertiesDsTimeTraceableStatus(out, size)
		PtpOc.TimePropertiesDsTimeTraceableStatus = C.GoString(out)
	case PtpOc.TimePropertiesDsLeap61Status:
		C.readPtpOcTimePropertiesDsLeap61Status(out, size)
		PtpOc.TimePropertiesDsLeap61Status = C.GoString(out)
	case PtpOc.TimePropertiesDsLeap59Status:
		C.readPtpOcTimePropertiesDsLeap59Status(out, size)
		PtpOc.TimePropertiesDsLeap59Status = C.GoString(out)
	case PtpOc.TimePropertiesDsUtcOffsetValStatus:
		C.readPtpOcTimePropertiesDsUtcOffsetValStatus(out, size)
		PtpOc.TimePropertiesDsUtcOffsetValStatus = C.GoString(out)
	case PtpOc.TimePropertiesDsUtcOffsetValue:
		C.readPtpOcTimePropertiesDsUtcOffsetValue(out, size)
		PtpOc.TimePropertiesDsUtcOffsetValue = C.GoString(out)
	case PtpOc.TimePropertiesDsCurrentOffsetValue:
		C.readPtpOcTimePropertiesDsCurrentOffsetValue(out, size)
		PtpOc.TimePropertiesDsCurrentOffsetValue = C.GoString(out)
	case PtpOc.TimePropertiesDsJumpSecondsValue:
		C.readPtpOcTimePropertiesDsJumpSecondsValue(out, size)
		PtpOc.TimePropertiesDsJumpSecondsValue = C.GoString(out)
	case PtpOc.TimePropertiesDsNextJumpValue:
		C.readPtpOcTimePropertiesDsNextJumpValue(out, size)
		PtpOc.TimePropertiesDsNextJumpValue = C.GoString(out)
	case PtpOc.TimePropertiesDsDisplayNameValue:
		C.readPtpOcTimePropertiesDsDisplayNameValue(out, size)
		PtpOc.TimePropertiesDsDisplayNameValue = C.GoString(out)

	default:
		fmt.Println("no such property")
	}

	mutex.Unlock()
	defer C.free(unsafe.Pointer(out))

	updatedData, err := json.MarshalIndent(PtpOc, "", "  ")
	if err != nil {
		log.Fatalf("Error marshaling: %v", err)
	}

	err = os.WriteFile("/home/jowens/Projects/NovusTimeServer/comms/ptpOc.json", updatedData, 0644)
	if err != nil {
		log.Fatalf("Error writing file: %v", err)
	}

	return C.GoString(out)
}

func WritePtpOc(property string, value string) {
	start := time.Now()

	in := C.CString(value)

	mutex.Lock()

	switch property {

	case PtpOc.Profile:
		err := C.writePtpOcProfile(in, size)
		if err != 0 {
			fmt.Println("writePtpOcProfile ERROR: ", err)
		}

	case PtpOc.DefaultDsTwoStepStatus:
		err := C.writePtpOcDefaultDsTwoStepStatus(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsTwoStepStatus ERROR: ", err)
		}

	case PtpOc.DefaultDsSignalingStatus:
		err := C.writePtpOcDefaultDsSignalingStatus(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsSignalingStatus ERROR: ", err)
		}

	case PtpOc.DefaultDsSlaveOnlyStatus:
		err := C.writePtpOcDefaultDsSlaveOnlyStatus(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsSlaveOnlyStatus ERROR: ", err)
		}

	case PtpOc.DefaultDsMasterOnlyStatus:
		err := C.writePtpOcDefaultDsMasterOnlyStatus(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsMasterOnlyStatus ERROR: ", err)
		}

	case PtpOc.DefaultDsDisableOffsetCorrectionStatus:
		err := C.writePtpOcDefaultDsDisableOffsetCorrectionStatus(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsDisableOffsetCorStatus ERROR: ", err)
		}

	case PtpOc.DefaultDsListedUnicastSlavesOnlyStatus:
		err := C.writePtpOcDefaultDsListedUnicastSlavesOnlyStatus(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsListedUnicastSlavesOnlyStatus ERROR: ", err)
		}

	case PtpOc.Layer:
		err := C.writePtpOcLayer(in, size)
		if err != 0 {
			fmt.Println("writePtpOcLayer ERROR: ", err)
		}

	case PtpOc.DelayMechanismValue:
		err := C.writePtpOcDelayMechanismValue(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDelayMechanismValue ERROR: ", err)
		}

	case PtpOc.VlanAddress:
		err := C.writePtpOcVlanAddress(in, size)
		if err != 0 {
			fmt.Println("writePtpOcVlanAddress ERROR: ", err)
		}

	case PtpOc.VlanStatus:
		err := C.writePtpOcVlanStatus(in, size)
		if err != 0 {
			fmt.Println("writePtpOcVlanStatus ERROR: ", err)
		}

	case PtpOc.DefaultDsClockId:
		err := C.writePtpOcDefaultDsClockIdValue(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsClockIdValue ERROR: ", err)
		}

	case PtpOc.DefaultDsDomain:
		err := C.writePtpOcDefaultDsDomainValue(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsDomainValue ERROR: ", err)
		}

	case PtpOc.DefaultDsPriority1:
		err := C.writePtpOcDefaultDsPriority1Value(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsPriority1Value ERROR: ", err)
		}

	case PtpOc.DefaultDsPriority2:
		err := C.writePtpOcDefaultDsPriority2Value(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsPriority2Value ERROR: ", err)
		}

	case PtpOc.DefaultDsClass:
		err := C.writePtpOcDefaultDsClassValue(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsClassValue ERROR: ", err)
		}

	case PtpOc.DefaultDsAccuracy:
		err := C.writePtpOcDefaultDsAccuracyValue(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsAccuracyValue ERROR: ", err)
		}

	case PtpOc.DefaultDsVariance:
		err := C.writePtpOcDefaultDsVarianceValue(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsVarianceValue ERROR: ", err)
		}

	case PtpOc.DefaultDsShortId:
		err := C.writePtpOcDefaultDsShortIdValue(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsShortIdValue ERROR: ", err)
		}

	case PtpOc.DefaultDsInaccuracy:
		err := C.writePtpOcDefaultDsInaccuracyValue(in, size)
		if err != 0 {
			fmt.Println("writePtpOcDefaultDsInaccuracyValue ERROR: ", err)
		}

		//
		//
	default:
		fmt.Println("NO SUCH WRITE PROPERTY")
	}
	mutex.Unlock()
	defer C.free(unsafe.Pointer(in))
	fmt.Println(property, "w : ", time.Since(start))

}

func ListPtpOcProperties() {

	properties := []string{
		//PtpOc.Version,
		PtpOc.Status,
		PtpOc.VlanStatus,
		PtpOc.VlanAddress,
		PtpOc.Profile,
		PtpOc.DefaultDsTwoStepStatus,
		PtpOc.DefaultDsSignalingStatus,
		PtpOc.Layer,
		PtpOc.DefaultDsSlaveOnlyStatus,
		PtpOc.DefaultDsMasterOnlyStatus,
		PtpOc.DefaultDsDisableOffsetCorrectionStatus,
		PtpOc.DefaultDsListedUnicastSlavesOnlyStatus,
		PtpOc.DelayMechanismValue,
		PtpOc.IpAddress,
		PtpOc.DefaultDsClockId,
		PtpOc.DefaultDsDomain,
		PtpOc.DefaultDsPriority1,
		PtpOc.DefaultDsPriority2,
		PtpOc.DefaultDsVariance,
		PtpOc.DefaultDsAccuracy,
		PtpOc.DefaultDsClass,
		PtpOc.DefaultDsShortId,
		PtpOc.DefaultDsInaccuracy,
		PtpOc.DefaultDsNumberOfPorts,
		PtpOc.PortDsPeerDelayValue,
		PtpOc.PortDsState,
		PtpOc.PortDsPDelayReqLogMsgIntervalValue,
		PtpOc.PortDsDelayReqLogMsgIntervalValue,
		PtpOc.PortDsDelayReceiptTimeoutValue,
		PtpOc.PortDsAsymmetryValue,
		PtpOc.PortDsMaxPeerDelayValue,
		PtpOc.CurrentDsStepsRemovedValue,
		PtpOc.CurrentDsOffsetValue,
		PtpOc.CurrentDsDelayValue,
		PtpOc.ParentDsParentClockIdValue,
		PtpOc.ParentDsGmClockIdValue,
		PtpOc.ParentDsGmPriority1Value,
		PtpOc.ParentDsGmPriority2Value,
		PtpOc.ParentDsGmVarianceValue,
		PtpOc.ParentDsGmAccuracyValue,
		PtpOc.ParentDsGmClassValue,
		PtpOc.ParentDsGmShortIdValue,
		PtpOc.ParentDsGmInaccuracyValue,
		PtpOc.ParentDsNwInaccuracyValue,
		PtpOc.TimePropertiesDsTimeSourceValue,
		PtpOc.TimePropertiesDsPtpTimescaleStatus,
		PtpOc.TimePropertiesDsFreqTraceableStatus,
		PtpOc.TimePropertiesDsTimeTraceableStatus,
		PtpOc.TimePropertiesDsLeap61Status,
		PtpOc.TimePropertiesDsLeap59Status,
		PtpOc.TimePropertiesDsUtcOffsetValStatus,
		PtpOc.TimePropertiesDsUtcOffsetValue,
		PtpOc.TimePropertiesDsCurrentOffsetValue,
		PtpOc.TimePropertiesDsJumpSecondsValue,
		PtpOc.TimePropertiesDsNextJumpValue,
		PtpOc.TimePropertiesDsDisplayNameValue}

	for _, p := range properties {
		fmt.Println(p, " : ", ReadPtpOc(p))
	}
}
