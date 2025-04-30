#ifndef SERIAL_INTERFACE_H

#define SERIAL_INTERFACE_H

#ifdef __linux__
#include <fcntl.h>   // linux
#include <unistd.h>  // linux
#include <termios.h> // linux
#include <stdio.h>

#endif

// common
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern struct termios tty;

int serialSetup(int);

int serialOpen(char fileDescriptor[]);
void serialClose(int fd);

int serialRead(int ser, char data[], size_t dataLength);
int serialWrite(int ser, char data[], size_t dataLength);
void serialSleep(void);

void serialPrintln(const char msg[]);

#endif // SERIAL_INTERFACE_H