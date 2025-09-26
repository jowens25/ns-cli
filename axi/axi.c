#include <stdint.h>
#include <stddef.h>
#include "axi.h"
#include "clkClock.h"
#include "ntpServer.h"
#include "ppsSlave.h"
#include "ptpOc.h"
#include "todSlave.h"
#include "cores.h"

int64_t temp_data = 0x00000000;
int64_t temp_addr = 0x00000000;
// const char *FPGA_PORT = "FPGA_PORT";

#define MAX_NUM_PROP 256
#define MAX_NUM_MODS 64
#define MAX_NUM_OPS 2 // 0 read // 1 write
#define Read 0
#define Write 1

read_write_func timeServer[MAX_NUM_OPS][MAX_NUM_MODS][MAX_NUM_PROP] =
    {
        [Read][ClkClock][ClkClockVersion] = readClkClockVersion,
        [Read][ClkClock][ClkClockInstance] = readClkClockInstance,
        [Read][ClkClock][ClkClockStatus] = readClkClockStatus,
        [Read][ClkClock][ClkClockSeconds] = readClkClockSeconds,
        [Read][ClkClock][ClkClockNanoseconds] = readClkClockNanoseconds,
        //[Read][ClkClock][ClkClockTimeAdj] = readClkClockTimeAdj,
        [Read][ClkClock][ClkClockInSync] = readClkClockInSync,
        [Read][ClkClock][ClkClockInHoldover] = readClkClockInHoldover,
        [Read][ClkClock][ClkClockInSyncThreshold] = readClkClockInSyncThreshold,
        [Read][ClkClock][ClkClockSource] = readClkClockSource,
        [Read][ClkClock][ClkClockDrift] = readClkClockDrift,
        [Read][ClkClock][ClkClockDriftInterval] = readClkClockDriftInterval,
        [Read][ClkClock][ClkClockDriftAdj] = readClkClockDriftAdj,
        [Read][ClkClock][ClkClockOffset] = readClkClockOffset,
        [Read][ClkClock][ClkClockOffsetInterval] = readClkClockOffsetInterval,
        [Read][ClkClock][ClkClockOffsetAdj] = readClkClockOffsetAdj,
        //[Read][ClkClock][ClkClockPiOffsetMulP] = readClkClockPiOffsetMulP,
        //[Read][ClkClock][ClkClockPiOffsetDivP] = readClkClockPiOffsetDivP,
        //[Read][ClkClock][ClkClockPiOffsetMulI] = readClkClockPiOffsetMulI,
        //[Read][ClkClock][ClkClockPiOffsetDivI] = readClkClockPiOffsetDivI,
        //[Read][ClkClock][ClkClockPiDriftMulP] = readClkClockPiDriftMulP,
        //[Read][ClkClock][ClkClockPiDriftDivP] = readClkClockPiDriftDivP,
        //[Read][ClkClock][ClkClockPiDriftMulI] = readClkClockPiDriftMulI,
        //[Read][ClkClock][ClkClockPiDriftDivI] = readClkClockPiDriftDivI,
        [Read][ClkClock][ClkClockPiSetCustomParameters] = readClkClockPiSetCustomParameters,
        [Read][ClkClock][ClkClockCorrectedOffset] = readClkClockCorrectedOffset,
        [Read][ClkClock][ClkClockCorrectedDrift] = readClkClockCorrectedDrift,
        [Read][ClkClock][ClkClockDate] = readClkClockDate,

        //[Write][ClkClock][ClkClockVersion] = readClkClockVersion,
        //[Write][ClkClock][ClkClockInstance] = readClkClockInstance,
        //[Write][ClkClock][ClkClockStatus] = readClkClockStatus,
        [Write][ClkClock][ClkClockSeconds] = writeClkClockSeconds,
        [Write][ClkClock][ClkClockNanoseconds] = writeClkClockNanoseconds,
        //[Write][ClkClock][ClkClockTimeAdj] = readClkClockTimeAdj,
        //[Write][ClkClock][ClkClockInSync] = readClkClockInSync,
        //[Write][ClkClock][ClkClockInHoldover] = writeClkClockInHoldover,
        [Write][ClkClock][ClkClockInSyncThreshold] = writeClkClockInSyncThreshold,
        //[Write][ClkClock][ClkClockSource] = readClkClockSource,
        [Write][ClkClock][ClkClockDrift] = writeClkClockDrift,
        [Write][ClkClock][ClkClockDriftInterval] = writeClkClockDriftInterval,
        //[Write][ClkClock][ClkClockDriftAdj] = readClkClockDriftAdj,
        [Write][ClkClock][ClkClockOffset] = writeClkClockOffset,
        [Write][ClkClock][ClkClockOffsetInterval] = writeClkClockOffsetInterval,
        //[Write][ClkClock][ClkClockOffsetAdj] = writeClkClockOffsetAdj,
        //[Write][ClkClock][ClkClockPiOffsetMulP] = readClkClockPiOffsetMulP,
        //[Write][ClkClock][ClkClockPiOffsetDivP] = readClkClockPiOffsetDivP,
        //[Write][ClkClock][ClkClockPiOffsetMulI] = readClkClockPiOffsetMulI,
        //[Write][ClkClock][ClkClockPiOffsetDivI] = readClkClockPiOffsetDivI,
        //[Write][ClkClock][ClkClockPiDriftMulP] = readClkClockPiDriftMulP,
        //[Write][ClkClock][ClkClockPiDriftDivP] = readClkClockPiDriftDivP,
        //[Write][ClkClock][ClkClockPiDriftMulI] = readClkClockPiDriftMulI,
        //[Write][ClkClock][ClkClockPiDriftDivI] = readClkClockPiDriftDivI,
        //[Write][ClkClock][ClkClockPiSetCustomParameters] = readClkClockPiSetCustomParameters,
        //[Write][ClkClock][ClkClockCorrectedOffset] = readClkClockCorrectedOffset,
        //[Write][ClkClock][ClkClockCorrectedDrift] = readClkClockCorrectedDrift,
        //[Write][ClkClock][ClkClockDate] = readClkClockDate,

        [Read][PtpOrdinaryClock][PtpOcVersion] = readPtpOcVersion,
        [Read][PtpOrdinaryClock][PtpOcInstanceNumber] = readPtpOcInstanceNumber,
        [Read][PtpOrdinaryClock][PtpOcVlanAddress] = readPtpOcVlanAddress,
        [Read][PtpOrdinaryClock][PtpOcVlanStatus] = readPtpOcVlanStatus,
        [Read][PtpOrdinaryClock][PtpOcProfile] = readPtpOcProfile,
        [Read][PtpOrdinaryClock][PtpOcLayer] = readPtpOcLayer,
        [Read][PtpOrdinaryClock][PtpOcDelayMechanismValue] = readPtpOcDelayMechanismValue,
        [Read][PtpOrdinaryClock][PtpOcIpAddress] = readPtpOcIpAddress,
        [Read][PtpOrdinaryClock][PtpOcStatus] = readPtpOcStatus,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsClockId] = readPtpOcDefaultDsClockId,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsDomain] = readPtpOcDefaultDsDomain,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsPriority1] = readPtpOcDefaultDsPriority1,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsPriority2] = readPtpOcDefaultDsPriority2,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsAccuracy] = readPtpOcDefaultDsAccuracy,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsClass] = readPtpOcDefaultDsClass,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsVariance] = readPtpOcDefaultDsVariance,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsShortId] = readPtpOcDefaultDsShortId,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsInaccuracy] = readPtpOcDefaultDsInaccuracy,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsNumberOfPorts] = readPtpOcDefaultDsNumberOfPorts,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsTwoStepStatus] = readPtpOcDefaultDsTwoStepStatus,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsSignalingStatus] = readPtpOcDefaultDsSignalingStatus,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsMasterOnlyStatus] = readPtpOcDefaultDsMasterOnlyStatus,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsSlaveOnlyStatus] = readPtpOcDefaultDsSlaveOnlyStatus,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsListedUnicastSlavesOnlyStatus] = readPtpOcDefaultDsListedUnicastSlavesOnlyStatus,
        [Read][PtpOrdinaryClock][PtpOcDefaultDsDisableOffsetCorrectionStatus] = readPtpOcDefaultDsDisableOffsetCorrectionStatus,
        [Read][PtpOrdinaryClock][PtpOcPortDsPeerDelayValue] = readPtpOcPortDsPeerDelayValue,
        [Read][PtpOrdinaryClock][PtpOcPortDsState] = readPtpOcPortDsState,
        [Read][PtpOrdinaryClock][PtpOcPortDsAsymmetryValue] = readPtpOcPortDsAsymmetryValue,
        [Read][PtpOrdinaryClock][PtpOcPortDsMaxPeerDelayValue] = readPtpOcPortDsMaxPeerDelayValue,
        [Read][PtpOrdinaryClock][PtpOcPortDsPDelayReqLogMsgIntervalValue] = readPtpOcPortDsPDelayReqLogMsgIntervalValue,
        [Read][PtpOrdinaryClock][PtpOcPortDsDelayReqLogMsgIntervalValue] = readPtpOcPortDsDelayReqLogMsgIntervalValue,
        [Read][PtpOrdinaryClock][PtpOcPortDsDelayReceiptTimeoutValue] = readPtpOcPortDsDelayReceiptTimeoutValue,
        [Read][PtpOrdinaryClock][PtpOcPortDsAnnounceLogMsgIntervalValue] = readPtpOcPortDsAnnounceLogMsgIntervalValue,
        [Read][PtpOrdinaryClock][PtpOcPortDsAnnounceReceiptTimeoutValue] = readPtpOcPortDsAnnounceReceiptTimeoutValue,
        [Read][PtpOrdinaryClock][PtpOcPortDsSyncLogMsgIntervalValue] = readPtpOcPortDsSyncLogMsgIntervalValue,
        [Read][PtpOrdinaryClock][PtpOcPortDsSyncReceiptTimeoutValue] = readPtpOcPortDsSyncReceiptTimeoutValue,
        [Read][PtpOrdinaryClock][PtpOcCurrentDsStepsRemovedValue] = readPtpOcCurrentDsStepsRemovedValue,
        [Read][PtpOrdinaryClock][PtpOcCurrentDsOffsetValue] = readPtpOcCurrentDsOffsetValue,
        [Read][PtpOrdinaryClock][PtpOcCurrentDsDelayValue] = readPtpOcCurrentDsDelayValue,
        [Read][PtpOrdinaryClock][PtpOcParentDsParentClockIdValue] = readPtpOcParentDsParentClockIdValue,
        [Read][PtpOrdinaryClock][PtpOcParentDsGmClockIdValue] = readPtpOcParentDsGmClockIdValue,
        [Read][PtpOrdinaryClock][PtpOcParentDsGmPriority1Value] = readPtpOcParentDsGmPriority1Value,
        [Read][PtpOrdinaryClock][PtpOcParentDsGmPriority2Value] = readPtpOcParentDsGmPriority2Value,
        [Read][PtpOrdinaryClock][PtpOcParentDsGmVarianceValue] = readPtpOcParentDsGmVarianceValue,
        [Read][PtpOrdinaryClock][PtpOcParentDsGmAccuracyValue] = readPtpOcParentDsGmAccuracyValue,
        [Read][PtpOrdinaryClock][PtpOcParentDsGmClassValue] = readPtpOcParentDsGmClassValue,
        [Read][PtpOrdinaryClock][PtpOcParentDsGmShortIdValue] = readPtpOcParentDsGmShortIdValue,
        [Read][PtpOrdinaryClock][PtpOcParentDsGmInaccuracyValue] = readPtpOcParentDsGmInaccuracyValue,
        [Read][PtpOrdinaryClock][PtpOcParentDsNwInaccuracyValue] = readPtpOcParentDsNwInaccuracyValue,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsTimeSourceValue] = readPtpOcTimePropertiesDsTimeSourceValue,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsPtpTimescaleStatus] = readPtpOcTimePropertiesDsPtpTimescaleStatus,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsFreqTraceableStatus] = readPtpOcTimePropertiesDsFreqTraceableStatus,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsTimeTraceableStatus] = readPtpOcTimePropertiesDsTimeTraceableStatus,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsLeap61Status] = readPtpOcTimePropertiesDsLeap61Status,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsLeap59Status] = readPtpOcTimePropertiesDsLeap59Status,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsUtcOffsetValStatus] = readPtpOcTimePropertiesDsUtcOffsetValStatus,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsUtcOffsetValue] = readPtpOcTimePropertiesDsUtcOffsetValue,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsCurrentOffsetValue] = readPtpOcTimePropertiesDsCurrentOffsetValue,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsJumpSecondsValue] = readPtpOcTimePropertiesDsJumpSecondsValue,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsNextJumpValue] = readPtpOcTimePropertiesDsNextJumpValue,
        [Read][PtpOrdinaryClock][PtpOcTimePropertiesDsDisplayNameValue] = readPtpOcTimePropertiesDsDisplayNameValue,

        [Write][PtpOrdinaryClock][PtpOcVersion] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcInstanceNumber] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcVlanAddress] = readPtpOcVlanAddress,
        [Write][PtpOrdinaryClock][PtpOcVlanStatus] = readPtpOcVlanStatus,
        [Write][PtpOrdinaryClock][PtpOcProfile] = readPtpOcProfile,
        [Write][PtpOrdinaryClock][PtpOcLayer] = readPtpOcLayer,
        [Write][PtpOrdinaryClock][PtpOcDelayMechanismValue] = readPtpOcDelayMechanismValue,
        [Write][PtpOrdinaryClock][PtpOcIpAddress] = readPtpOcIpAddress,
        [Write][PtpOrdinaryClock][PtpOcStatus] = readPtpOcStatus,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsClockId] = readPtpOcDefaultDsClockId,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsDomain] = readPtpOcDefaultDsDomain,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsPriority1] = readPtpOcDefaultDsPriority1,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsPriority2] = readPtpOcDefaultDsPriority2,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsAccuracy] = readPtpOcDefaultDsAccuracy,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsClass] = readPtpOcDefaultDsClass,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsVariance] = readPtpOcDefaultDsVariance,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsShortId] = readPtpOcDefaultDsShortId,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsInaccuracy] = readPtpOcDefaultDsInaccuracy,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsNumberOfPorts] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsTwoStepStatus] = readPtpOcDefaultDsTwoStepStatus,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsSignalingStatus] = readPtpOcDefaultDsSignalingStatus,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsMasterOnlyStatus] = readPtpOcDefaultDsMasterOnlyStatus,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsSlaveOnlyStatus] = readPtpOcDefaultDsSlaveOnlyStatus,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsListedUnicastSlavesOnlyStatus] = readPtpOcDefaultDsListedUnicastSlavesOnlyStatus,
        [Write][PtpOrdinaryClock][PtpOcDefaultDsDisableOffsetCorrectionStatus] = readPtpOcDefaultDsDisableOffsetCorrectionStatus,
        [Write][PtpOrdinaryClock][PtpOcPortDsPeerDelayValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcPortDsState] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcPortDsAsymmetryValue] = readPtpOcPortDsAsymmetryValue,
        [Write][PtpOrdinaryClock][PtpOcPortDsMaxPeerDelayValue] = readPtpOcPortDsMaxPeerDelayValue,
        [Write][PtpOrdinaryClock][PtpOcPortDsPDelayReqLogMsgIntervalValue] = readPtpOcPortDsPDelayReqLogMsgIntervalValue,
        [Write][PtpOrdinaryClock][PtpOcPortDsDelayReqLogMsgIntervalValue] = readPtpOcPortDsDelayReqLogMsgIntervalValue,
        [Write][PtpOrdinaryClock][PtpOcPortDsDelayReceiptTimeoutValue] = readPtpOcPortDsDelayReceiptTimeoutValue,
        [Write][PtpOrdinaryClock][PtpOcPortDsAnnounceLogMsgIntervalValue] = readPtpOcPortDsAnnounceLogMsgIntervalValue,
        [Write][PtpOrdinaryClock][PtpOcPortDsAnnounceReceiptTimeoutValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcPortDsSyncLogMsgIntervalValue] = readPtpOcPortDsSyncLogMsgIntervalValue,
        [Write][PtpOrdinaryClock][PtpOcPortDsSyncReceiptTimeoutValue] = readPtpOcPortDsSyncReceiptTimeoutValue,
        [Write][PtpOrdinaryClock][PtpOcCurrentDsStepsRemovedValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcCurrentDsOffsetValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcCurrentDsDelayValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcParentDsParentClockIdValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcParentDsGmClockIdValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcParentDsGmPriority1Value] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcParentDsGmPriority2Value] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcParentDsGmVarianceValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcParentDsGmAccuracyValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcParentDsGmClassValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcParentDsGmShortIdValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcParentDsGmInaccuracyValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcParentDsNwInaccuracyValue] = readOnly,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsTimeSourceValue] = readPtpOcTimePropertiesDsTimeSourceValue,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsPtpTimescaleStatus] = readPtpOcTimePropertiesDsPtpTimescaleStatus,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsFreqTraceableStatus] = readPtpOcTimePropertiesDsFreqTraceableStatus,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsTimeTraceableStatus] = readPtpOcTimePropertiesDsTimeTraceableStatus,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsLeap61Status] = readPtpOcTimePropertiesDsLeap61Status,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsLeap59Status] = readPtpOcTimePropertiesDsLeap59Status,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsUtcOffsetValStatus] = readPtpOcTimePropertiesDsUtcOffsetValStatus,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsUtcOffsetValue] = readPtpOcTimePropertiesDsUtcOffsetValue,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsCurrentOffsetValue] = readPtpOcTimePropertiesDsCurrentOffsetValue,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsJumpSecondsValue] = readPtpOcTimePropertiesDsJumpSecondsValue,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsNextJumpValue] = readPtpOcTimePropertiesDsNextJumpValue,
        [Write][PtpOrdinaryClock][PtpOcTimePropertiesDsDisplayNameValue] = readPtpOcTimePropertiesDsDisplayNameValue,

        [Read][NtpServer][NtpServerVersion] = readNtpServerVersion,
        [Read][NtpServer][NtpServerInstanceNumber] = readNtpServerInstanceNumber,
        [Read][NtpServer][NtpServerMacAddress] = readNtpServerMacAddress,
        [Read][NtpServer][NtpServerVlanAddress] = readNtpServerVlanAddress,
        [Read][NtpServer][NtpServerVlanStatus] = readNtpServerVlanStatus,
        [Read][NtpServer][NtpServerIpMode] = readNtpServerIpMode,
        [Read][NtpServer][NtpServerIpAddress] = readNtpServerIpAddress,
        [Read][NtpServer][NtpServerUnicastMode] = readNtpServerUnicastMode,
        [Read][NtpServer][NtpServerMulticastMode] = readNtpServerMulticastMode,
        [Read][NtpServer][NtpServerBroadcastMode] = readNtpServerBroadcastMode,
        [Read][NtpServer][NtpServerStatus] = readNtpServerStatus,
        [Read][NtpServer][NtpServerStratumValue] = readNtpServerStratumValue,
        [Read][NtpServer][NtpServerPollIntervalValue] = readNtpServerPollIntervalValue,
        [Read][NtpServer][NtpServerPrecisionValue] = readNtpServerPrecisionValue,
        [Read][NtpServer][NtpServerReferenceIdValue] = readNtpServerReferenceId,
        [Read][NtpServer][NtpServerLeap59Status] = readNtpServerLeap59Status,
        [Read][NtpServer][NtpServerLeap59InProgress] = readNtpServerLeap59InProgress,
        [Read][NtpServer][NtpServerLeap61Status] = readNtpServerLeap61Status,
        [Read][NtpServer][NtpServerLeap61InProgress] = readNtpServerLeap61InProgress,
        [Read][NtpServer][NtpServerUtcSmearingStatus] = readNtpServerSmearingStatus,
        [Read][NtpServer][NtpServerUtcOffsetStatus] = readNtpServerUtcOffsetStatus,
        [Read][NtpServer][NtpServerUtcOffsetValue] = readNtpServerUtcOffsetValue,
        [Read][NtpServer][NtpServerRequestsValue] = readNtpServerRequestsValue,
        [Read][NtpServer][NtpServerResponsesValue] = readNtpServerResponsesValue,
        [Read][NtpServer][NtpServerRequestsDroppedValue] = readNtpServerRequestsDroppedValue,
        [Read][NtpServer][NtpServerBroadcastsValue] = readNtpServerBroadcastsValue,
        [Read][NtpServer][NtpServerClearCountersStatus] = writeOnly,

        [Write][NtpServer][NtpServerVersion] = readOnly,
        [Write][NtpServer][NtpServerInstanceNumber] = readOnly,
        [Write][NtpServer][NtpServerMacAddress] = writeNtpServerMacAddress,
        [Write][NtpServer][NtpServerVlanAddress] = writeNtpServerVlanAddress,
        [Write][NtpServer][NtpServerVlanStatus] = writeNtpServerVlanStatus,
        [Write][NtpServer][NtpServerIpMode] = writeNtpServerIpMode,
        [Write][NtpServer][NtpServerIpAddress] = writeNtpServerIpAddress,
        [Write][NtpServer][NtpServerUnicastMode] = writeNtpServerUnicastMode,
        [Write][NtpServer][NtpServerMulticastMode] = writeNtpServerMulticastMode,
        [Write][NtpServer][NtpServerBroadcastMode] = writeNtpServerBroadcastMode,
        [Write][NtpServer][NtpServerStatus] = writeNtpServerStatus,
        [Write][NtpServer][NtpServerStratumValue] = writeNtpServerStratumValue,
        [Write][NtpServer][NtpServerPollIntervalValue] = writeNtpServerPollIntervalValue,
        [Write][NtpServer][NtpServerPrecisionValue] = writeNtpServerPrecisionValue,
        [Write][NtpServer][NtpServerReferenceIdValue] = writeNtpServerReferenceIdValue,
        [Write][NtpServer][NtpServerLeap59Status] = writeNtpServerLeap59Status,
        [Write][NtpServer][NtpServerLeap59InProgress] = readOnly,
        [Write][NtpServer][NtpServerLeap61Status] = writeNtpServerLeap61Status,
        [Write][NtpServer][NtpServerLeap61InProgress] = readOnly,
        [Write][NtpServer][NtpServerUtcSmearingStatus] = writeNtpServerUtcSmearingStatus,
        [Write][NtpServer][NtpServerUtcOffsetStatus] = writeNtpServerUtcOffsetStatus,
        [Write][NtpServer][NtpServerUtcOffsetValue] = writeNtpServerUtcOffsetValue,
        [Write][NtpServer][NtpServerRequestsValue] = readOnly,
        [Write][NtpServer][NtpServerResponsesValue] = readOnly,
        [Write][NtpServer][NtpServerRequestsDroppedValue] = readOnly,
        [Write][NtpServer][NtpServerBroadcastsValue] = readOnly,
        [Write][NtpServer][NtpServerClearCountersStatus] = writeNtpServerClearCountersStatus,

        [Read][PpsSlave][PpsSlaveVersion] = readPpsSlaveVersion,
        [Read][PpsSlave][PpsSlaveInstanceNumber] = readPpsSlaveInstanceNumber,
        [Read][PpsSlave][PpsSlaveStatus] = readPpsSlaveEnableStatus,
        [Read][PpsSlave][PpsSlavePolarity] = readPpsSlavePolarity,
        [Read][PpsSlave][PpsSlaveInputOkStatus] = readPpsSlaveInputOkStatus,
        [Read][PpsSlave][PpsSlavePulseWidthValue] = readPpsSlavePulseWidthValue,
        [Read][PpsSlave][PpsSlaveCableDelayValue] = readPpsSlaveCableDelayValue,

        [Write][PpsSlave][PpsSlaveCableDelayValue] = writePpsSlaveCableDelayValue,
        [Write][PpsSlave][PpsSlaveCableDelayValue] = writePpsSlaveCableDelayValue,
        [Write][PpsSlave][PpsSlavePolarity] = writePpsSlavePolarity,
        [Write][PpsSlave][PpsSlaveStatus] = writePpsSlaveEnableStatus,

        [Read][TodSlave][TodSlaveVersion] = readTodSlaveVersion,
        [Read][TodSlave][TodSlaveInstance] = readTodSlaveInstance,
        [Read][TodSlave][TodSlaveProtocol] = readTodSlaveProtocol,
        [Read][TodSlave][TodSlaveGnss] = readTodSlaveGnss,
        [Read][TodSlave][TodSlaveMsgDisable] = readTodSlaveMsgDisable,
        [Read][TodSlave][TodSlaveCorrection] = readTodSlaveCorrection,
        [Read][TodSlave][TodSlaveBaudRate] = readTodSlaveBaudRate,
        [Read][TodSlave][TodSlaveInvertedPolarity] = readTodSlaveInvertedPolarity,
        [Read][TodSlave][TodSlaveUtcOffset] = readTodSlaveUtcOffset,
        [Read][TodSlave][TodSlaveUtcInfoValid] = readTodSlaveUtcInfoValid,
        [Read][TodSlave][TodSlaveLeapAnnounce] = readTodSlaveLeapAnnounce,
        [Read][TodSlave][TodSlaveLeap59] = readTodSlaveLeap59,
        [Read][TodSlave][TodSlaveLeap61] = readTodSlaveLeap61,
        [Read][TodSlave][TodSlaveLeapInfoValid] = readTodSlaveLeapInfoValid,
        [Read][TodSlave][TodSlaveTimeToLeap] = readTodSlaveTimeToLeap,
        [Read][TodSlave][TodSlaveGnssFix] = readTodSlaveGnssFix,
        [Read][TodSlave][TodSlaveGnssFixOk] = readTodSlaveGnssFixOk,
        [Read][TodSlave][TodSlaveSpoofingState] = readTodSlaveSpoofingState,
        [Read][TodSlave][TodSlaveFixAndSpoofingInfoValid] = readTodSlaveFixAndSpoofingInfoValid,
        [Read][TodSlave][TodSlaveJammingLevel] = readTodSlaveJammingLevel,
        [Read][TodSlave][TodSlaveJammingState] = readTodSlaveJammingState,
        [Read][TodSlave][TodSlaveAntennaState] = readTodSlaveAntennaState,
        [Read][TodSlave][TodSlaveAntennaAndJammingInfoValid] = readTodSlaveAntennaAndJammingInfoValid,
        [Read][TodSlave][TodSlaveNrOfSatellitesSeen] = readTodSlaveNrOfSatellitesSeen,
        [Read][TodSlave][TodSlaveNrOfSatellitesLocked] = readTodSlaveNrOfSatellitesLocked,
        [Read][TodSlave][TodSlaveNrOfSatellitesInfo] = readTodSlaveNrOfSatellitesInfo,
        [Read][TodSlave][TodSlaveEnable] = readTodSlaveEnable,
        [Read][TodSlave][TodSlaveInputOk] = readTodSlaveInputOk,

        [Write][TodSlave][TodSlaveProtocol] = writeTodSlaveProtocol,
        [Write][TodSlave][TodSlaveGnss] = writeTodSlaveGnss,
        [Write][TodSlave][TodSlaveMsgDisable] = writeTodSlaveMsgDisable,
        [Write][TodSlave][TodSlaveCorrection] = writeTodSlaveCorrection,
        [Write][TodSlave][TodSlaveBaudRate] = writeTodSlaveBaudRate,
        [Write][TodSlave][TodSlaveInvertedPolarity] = writeTodSlaveInvertedPolarity,
        [Write][TodSlave][TodSlaveEnable] = writeTodSlaveEnable,

};

