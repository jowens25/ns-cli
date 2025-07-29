
#ifndef CONFIG_H
#define CONFIG_H
#define Ucm_Config_BlockSize 16
#define Ucm_Config_TypeInstanceReg 0x00000000
#define Ucm_Config_BaseAddrLReg 0x00000004
#define Ucm_Config_BaseAddrHReg 0x00000008
#define Ucm_Config_IrqMaskReg 0x0000000C
int readConfig(void);

#endif
