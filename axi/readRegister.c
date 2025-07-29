
#include "axi.h"

int readRegister(int64_t addr, int64_t *data)
{
    char writeData[64] = {0};
    char readData[64] = {0};
    // char tempData[32] = {0};
    char hexAddr[64] = {0};
    char hexData[64] = {0};
    char hexChecksum[3] = {0};

    char *FPGA_PORT = getenv("FPGA_PORT");

    int ser = serOpen(FPGA_PORT);
    if (ser == -1)
    {

        printf("Error opening serial port\n");
        return -1;
    }

    // build message
    strcat(writeData, "$RC,");

    sprintf(hexAddr, "0x%08lx", addr); // convert to hex string

    strcat(writeData, hexAddr);
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
        printf("serWrite error");
        return -1;
    }

    usleep(1000);
    // receive message
    err = serRead(ser, readData, sizeof(readData));
    if (err != 0)
    {
        printf("read - serRead error\n");
        return -1;
    }
    // close
    serClose(ser);

    if (isErrorResponse(readData))
    {
        printf("error response: %s \n", readData);
        return -1;
    }

    if (!isReadResponse(readData))
    {
        printf("missing read response\n");
        return -1;
    }

    if (isChecksumCorrect(readData))
    {
        printf("read reg - wrong checksum\n");
        return -1;
    }

    for (int i = 0; i < 8; i++)
    {
        hexData[i] = readData[i + 17];
    }

    *data = (int64_t)strtol(hexData, NULL, 16);
    // printf("Read Response: %s \n", readData);

    return 0;
}