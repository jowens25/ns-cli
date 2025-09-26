#ifndef AXI_H

#define AXI_H

// temp port selection

#ifdef __linux__
#include <fcntl.h>   // linux
#include <unistd.h>  // linux
#include <termios.h> // linux
#endif

#ifdef MCU

#endif

// common
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stddef.h>

// extern const char *FPGA_PORT;

extern struct termios tty;

int connect(void);

int serOpen(char fileDescriptor[]);
int serClose(int fileDescriptor);

int serRead(int ser, char data[], size_t dataLength);
int serWrite(int ser, char data[], size_t dataLength);

unsigned char calculateChecksum(char *data);

int isWriteResponse(char *message);
int isReadResponse(char *message);
int isErrorResponse(char *message);
int isChecksumCorrect(char *message);

int setupTermios(int);

int readRegister(int64_t addr, int64_t *data);
int writeRegister(int64_t addr, int64_t *data);
int RawWrite(char *addr, char *data);

// int AxiRead(char *core, char *property, char *value);
int Axi(char *operation, char *core, char *property, char *value);

int readOnly(char *buf, size_t size);

int writeOnly(char *buf, size_t size);

extern int64_t temp_data;
extern int64_t temp_addr;

typedef int (*read_write_func)(char *value, size_t size);

#endif // AXI_H