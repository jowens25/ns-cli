
#ifndef PPS_SLAVE_H

#define PPS_SLAVE_H

#define Ucm_PpsSlave_ControlReg 0x00000000
#define Ucm_PpsSlave_StatusReg 0x00000004
#define Ucm_PpsSlave_PolarityReg 0x00000008
#define Ucm_PpsSlave_VersionReg 0x0000000C
#define Ucm_PpsSlave_PulseWidthReg 0x00000010
#define Ucm_PpsSlave_CableDelayReg 0x00000020

#define PpsSlaveVersion 0
#define PpsSlaveInstanceNumber 1
#define PpsSlaveStatus 2
#define PpsSlavePolarity 3
#define PpsSlaveInputOkStatus 4
#define PpsSlavePulseWidthValue 5
#define PpsSlaveCableDelayValue 6

extern char *PpsSlaveProperties[8];

int hasPpsSlave(char *in, size_t size);
int readPpsSlaveVersion(char *value, size_t size);
int readPpsSlaveInstanceNumber(char *instanceNumber, size_t size);
int readPpsSlaveEnableStatus(char *status, size_t size);
int readPpsSlavePolarity(char *status, size_t size);
int readPpsSlaveInputOkStatus(char *status, size_t size);
int readPpsSlavePulseWidthValue(char *value, size_t size);
int readPpsSlaveCableDelayValue(char *value, size_t size);
int writePpsSlaveCableDelayValue(char *cable_delay, size_t size);
int writePpsSlaveCableDelayValue(char *cable_delay, size_t size);
int writePpsSlavePolarity(char *status, size_t size);
int writePpsSlaveEnableStatus(char *status, size_t size);

#endif