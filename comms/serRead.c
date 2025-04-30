

// #include <stdio.h>
// #include <stdlib.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <termios.h>
#include <string.h>
// #include <stdint.h>
// #include <stdbool.h>
#include "axi.h"
// reads n bytes where n is the size of the input array
int serRead(int ser, char data[], size_t dataLength)
{
    int numRead = read(ser, data, 64);
    if (numRead <= 0)
    {
        printf("serial read error");
        return -1;
    }

    // printf("Serial Read %d bytes: %s", numRead, data);
    return 0;
}