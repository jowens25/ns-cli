
#include "axi.h"
#include <stdint.h>
#include <string.h>
#include "serialInterface.h"
int readRegister(int64_t addr, int64_t *data)
{
    char writeData[32] = {0};
    char readData[32] = {0};
    // char tempData[32] = {0};
    char hexAddr[32] = {0};
    // char hexData[32] = {0};
    char hexChecksum[3] = {0};

    int ser = serialOpen("/dev/ttyUSB0");
    if (ser == -1)
    {
        close(ser);
        printf("Error opening serial port");
        return -1;
    }

    // build message
    strcat(writeData, "$RC,");

    sprintf(hexAddr, "0x%08lx", addr); // convert to hex string

    strcat(writeData, hexAddr);
    printf("write data array: %s\n", writeData);

    char checksum = calculateChecksum(writeData);
    sprintf(hexChecksum, "%02x", checksum); // convert to hex string
    strcat(writeData, "*");

    strcat(writeData, hexChecksum);
    strcat(writeData, "\r\n");

    printf("write data array: %s\n", writeData);

    // send message
    int err = serialWrite(ser, writeData, strlen(writeData));
    if (err != 0)
    {
        printf("serialWrite error");
        return -1;
    }

    usleep(1000);
    // receive message
    err = serialRead(ser, readData, sizeof(readData));
    if (err != 0)
    {
        printf("serialRead error");
        return -1;
    }
    // close
    close(ser);

    if (isErrorResponse(readData))
    {
        printf("error response");
        return -1;
    }

    if (!isReadResponse(readData))
    {
        printf("missing read response");
        return -1;
    }

    if (isChecksumCorrect(readData))
    {
        printf("wrong checksum");
        return -1;
    }

    printf("readRegister");

    return 0;
}