int Axi(char *operation, char *core, char *property, char *value)
{
    int op_id = getOperationId(operation);
    // printf("op id: %d\n", op_id);

    if (op_id < 0)
    {
        return -1;
    }

    int core_id = getCoreId(core);
    // printf("core id: %d\n", core_id);
    if (core_id < 0)
    {
        return -1;
    }

    int property_id = getPropertyId(core_id, property);
    // printf("property id: %d\n", property_id);

    if (property_id < 0)
    {

        return -1;
    }

    int err = timeServer[op_id][core_id][property_id](value, 64);

    if (err != 0)
    {
        return -1;
    }

    printf("%s -> %s -> %s -> %s\n", operation, core, property, value);

    return 0;
}

int readOnly(char *buf, size_t size)
{
    // snprintf(buf, size, "%s", "read-only");
    // return 0;
    //
    // exec("read", "ntp-server", "ip-address", )

    return 0;
}

int writeOnly(char *buf, size_t size)
{
    snprintf(buf, size, "%s", "write-only");
    return 0;
}

int connect(void)
{
    // printf("connect called\n");

    char connectCommand[] = "$CC*00\r\n";
    char writeData[64] = {0};
    char readData[64] = {0};

    // printf("write data array: %s\n", writeData);
    char *FPGA_PORT = getenv("FPGA_PORT");

    int ser = serOpen(FPGA_PORT);
    if (ser == -1)
    {

        printf("connect Error opening serial port\n");
        return -1;
    }

    strcpy(writeData, connectCommand);

    int err = serWrite(ser, writeData, strlen(writeData));
    // usleep(2000); //

    if (err != 0)
    {
        printf("serWrite error\n");
        return -1;
    }

    err = serRead(ser, readData, sizeof(readData));
    if (err != 0)
    {
        printf("connect - serRead error\n");
        return -1;
    }

    // serClose(ser);

    if (isChecksumCorrect(readData) != 0)
    {
        printf("connect readData: %s\n", readData);
        printf("connect check sum wrong\n");
        return -1;
    }

    printf("Connect: Success\n");

    return 0;
}

