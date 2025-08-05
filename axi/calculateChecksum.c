
#include "axi.h"
//
unsigned char calculateChecksum(char *data)
{
    // char out[3] = {0};

    unsigned char checksum = 0;
    for (int i = 1; i < strlen(data); i++)
    {
        if ('*' == data[i])
        {
            break;
        }
        checksum = checksum ^ data[i];
    }

    // sprintf(out, "%02X", checksum); // convert to two chars wide

    return checksum;
}