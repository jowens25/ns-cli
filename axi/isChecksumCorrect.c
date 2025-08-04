
#include "axi.h"
//
int isChecksumCorrect(char *message)
{
    char calculatedChecksum[64];
    char *messageChecksum;
    char *cmdAddressData;
    printf("check sum: %s\n", message);

    // printf("is checksum correct message: %s\n", message);

    cmdAddressData = strtok(message, "*");
    messageChecksum = strtok(NULL, "*"); // assign token to pointer then add a zero to end the string right after the two checksum digits

    messageChecksum[2] = 0;

    // printf("cmd AddressData var: %s\n", cmdAddressData);

    sprintf(calculatedChecksum, "%02X", calculateChecksum(cmdAddressData)); // important formats checksum
    // strcat(hexChecksum, "\r\n");                                     // add back normal ended for comparison

    // printf("hexChecksum: %s", hexChecksum);
    // printf("messageChecksum: %s", messageChecksum);

    // printf("calculated checksum var: %s\n", calculatedChecksum);
    // printf("message checksum var: %s\n", messageChecksum);

    // printf(":::%s:::\n", messageChecksum);

    if (strncmp(calculatedChecksum, messageChecksum, 3) == 0)
    {
        // printf("check sums equal %s\n", calculatedChecksum);
        return 0;
    }
    // printf("fail\n");
    return -1;
}