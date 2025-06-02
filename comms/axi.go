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
	"fmt"
	"sync"
	"time"
	"unsafe"
)

type NtpServerApi struct {
	Status               string
	InstanceNumber       string
	IpMode               string
	IpAddress            string
	MacAddress           string
	VlanStatus           string
	VlanAddress          string
	UnicastMode          string
	MulticastMode        string
	BroadcastMode        string
	PrecisionValue       string
	PollIntervalValue    string
	StratumValue         string
	ReferenceId          string
	SmearingStatus       string
	Leap61Progress       string
	Leap59Progress       string
	Leap61Status         string
	Leap59Status         string
	UtcOffsetStatus      string
	UtcOffsetValue       string
	RequestsValue        string
	ResponsesValue       string
	RequestsDroppedValue string
	BroadcastsValue      string
	ClearCountersStatus  string
	Version              string
	Root                 string
}

var NtpServer = NtpServerApi{
	Status:               "status",
	InstanceNumber:       "instance",
	IpMode:               "ip-mode",
	IpAddress:            "ip-address",
	MacAddress:           "mac-address",
	VlanStatus:           "vlan-status",
	VlanAddress:          "vlan-address",
	UnicastMode:          "unicast",
	MulticastMode:        "multicast",
	BroadcastMode:        "broadcast",
	PrecisionValue:       "precision",
	PollIntervalValue:    "poll-interval",
	StratumValue:         "stratum",
	ReferenceId:          "reference-id",
	SmearingStatus:       "smearing-status",
	Leap61Progress:       "leap61-progress",
	Leap59Progress:       "leap59-progress",
	Leap61Status:         "leap61-status",
	Leap59Status:         "leap59-status",
	UtcOffsetStatus:      "utc-offset-status",
	UtcOffsetValue:       "utc-offset",
	RequestsValue:        "requests",
	ResponsesValue:       "responses",
	RequestsDroppedValue: "requestsdropped",
	BroadcastsValue:      "broadcasts",
	ClearCountersStatus:  "clearcounters",
	Version:              "version",
	Root:                 "ntp-server",
}

type PtpOcApi struct {
	Version                                string
	Root                                   string
	Status                                 string
	VlanStatus                             string
	VlanAddress                            string
	Profile                                string
	DefaultDsTwoStepStatus                 string
	DefaultDsSignalingStatus               string
	Layer                                  string
	DefaultDsSlaveOnlyStatus               string
	DefaultDsMasterOnlyStatus              string
	DefaultDsDisableOffsetCorrectionStatus string
	DefaultDsListedUnicastSlavesOnlyStatus string
	DelayMechanismValue                    string
	IpAddress                              string
	DefaultDsClockId                       string
	DefaultDsDomain                        string
	DefaultDsPriority1                     string
	DefaultDsPriority2                     string
	DefaultDsVariance                      string
	DefaultDsAccuracy                      string
	DefaultDsClass                         string
	DefaultDsShortId                       string
	DefaultDsInaccuracy                    string
	DefaultDsNumberOfPorts                 string
	PortDsPeerDelayValue                   string
	PortDsState                            string
	PortDsPDelayReqLogMsgIntervalValue     string
	PortDsDelayReqLogMsgIntervalValue      string
	PortDsDelayReceiptTimeoutValue         string
	PortDsAsymmetryValue                   string
	PortDsMaxPeerDelayValue                string
	CurrentDsStepsRemovedValue             string
	CurrentDsOffsetValue                   string
	CurrentDsDelayValue                    string
	ParentDsParentClockIdValue             string
	ParentDsGmClockIdValue                 string
	ParentDsGmPriority1Value               string
	ParentDsGmPriority2Value               string
	ParentDsGmVarianceValue                string
	ParentDsGmAccuracyValue                string
	ParentDsGmClassValue                   string
	ParentDsGmShortIdValue                 string
	ParentDsGmInaccuracyValue              string
	ParentDsNwInaccuracyValue              string
	TimePropertiesDsTimeSourceValue        string
	TimePropertiesDsPtpTimescaleStatus     string
	TimePropertiesDsFreqTraceableStatus    string
	TimePropertiesDsTimeTraceableStatus    string
	TimePropertiesDsLeap61Status           string
	TimePropertiesDsLeap59Status           string
	TimePropertiesDsUtcOffsetValStatus     string
	TimePropertiesDsUtcOffsetValue         string
	TimePropertiesDsCurrentOffsetValue     string
	TimePropertiesDsJumpSecondsValue       string
	TimePropertiesDsNextJumpValue          string
	TimePropertiesDsDisplayNameValue       string
}

