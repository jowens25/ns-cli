
#include "axi.h"

int writeRegister(int64_t addr, int64_t *data)
{
    char writeData[32] = {0};
    char readData[32] = {0};
    // char tempData[32] = {0};
    char hexAddr[32] = {0};
    char hexData[32] = {0};
    char hexChecksum[3] = {0};

    int ser = serOpen("/dev/ttyUSB0");
    if (ser == -1)
    {

        printf("Error opening serial port\n");
        return -1;
    }

    // build message
    strcat(writeData, "$WC,");

    sprintf(hexAddr, "0x%08lx", addr); // convert to hex string

    strcat(writeData, hexAddr);

    strcat(writeData, ",");

    sprintf(hexData, "0x%08lx", *data);

    strcat(writeData, hexData);

    // printf("write data array: %s\n", writeData);

    char checksum = calculateChecksum(writeData);
    sprintf(hexChecksum, "%02X", checksum); // convert to hex string
    strcat(writeData, "*");

    strcat(writeData, hexChecksum);
    strcat(writeData, "\r\n");

    // printf("write data array: %s\n", writeData);

    // send message
    int err = serWrite(ser, writeData, strlen(writeData));
    if (err != 0)
    {
        printf("serWrite error\n");
        return -1;
    }

    usleep(1000);
    // receive message
    err = serRead(ser, readData, sizeof(readData));
    if (err != 0)
    {
        printf("serRead error\n");
        return -1;
    }
    // close
    close(ser);

    if (isErrorResponse(readData))
    {
        printf("error response: %s", readData);
        return -1;
    }

    if (!isWriteResponse(readData))
    {
        printf("missing write response\n");
        return -1;
    }

    if (isChecksumCorrect(readData))
    {
        printf("wrong checksum\n");
        return -1;
    }

    // printf("writeRegister\n");

    return 0;
}