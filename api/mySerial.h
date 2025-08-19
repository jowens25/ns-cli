#ifndef MY_SERIAL_H

#define MY_SERIAL_H

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

extern struct termios mcu_tty;

int MySerialOpen(char fileDescriptor[]);
int MySerialClose(int fileDescriptor);

int MySerialRead(int ser, char data[], size_t dataLength);
int MySerialWrite(int ser, char data[], size_t dataLength);

unsigned char CalculateChecksum(char *data);

int IsChecksumCorrect(char *message);

int SetupTermios(int);

int WriteThenRead(char *cmd, char *param);

#endif // MY_SERIAL_H