var PtpOc = PtpOcApi{
	Version:                                "version",
	Root:                                   "ptp-oc",
	Status:                                 "status",
	VlanStatus:                             "vlan-status",
	VlanAddress:                            "vlan-address",
	Profile:                                "profile",
	DefaultDsTwoStepStatus:                 "default-ds-two-step-status",
	DefaultDsSignalingStatus:               "default-ds-signaling-status",
	Layer:                                  "layer",
	DefaultDsSlaveOnlyStatus:               "default-ds-slave-only-status",
	DefaultDsMasterOnlyStatus:              "default-ds-master-only-status",
	DefaultDsDisableOffsetCorrectionStatus: "default-ds-disable-offset-correction-status",
	DefaultDsListedUnicastSlavesOnlyStatus: "default-ds-listed-unicast-slaves-only-status",
	DelayMechanismValue:                    "delay-mechanism-value",
	IpAddress:                              "ip-address",
	DefaultDsClockId:                       "default-ds-clock-id",
	DefaultDsDomain:                        "default-ds-domain",
	DefaultDsPriority1:                     "default-ds-priority1",
	DefaultDsPriority2:                     "default-ds-priority2",
	DefaultDsVariance:                      "default-ds-variance",
	DefaultDsAccuracy:                      "default-ds-accuracy",
	DefaultDsClass:                         "default-ds-class",
	DefaultDsShortId:                       "default-ds-shortid",
	DefaultDsInaccuracy:                    "default-ds-inaccuracy",
	DefaultDsNumberOfPorts:                 "default-ds-numberofports",
	PortDsPeerDelayValue:                   "port-ds-peer-delay-value",
	PortDsState:                            "port-ds-state",
	PortDsPDelayReqLogMsgIntervalValue:     "port-ds-p-delay-req-log-msg-interval-value",
	PortDsDelayReqLogMsgIntervalValue:      "port-ds-delay-req-log-msg-interval-value",
	PortDsDelayReceiptTimeoutValue:         "port-ds-delay-receipt-timeout-value",
	PortDsAsymmetryValue:                   "port-ds-asymmetry-value",
	PortDsMaxPeerDelayValue:                "port-ds-max-peer-delay-value",
	CurrentDsStepsRemovedValue:             "current-ds-steps-removed-value",
	CurrentDsOffsetValue:                   "current-ds-offset-value",
	CurrentDsDelayValue:                    "current-ds-delay-value",
	ParentDsParentClockIdValue:             "parent-ds-parent-clock-id-value",
	ParentDsGmClockIdValue:                 "parent-ds-gm-clock-id-value",
	ParentDsGmPriority1Value:               "parent-ds-gm-priority-1-value",
	ParentDsGmPriority2Value:               "parent-ds-gm-priority-2-value",
	ParentDsGmVarianceValue:                "parent-ds-gm-variance-value",
	ParentDsGmAccuracyValue:                "parent-ds-gm-accuracy-value",
	ParentDsGmClassValue:                   "parent-ds-gm-class-value",
	ParentDsGmShortIdValue:                 "parent-ds-gm-short-id-value",
	ParentDsGmInaccuracyValue:              "parent-ds-gm-inaccuracy-value",
	ParentDsNwInaccuracyValue:              "parent-ds-nw-inaccuracy-value",
	TimePropertiesDsTimeSourceValue:        "time-properties-ds-time-source-value",
	TimePropertiesDsPtpTimescaleStatus:     "time-properties-ds-ptp-time-scale-status",
	TimePropertiesDsFreqTraceableStatus:    "time-properties-ds-freq-traceable-status",
	TimePropertiesDsTimeTraceableStatus:    "time-properties-ds-time-traceable-status",
	TimePropertiesDsLeap61Status:           "time-properties-ds-leap61-status",
	TimePropertiesDsLeap59Status:           "time-properties-ds-leap59-status",
	TimePropertiesDsUtcOffsetValStatus:     "time-properties-ds-ut-coffset-val-status",
	TimePropertiesDsUtcOffsetValue:         "time-properties-ds-utc-offset-value",
	TimePropertiesDsCurrentOffsetValue:     "time-properties-ds-current-offset-value",
	TimePropertiesDsJumpSecondsValue:       "time-properties-ds-jump-seconds-value",
	TimePropertiesDsNextJumpValue:          "time-properties-ds-next-jump-value",
	TimePropertiesDsDisplayNameValue:       "time-properties-ds-display-name-value",
}

