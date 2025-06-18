

#include "axi.h"
//
int isReadResponse(char *message)
{
    if (strncmp("$RR", message, 3) == 0)
    {
        return 1;
    }

    return 0;
}