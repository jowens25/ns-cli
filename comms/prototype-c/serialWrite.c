#include "serialInterface.h"
// writes n bytes where n is the size of the input array
int serialWrite(int ser, char data[], size_t dataLength)
{
    int numWrote = write(ser, data, dataLength);
    if (numWrote <= 0)
    {
        return -1;
    }
    return 0;
}