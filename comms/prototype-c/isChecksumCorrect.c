
#include "axi.h"
#include <string.h>

int isChecksumCorrect(char *message)
{
    // fprintf("string 2: %s", *token + strlen(token) + 1);
    // printf("message: %s\n", message);
    // printf("message checksum: %s\n", messageChecksum);
    //    // printf("hex check sum: %s\n", hexChecksum);

    char *cmdAddressData = strtok(message, "*");
    char *messageChecksum = strtok(NULL, "*");

    char hexChecksum[32] = {0};

    sprintf(hexChecksum, "%x", calculateChecksum(cmdAddressData));
    strcat(hexChecksum, "\r\n"); // add back normal ended for comparison

    if (strcmp(hexChecksum, messageChecksum) == 0)
    {
        // printf("check sums equal %s\n", hexChecksum);
        return 0;
    }
    return -1;
}