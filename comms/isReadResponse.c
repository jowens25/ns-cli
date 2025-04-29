

#include "axi.h"
#include <string.h>

int isReadResponse(char *message)
{
    if (strncmp("$RR", message, 3) == 0)
    {
        return 1;
    }

    return 0;
}