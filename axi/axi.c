#include <stdint.h>
#include "axi.h"
#include "ntpServer.h"
#include "ptpOc.h"
#include "coreConfig.h"
#include "config.h"
int64_t temp_data = 0x00000000;
int64_t temp_addr = 0x00000000;

#define MAX_NUM_PROP 256
#define MAX_NUM_MODS 64
#define MAX_NUM_OPS 2 // 0 read // 1 write
#define Read 0
#define Write 1

read_write_func timeServer[MAX_NUM_OPS][MAX_NUM_MODS][MAX_NUM_PROP] =
    {
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
};

// int AxiRead(char *core, char *property, char *value)
//{
//
//     int core_id = getCoreId(core);
//     printf("core id: %d\n", core_id);
//
//     int property_id = getPropertyId(core_id, property);
//     printf("property id: %d\n", property_id);
//
//     timeServer[Read][core_id][property_id](value, 64);
//
//     printf("read -> %s -> %s -> %s\n", core, property, value);
//
//     return 0;
// }
//
// int AxiWrite(char *core, char *property, char *value)
//{
//     int core_id = getCoreId(core);
//     printf("core id: %d\n", core_id);
//
//     int property_id = getPropertyId(core_id, property);
//     printf("property id: %d\n", property_id);
//
//     int err = timeServer[Write][core_id][property_id](value, 64);
//
//     if (err != 0)
//     {
//         return -1;
//     }
//
//     printf("write -> %s -> %s -> %s\n", core, property, value);
//
//     return 0;
// }

int Axi(char *operation, char *core, char *property, char *value)

{
    int op_id = getOperationId(operation);
    printf("op id: %d\n", op_id);

    if (op_id < 0)
    {
        return -1;
    }

    int core_id = getCoreId(core);
    printf("core id: %d\n", core_id);
    if (core_id < 0)
    {
        return -1;
    }

    int property_id = getPropertyId(core_id, property);
    printf("property id: %d\n", property_id);

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

int exec(int read, int module, int property, char *buf)
{
    //  timeServer[read][module][pro](buf, sizeof(buf));
}

int readOnly(char *buf, size_t size)
{
    // snprintf(buf, size, "%s", "read-only");
    // return 0;
    //
    // exec("read", "ntp-server", "ip-address", )
}

int writeOnly(char *buf, size_t size)
{
    snprintf(buf, size, "%s", "write-only");
    return 0;
}