var mutex sync.Mutex

const size = C.size_t(64)

func init() {
	mutex.Lock()
	C.connect()
	C.readConfig()
	mutex.Unlock()
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
	case PtpOc.Version:
		//C.readPtpOcVersion(out, size)
	case PtpOc.Root:
		//C.readPtpOcRoot(out, size)
	case PtpOc.Status:
		C.readPtpOcStatus(out, size)
	case PtpOc.VlanStatus:
		C.readPtpOcVlanStatus(out, size)
	case PtpOc.VlanAddress:
		C.readPtpOcVlanAddress(out, size)
	case PtpOc.Profile:
		C.readPtpOcProfile(out, size)
	case PtpOc.DefaultDsTwoStepStatus:
		C.readPtpOcDefaultDsTwoStepStatus(out, size)
	case PtpOc.DefaultDsSignalingStatus:
		C.readPtpOcDefaultDsSignalingStatus(out, size)
	case PtpOc.Layer:
		C.readPtpOcLayer(out, size)
	case PtpOc.DefaultDsSlaveOnlyStatus:
		C.readPtpOcDefaultDsSlaveOnlyStatus(out, size)
	case PtpOc.DefaultDsMasterOnlyStatus:
		C.readPtpOcDefaultDsMasterOnlyStatus(out, size)
	case PtpOc.DefaultDsDisableOffsetCorrectionStatus:
		C.readPtpOcDefaultDsDisableOffsetCorrectionStatus(out, size)
	case PtpOc.DefaultDsListedUnicastSlavesOnlyStatus:
		C.readPtpOcDefaultDsListedUnicastSlavesOnlyStatus(out, size)
	case PtpOc.DelayMechanismValue:
		C.readPtpOcDelayMechanismValue(out, size)
	case PtpOc.IpAddress:
		C.readPtpOcIpAddress(out, size)
	case PtpOc.DefaultDsClockId:
		C.readPtpOcDefaultDsClockId(out, size)
	case PtpOc.DefaultDsDomain:
		C.readPtpOcDefaultDsDomain(out, size)
	case PtpOc.DefaultDsPriority1:
		C.readPtpOcDefaultDsPriority1(out, size)
	case PtpOc.DefaultDsPriority2:
		C.readPtpOcDefaultDsPriority2(out, size)
	case PtpOc.DefaultDsVariance:
		C.readPtpOcDefaultDsVariance(out, size)
	case PtpOc.DefaultDsAccuracy:
		C.readPtpOcDefaultDsAccuracy(out, size)
	case PtpOc.DefaultDsClass:
		C.readPtpOcDefaultDsClass(out, size)
	case PtpOc.DefaultDsShortId:
		C.readPtpOcDefaultDsShortId(out, size)
	case PtpOc.DefaultDsInaccuracy:
		C.readPtpOcDefaultDsInaccuracy(out, size)
	case PtpOc.DefaultDsNumberOfPorts:
		C.readPtpOcDefaultDsNumberOfPorts(out, size)
		//
		//
	case PtpOc.PortDsPeerDelayValue:
		C.readPtpOcPortDsPeerDelayValue(out, size)
	case PtpOc.PortDsState:
		C.readPtpOcPortDsState(out, size)
	case PtpOc.PortDsPDelayReqLogMsgIntervalValue:
		C.readPtpOcPortDsPDelayReqLogMsgIntervalValue(out, size)
	case PtpOc.PortDsDelayReqLogMsgIntervalValue:
		C.readPtpOcPortDsDelayReqLogMsgIntervalValue(out, size)
	case PtpOc.PortDsDelayReceiptTimeoutValue:
		C.readPtpOcPortDsDelayReceiptTimeoutValue(out, size)
	case PtpOc.PortDsAsymmetryValue:
		C.readPtpOcPortDsAsymmetryValue(out, size)
	case PtpOc.PortDsMaxPeerDelayValue:
		C.readPtpOcPortDsMaxPeerDelayValue(out, size)

		//
	case PtpOc.CurrentDsStepsRemovedValue:
		C.readPtpOcCurrentDsStepsRemovedValue(out, size)
	case PtpOc.CurrentDsOffsetValue:
		C.readPtpOcCurrentDsOffsetValue(out, size)

	case PtpOc.CurrentDsDelayValue:
		C.readPtpOcCurrentDsDelayValue(out, size)

	// parent dataset
	case PtpOc.ParentDsParentClockIdValue:
		C.readPtpOcParentDsParentClockIdValue(out, size)

	case PtpOc.ParentDsGmClockIdValue:
		C.readPtpOcParentDsGmClockIdValue(out, size)
	case PtpOc.ParentDsGmPriority1Value:
		C.readPtpOcParentDsGmPriority1Value(out, size)

	case PtpOc.ParentDsGmPriority2Value:
		C.readPtpOcParentDsGmPriority2Value(out, size)
	case PtpOc.ParentDsGmVarianceValue:
		C.readPtpOcParentDsGmVarianceValue(out, size)
	case PtpOc.ParentDsGmAccuracyValue:
		C.readPtpOcParentDsGmAccuracyValue(out, size)
	case PtpOc.ParentDsGmClassValue:
		C.readPtpOcParentDsGmClassValue(out, size)
	case PtpOc.ParentDsGmShortIdValue:
		C.readPtpOcParentDsGmShortIdValue(out, size)
	case PtpOc.ParentDsGmInaccuracyValue:
		C.readPtpOcParentDsGmInaccuracyValue(out, size)
	case PtpOc.ParentDsNwInaccuracyValue:
		C.readPtpOcParentDsNwInaccuracyValue(out, size)

		// time properties
	case PtpOc.TimePropertiesDsTimeSourceValue:
		C.readPtpOcTimePropertiesDsTimeSourceValue(out, size)

	case PtpOc.TimePropertiesDsPtpTimescaleStatus:
		C.readPtpOcTimePropertiesDsPtpTimescaleStatus(out, size)
	case PtpOc.TimePropertiesDsFreqTraceableStatus:
		C.readPtpOcTimePropertiesDsFreqTraceableStatus(out, size)
	case PtpOc.TimePropertiesDsTimeTraceableStatus:
		C.readPtpOcTimePropertiesDsTimeTraceableStatus(out, size)
	case PtpOc.TimePropertiesDsLeap61Status:
		C.readPtpOcTimePropertiesDsLeap61Status(out, size)
	case PtpOc.TimePropertiesDsLeap59Status:
		C.readPtpOcTimePropertiesDsLeap59Status(out, size)
	case PtpOc.TimePropertiesDsUtcOffsetValStatus:
		C.readPtpOcTimePropertiesDsUtcOffsetValStatus(out, size)
	case PtpOc.TimePropertiesDsUtcOffsetValue:
		C.readPtpOcTimePropertiesDsUtcOffsetValue(out, size)
	case PtpOc.TimePropertiesDsCurrentOffsetValue:
		C.readPtpOcTimePropertiesDsCurrentOffsetValue(out, size)
	case PtpOc.TimePropertiesDsJumpSecondsValue:
		C.readPtpOcTimePropertiesDsJumpSecondsValue(out, size)
	case PtpOc.TimePropertiesDsNextJumpValue:
		C.readPtpOcTimePropertiesDsNextJumpValue(out, size)
	case PtpOc.TimePropertiesDsDisplayNameValue:
		C.readPtpOcTimePropertiesDsDisplayNameValue(out, size)

	default:
		fmt.Println("no such property")
	}
	mutex.Unlock()
	defer C.free(unsafe.Pointer(out))
	return C.GoString(out)
}

func WritePtpOc(property string, value string) {
	in := C.CString(value)
	mutex.Lock()
	C.connect()
	C.readConfig()
	switch property {

	case PtpOc.Version:
		fmt.Println("this is not accessable")
		//err := C.writeNtpServerMacAddress(in, size)
	//	if err != 0 {
	//		fmt.Println("writeNtpServerMacAddress ERROR: ", err)
	//	}

	default:
		fmt.Println("no such property")
	}
	mutex.Unlock()
	defer C.free(unsafe.Pointer(in))
}

func ListPtpOcProperties() {
	properties := []string{
		//PtpOc.Version,
		//PtpOc.Root,
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
		//fmt.Println(i)
		fmt.Println(p, " : ", ReadPtpOc(p))
	}

}
