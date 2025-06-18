

#include "axi.h"
//
int isWriteResponse(char *message)
{
    if (strncmp("$WR", message, 3) == 0)
    {
        return 1;
    }

    return 0;
}