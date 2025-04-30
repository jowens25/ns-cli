#ifndef AXI_H

#define AXI_H

#include "serialInterface.h"
#include <stdio.h>
int connect(void);

unsigned char calculateChecksum(char *data);

int isWriteResponse(char *message);
int isReadResponse(char *message);
int isErrorResponse(char *message);
int isChecksumCorrect(char *message);

#endif // AXI_H