int reset(void)
{
    // printf("connect called\n");

    char connectCommand[] = "$SC*10\r\n ";
    char writeData[64] = {0};
    char readData[64] = {0};

    // printf("write data array: %s\n", writeData);
    char *FPGA_PORT = getenv("FPGA_PORT");

    int ser = serOpen(FPGA_PORT);
    if (ser == -1)
    {

        printf("c Error opening serial port\n");
        return -1;
    }

    strcpy(writeData, connectCommand);

    int err = serWrite(ser, writeData, strlen(writeData));
    // usleep(2000); //

    if (err != 0)
    {
        printf("serWrite error\n");
        return -1;
    }

    err = serRead(ser, readData, sizeof(readData));
    if (err != 0)
    {
        printf("connect - serRead error\n");
        return -1;
    }

    // serClose(ser);

    if (isChecksumCorrect(readData) != 0)
    {
        printf("connect readData: %s\n", readData);
        printf("connect check sum wrong\n");
        return -1;
    }

    // printf("Connect: Success\n");

    return 0;
}

int RawWrite(char *addr, char *data)
{
    char *err;
    long raw_addr = strtol(addr, &err, 16);

    if (err == addr || *err != '\0')
    {
        return -1;
    }

    long raw_data = strtol(data, &err, 16);

    if (err == data || *err != '\0')
    {
        return -1;
    }

    if (writeRegister(raw_addr, &raw_data) != 0)
    {
        return -1;
    }

    return 0;
}

