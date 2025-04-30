
#include "axi.h"
//
int isChecksumCorrect(char *message)
{

    char *cmdAddressData = strtok(message, "*");
    char *messageChecksum = strtok(NULL, "*");

    char hexChecksum[32] = {0};

    sprintf(hexChecksum, "%02X", calculateChecksum(cmdAddressData)); // important formats checksum
    strcat(hexChecksum, "\r\n");                                     // add back normal ended for comparison

    // printf("hexChecksum: %s", hexChecksum);
    // printf("messageChecksum: %s", messageChecksum);

    if (strcmp(hexChecksum, messageChecksum) == 0)
    {
        // printf("check sums equal %s\n", hexChecksum);
        return 0;
    }
    return -1;
}