
#include "axi.h"

int serRead(int ser, char data[], size_t dataLength)
{
    char temp;
    int index = 0;
    int totalRead = 0;
    int consecutiveTimeouts = 0;

    // memset(data, 0, dataLength);

    while (index < dataLength - 1)
    {
        int numRead = read(ser, &temp, 1);

        if (numRead < 0)
        {
            perror("serial read error");
            return -1;
        }
        else if (numRead == 0)
        {
            // Timeout - but maybe more data is coming
            consecutiveTimeouts++;
            if (consecutiveTimeouts > 5) // Give up after 5 timeouts
            {
                printf("serRead timeout after %d bytes\n", totalRead);
                break;
            }
            continue;
        }

        consecutiveTimeouts = 0; // Reset timeout counter
        totalRead++;

        // Check for line ending
        if (temp == '\n')
        {
            break; // Complete line received
        }
        else if (temp == '\r')
        {
            continue; // Skip \r, don't store it
        }

        data[index] = temp;
        index++;
    }

    data[index] = '\0';

    if (totalRead > 0)
    {
        // printf("Serial Read %d bytes: '%s'\n", totalRead, data);
    }

    return 0;
}