//
unsigned char calculateChecksum(char *data)
{
    // char out[3] = {0};

    unsigned char checksum = 0;
    for (int i = 1; i < strlen(data); i++)
    {
        if ('*' == data[i])
        {
            break;
        }
        checksum = checksum ^ data[i];
    }

    // sprintf(out, "%02X", checksum); // convert to two chars wide

    return checksum;
}

int isChecksumCorrect(char *message)
{
    char calculatedChecksum[64];
    char *messageChecksum;
    char *cmdAddressData;

    if (0 == strlen(message))
    {
        return -1;
    }

    cmdAddressData = strtok(message, "*");

    if (cmdAddressData == NULL)
    {
        return -2;
    }

    messageChecksum = strtok(NULL, "*"); // assign token to pointer then add a zero to end the string right after the two checksum digits

    if (messageChecksum == NULL)
    {
        return -3;
    }

    messageChecksum[2] = 0;

    // printf("cmd AddressData var: %s\n", cmdAddressData);

    sprintf(calculatedChecksum, "%02X", calculateChecksum(cmdAddressData)); // important formats checksum

    if (strncmp(calculatedChecksum, messageChecksum, 3) == 0)
    {
        return 0;
    }
    return -1;
}

int isErrorResponse(char *message)
{
    if (strncmp("$ER", message, 3) == 0)
    {
        return 1;
    }

    return 0;
}

