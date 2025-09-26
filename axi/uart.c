#include <stdint.h>
#include <stddef.h>
#include "axi.h"

int writeReadUart(char *cmd, char *response)
{

    char readData[1024] = {0};

    int ser = serOpen("/dev/ttymxc2");
    if (ser == -1)
    {

        printf("connect Error opening serial port\n");
        return -1;
    }

    int err = serWrite(ser, cmd, strlen(cmd));

    err = serRead(ser, readData, sizeof(readData));

    if (err != 0)
    {
        printf("read - serRead error\n");
        return -1;
    }
    // close
    serClose(ser);

    printf("%s\n", readData);

    return 0;
}
