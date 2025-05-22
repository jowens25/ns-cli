#ifndef AXI_H

#define AXI_H

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

extern int64_t temp_data;
extern int64_t temp_addr;

#endif // AXI_H