int isReadResponse(char *message)
{
    if (strncmp("$RR", message, 3) == 0)
    {
        return 1;
    }

    return 0;
}

int isWriteResponse(char *message)
{
    if (strncmp("$WR", message, 3) == 0)
    {
        return 1;
    }

    return 0;
}

int readRegister(int64_t addr, int64_t *data)
{
    char writeData[64] = {0};
    char readData[64] = {0};
    // char tempData[32] = {0};
    char hexAddr[64] = {0};
    char hexData[64] = {0};
    char hexChecksum[3] = {0};

    char *FPGA_PORT = getenv("FPGA_PORT");

    int ser = serOpen(FPGA_PORT);
    if (ser == -1)
    {

        printf("r Error opening serial port\n");
        return -1;
    }

    // build message
    strcat(writeData, "$RC,");

    sprintf(hexAddr, "0x%08lx", addr); // convert to hex string

    strcat(writeData, hexAddr);
    // printf("write data array: %s\n", writeData);

    char checksum = calculateChecksum(writeData);
    sprintf(hexChecksum, "%02X", checksum); // convert to hex string
    strcat(writeData, "*");

    strcat(writeData, hexChecksum);
    strcat(writeData, "\r\n");

    // printf("writeRegister: %s", writeData);

    // send message
    int err = serWrite(ser, writeData, strlen(writeData));
    if (err != 0)
    {
        printf("serWrite error");
        return -1;
    }

    // usleep(50000);
    //    receive message
    err = serRead(ser, readData, sizeof(readData));
    // printf("read data: %s \n", readData);

    if (err != 0)
    {
        printf("read - serRead error\n");
        return -1;
    }
    // close
    serClose(ser);

    if (isErrorResponse(readData))
    {
        printf("error response: %s \n", readData);
        return -1;
    }

    if (!isReadResponse(readData))
    {
        printf("missing read response\n");
        return -1;
    }

    if (isChecksumCorrect(readData) != 0)
    {
        printf("read reg - wrong checksum\n");
        return -1;
    }

    for (int i = 0; i < 8; i++)
    {
        hexData[i] = readData[i + 17];
    }

    hexData[8] = '\0';

    // printf("hex data: %s\n", hexData);
    *data = (int64_t)strtol(hexData, NULL, 16);
    // printf("Read Response: %s \n", readData);

    return 0;
}

