#include "serialInterface.h"
// open a serial port
int serialOpen(char fileDescriptor[])
{
    int fd = open(fileDescriptor, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        return -1;
    }

    serialSetup(fd);

    return fd;
}