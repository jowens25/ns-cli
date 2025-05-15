#include "axi.h"

int serClose(int fileDescriptor)
{

    close(fileDescriptor);

    return 0;
}