int writeRegister(int64_t addr, int64_t *data)
{

    char writeData[64] = {0};
    char readData[64] = {0};
    // char tempData[32] = {0};
    char hexAddr[64] = {0};
    char hexData[64] = {0};
    char hexChecksum[3] = {0};

    char *FPGA_PORT = getenv("FPGA_PORT");

    int ser = serOpen(FPGA_PORT);
    if (ser == -1)
    {

        printf("w Error opening serial port\n");
        return -1;
    }

    // build message
    strcat(writeData, "$WC,");

    sprintf(hexAddr, "0x%08lx", addr); // convert to hex string

    strcat(writeData, hexAddr);

    strcat(writeData, ",");

    sprintf(hexData, "0x%08lx", *data);

    strcat(writeData, hexData);

    // printf("write data array: %s\n", writeData);

    char checksum = calculateChecksum(writeData);
    sprintf(hexChecksum, "%02X", checksum); // convert to hex string
    strcat(writeData, "*");

    strcat(writeData, hexChecksum);
    strcat(writeData, "\r\n");

    // printf("write data array: %s\n", writeData);

    // send message
    int err = serWrite(ser, writeData, strlen(writeData));
    if (err != 0)
    {
        printf("serWrite error \n");
        return -1;
    }

    // usleep(2000);
    //   receive message
    err = serRead(ser, readData, sizeof(readData));
    if (err != 0)
    {
        printf("write - serRead error \n");
        return -1;
    }
    // close
    // serClose(ser);

    if (isErrorResponse(readData))
    {
        printf("error response: %s", readData);
        return -1;
    }

    if (!isWriteResponse(readData))
    {

        printf("missing write response \n");
        // printf("read data: %s\n", readData);
        return -1;
    }

    if (isChecksumCorrect(readData) != 0)
    {
        printf("wrong checksum \n");
        return -1;
    }

    // printf("Write Response: %s \n", readData);

    return 0;
}

