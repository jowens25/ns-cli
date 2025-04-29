
#include "axi.h"
#include <stdint.h>
#include <string.h>
#include "serialInterface.h"

int writeRegister(int32_t addr, int32_t *data)
{
    char writeData[32] = {0};
    char readData[32] = {0};
    // char tempData[32] = {0};
    char hexAddr[32] = {0};
    char hexData[32] = {0};
    char hexChecksum[3] = {0};

    int ser = serialOpen("/dev/ttyUSB0");
    if (ser == -1)
    {
        serialClose(ser);
        perror("Error opening serial port");
        return -1;
    }

    // build message
    strcat(writeData, "$WC,");

    sprintf(hexAddr, "0x%08x", addr); // convert to hex string

    strcat(writeData, hexAddr);

    strcat(writeData, ",");

    sprintf(hexData, "0x%08x", *data);

    strcat(writeData, hexData);

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
        perror("serialWrite error");
        return -1;
    }

    usleep(1000);
    // receive message
    err = serialRead(ser, readData, sizeof(readData));
    if (err != 0)
    {
        perror("serialRead error");
        return -1;
    }
    // close
    close(ser);

    if (isErrorResponse(readData))
    {
        perror("error response");
        return -1;
    }

    if (!isWriteResponse(readData))
    {
        perror("missing write response");
        return -1;
    }

    if (isChecksumCorrect(readData))
    {
        perror("wrong checksum");
        return -1;
    }

    perror("writeRegister");

    return 0;
}