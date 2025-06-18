
#include "axi.h"
//
int isErrorResponse(char *message)
{
    if (strncmp("$ER", message, 3) == 0)
    {
        return 1;
    }

    return 0;
}