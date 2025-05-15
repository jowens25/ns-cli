
#include "axi.h"
//
int isChecksumCorrect(char *message)
{

    printf("is checksum correct message: %s\n", message);

    char *cmdAddressData = strtok(message, "*");
    char *messageChecksum = strtok(NULL, "*");
    printf("cmd AddressData var: %s\n", cmdAddressData);

    char hexChecksum[64] = {0};

    sprintf(hexChecksum, "%02X", calculateChecksum(cmdAddressData)); // important formats checksum
    strcat(hexChecksum, "\r\n");                                     // add back normal ended for comparison

    // printf("hexChecksum: %s", hexChecksum);
    // printf("messageChecksum: %s", messageChecksum);

    printf("hex checksum var: %s\n", hexChecksum);
    printf("message checksum var: %s\n", messageChecksum);

    printf(":::%s:::\n", messageChecksum);

    if (strcmp(hexChecksum, messageChecksum) == 0)
    {
        printf("check sums equal %s\n", hexChecksum);
        return 0;
    }
    printf("fail\n");
    return -1;
}