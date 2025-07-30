#ifndef CLK_CLOCK

#define CLK_CLOCK
#include <stddef.h>

#define Ucm_ClkClock_ControlReg 0x00000000
#define Ucm_ClkClock_StatusReg 0x00000004
#define Ucm_ClkClock_SelectReg 0x00000008
#define Ucm_ClkClock_VersionReg 0x0000000C
#define Ucm_ClkClock_TimeValueLReg 0x00000010
#define Ucm_ClkClock_TimeValueHReg 0x00000014
#define Ucm_ClkClock_TimeAdjValueLReg 0x00000020
#define Ucm_ClkClock_TimeAdjValueHReg 0x00000024
#define Ucm_ClkClock_OffsetAdjValueReg 0x00000030
#define Ucm_ClkClock_OffsetAdjIntervalReg 0x00000034
#define Ucm_ClkClock_DriftAdjValueReg 0x00000040
#define Ucm_ClkClock_DriftAdjIntervalReg 0x00000044
#define Ucm_ClkClock_InSyncThresholdReg 0x00000050
#define Ucm_ClkClock_ServoOffsetFactorPReg 0x00000060
#define Ucm_ClkClock_ServoOffsetFactorIReg 0x00000064
#define Ucm_ClkClock_ServoDriftFactorPReg 0x00000068
#define Ucm_ClkClock_ServoDriftFactorIReg 0x0000006C
#define Ucm_ClkClock_StatusOffsetReg 0x00000070
#define Ucm_ClkClock_StatusDriftReg 0x00000074
#define Ucm_ClkClock_StatusOffsetFractionsReg 0x00000078
#define Ucm_ClkClock_StatusDriftFractionsReg 0x0000007C

#define ClkClockVersion 0
#define ClkClockInstance 1
#define ClkClockStatus 2
#define ClkClockSeconds 3
#define ClkClockNanoseconds 4
#define ClkClockTimeAdj 5
#define ClkClockInSync 6
#define ClkClockInHoldover 7
#define ClkClockInSyncThreshold 8
#define ClkClockSource 9
#define ClkClockDrift 10
#define ClkClockDriftInterval 11
#define ClkClockDriftAdj 12
#define ClkClockOffset 13
#define ClkClockOffsetInterval 14
#define ClkClockOffsetAdj 15
#define ClkClockPiOffsetMulP 16
#define ClkClockPiOffsetDivP 17
#define ClkClockPiOffsetMulI 18
#define ClkClockPiOffsetDivI 19
#define ClkClockPiDriftMulP 20
#define ClkClockPiDriftDivP 21
#define ClkClockPiDriftMulI 22
#define ClkClockPiDriftDivI 23
#define ClkClockPiSetCustomParameters 24
#define ClkClockCorrectedOffset 25
#define ClkClockCorrectedDrift 26
#define ClkClockDate 27

extern char *ClkClockProperties[29];
int hasClkClock(char *in, size_t size);
int readClkClockVersion(char *version, size_t size);
int readClkClockInstance(char *instance, size_t size);
int readClkClockStatus(char *status, size_t size);
int readClkClockSeconds(char *seconds, size_t size);
int readClkClockNanoseconds(char *nanoseconds, size_t size);
// int readClkClockTimeAdj(char *timeadj, size_t size);
int readClkClockInSync(char *insync, size_t size);
int readClkClockInHoldover(char *inholdover, size_t size);
int readClkClockInSyncThreshold(char *insyncthreshold, size_t size);
int readClkClockSource(char *source, size_t size);
int readClkClockDrift(char *drift, size_t size);
int readClkClockDriftInterval(char *driftinterval, size_t size);
int readClkClockDriftAdj(char *driftadj, size_t size);
int readClkClockOffset(char *offset, size_t size);
int readClkClockOffsetInterval(char *offsetinterval, size_t size);
int readClkClockOffsetAdj(char *offsetadj, size_t size);
// int readClkClockPiOffsetMulP(char *pioffsetmulp, size_t size);
// int readClkClockPiOffsetDivP(char *pioffsetdivp, size_t size);
// int readClkClockPiOffsetMulI(char *pioffsetmuli, size_t size);
// int readClkClockPiOffsetDivI(char *pioffsetdivi, size_t size);
// int readClkClockPiDriftMulP(char *pidriftmulp, size_t size);
// int readClkClockPiDriftDivP(char *pidriftdivp, size_t size);
// int readClkClockPiDriftMulI(char *pidriftmuli, size_t size);
// int readClkClockPiDriftDivI(char *pidriftdivi, size_t size);
int readClkClockPiSetCustomParameters(char *pisetcustomparameters, size_t size);
int readClkClockCorrectedOffset(char *correctedoffset, size_t size);
int readClkClockCorrectedDrift(char *correcteddrift, size_t size);
int readClkClockDate(char *date, size_t size);

int writeClkClockInSyncThreshold(char *insyncthreshold, size_t size);
int writeClkClockSeconds(char *seconds, size_t size);
int writeClkClockNanoseconds(char *seconds, size_t size);
int writeClkClockOffset(char *offset, size_t size);
int writeClkClockOffsetInterval(char *interval, size_t size);

int writeClkClockDrift(char *drift, size_t size);
int writeClkClockDriftInterval(char *driftinterval, size_t size);

#endif // CLK_CLOCK
