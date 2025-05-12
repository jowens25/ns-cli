#include "axi.h"
//
int connect(void)
{
    // printf("connect called");

    char connectCommand[] = "$CC*00\r\n";
    char writeData[32] = {0};
    char readData[32] = {0};

    // printf("write data array: %s\n", writeData);

    int ser = serOpen("/dev/ttyUSB0");
    if (ser == -1)
    {

        printf("Error opening serial port \n");
        return -1;
    }

    strcpy(writeData, connectCommand);

    int err = serWrite(ser, writeData, strlen(writeData));
    usleep(1000); //

    if (err != 0)
    {
        printf("serWrite error\n");
        return -1;
    }

    err = serRead(ser, readData, sizeof(readData));
    if (err != 0)
    {
        printf("serRead error\n");
        return -1;
    }

    close(ser);

    if (isChecksumCorrect(readData) != 0)
    {
        printf("connect check sum wrong\n");
        return -1;
    }

    printf("Connect: Success\n");

    return 0;
}