struct termios tty;

int serOpen(char fileDescriptor[])
{
    int fd = open(fileDescriptor, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        printf("ser open error\n");
        return -1;
    }

    setupTermios(fd);

    return fd;
}

int serClose(int fileDescriptor)
{

    close(fileDescriptor);

    return 0;
}

int serRead(int ser, char data[], size_t dataLength)
{
    char temp;
    int index = 0;
    int totalRead = 0;
    int consecutiveTimeouts = 0;

    // memset(data, 0, dataLength);

    while (index < dataLength - 1)
    {
        int numRead = read(ser, &temp, 1);

        if (numRead < 0)
        {
            perror("serial read error");
            return -1;
        }
        else if (numRead == 0)
        {
            // Timeout - but maybe more data is coming
            consecutiveTimeouts++;
            if (consecutiveTimeouts > 5) // Give up after 5 timeouts
            {
                printf("serRead timeout after %d bytes\n", totalRead);
                break;
            }
            continue;
        }

        consecutiveTimeouts = 0; // Reset timeout counter
        totalRead++;

        // Check for line ending
        if (temp == '\n')
        {
            break; // Complete line received
        }
        else if (temp == '\r')
        {
            continue; // Skip \r, don't store it
        }

        data[index] = temp;
        index++;
    }

    data[index] = '\0';

    if (totalRead > 0)
    {
        // printf("Serial Read %d bytes: '%s'\n", totalRead, data);
    }

    return 0;
}

