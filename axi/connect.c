#include "axi.h"
//
int connect(void)
{
    // printf("connect called");

    char connectCommand[] = "$CC*00\r\n";
    char writeData[64] = {0};
    char readData[64] = {0};

    // printf("write data array: %s\n", writeData);
    char *FPGA_PORT = getenv("FPGA_PORT");

    int ser = serOpen(FPGA_PORT);
    if (ser == -1)
    {

        printf("c Error opening serial port \n");
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
        printf("connect - serRead error\n");
        return -1;
    }

    serClose(ser);

    if (isChecksumCorrect(readData) != 0)
    {
        printf("connect readData: %s\n", readData);
        printf("connect check sum wrong\n");
        return -1;
    }

    printf("Connect: Success\n");

    return 0;
}