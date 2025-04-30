#include "serialInterface.h"
#include <string.h>

// reads n bytes where n is the size of the input array
int serialRead(int ser, char data[], size_t dataLength)
{
    int numRead = read(ser, data, dataLength);
    if (numRead <= 0)
    {
        return -1;
    }
    return 0;
}