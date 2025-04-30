#ifndef CLK_CLOCK

#define CLK_CLOCK

#define ControlReg 0x00000000
#define StatusReg 0x00000004
#define SelectReg 0x00000008
#define VersionReg 0x0000000C
#define TimeValueLReg 0x00000010
#define TimeValueHReg 0x00000014
#define TimeAdjValueLReg 0x00000020
#define TimeAdjValueHReg 0x00000024
#define OffsetAdjValueReg 0x00000030
#define OffsetAdjIntervalReg 0x00000034
#define DriftAdjValueReg 0x00000040
#define DriftAdjIntervalReg 0x00000044
#define InSyncThresholdReg 0x00000050
#define ServoOffsetFactorPReg 0x00000060
#define ServoOffsetFactorIReg 0x00000064
#define ServoDriftFactorPReg 0x00000068
#define ServoDriftFactorIReg 0x0000006C
#define StatusOffsetReg 0x00000070
#define StatusDriftReg 0x00000074
#define StatusOffsetFractionsReg 0x00000078
#define StatusDriftFractionsReg 0x0000007C

#endif // CLK_CLOCK
