#ifndef SERIAL_INTERFACE_H

#define SERIAL_INTERFACE_H

#ifdef __linux__
#include <fcntl.h>   // linux
#include <unistd.h>  // linux
#include <termios.h> // linux
#include <stdio.h>
#endif

extern struct termios tty;

int serialSetup(int);

int serialOpen(char fileDescriptor[]);
void serialClose(int fd);

int serialRead(int ser, char data[], size_t dataLength);
int serialWrite(int ser, char data[], size_t dataLength);
void serialSleep(void);

#endif // SERIAL_INTERFACE_H