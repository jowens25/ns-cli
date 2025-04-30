#include "serialInterface.h"
#include <string.h>
void serialPrintln(const char msg[])
{
    printf("%s\n", msg);
    fwrite(msg, 1, sizeof(msg), stdout); // Prints "$CR*11"
}