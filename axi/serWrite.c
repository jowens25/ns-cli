
#include "axi.h" // writes n bytes where n is the str len of the input
int serWrite(int ser, char data[], size_t dataLength)
{
    int numWrote = write(ser, data, dataLength);
    if (numWrote <= 0)
    {
        printf("serial write error\n");
        return -1;
    }
    // printf("Serial Write %d bytes: %s", numWrote, data);
    return 0;
}