int serWrite(int ser, char data[], size_t dataLength)
{
    int numWrote = write(ser, data, dataLength);
    if (numWrote <= 0)
    {
        printf("serial write error\n");
        return -1;
    }
    // printf("Serial Write %d bytes: %s", numWrote, data);
    return 0;
}

int setupTermios(int fd)
{
    // printf("setup termios\n");
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(fd, &tty) != 0)
    {
        printf("tcgetattr");
        close(fd);
        return -1;
    }

    cfsetospeed(&tty, B115200); // Use a standard baud rate unless you know otherwise
    cfsetispeed(&tty, B115200);

    // 8N1 configuration
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    // tty.c_cflag &= ~CRTSCTS; // Disable hardware flow control
    tty.c_cflag |= (CLOCAL | CREAD);

    // Input processing - disable all special processing
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Output processing - raw output
    tty.c_oflag &= ~OPOST;

    // CRITICAL FIX: Disable canonical mode for character-by-character reading
    tty.c_lflag &= ~ICANON; // Raw mode - read character by character
    tty.c_lflag &= ~(ECHO | ECHONL | ISIG | IEXTEN);

    // Timeout settings for raw mode
    tty.c_cc[VMIN] = 0;  // Don't wait for minimum characters
    tty.c_cc[VTIME] = 5; // Timeout in deciseconds (0.5 seconds)

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        printf("tcsetattr error? \n");
        perror("tcsetattr");

        close(fd);
        return -1;
    }

    tcflush(fd, TCIOFLUSH);
    return 0;
}