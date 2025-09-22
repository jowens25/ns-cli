// driver.c
#include "axi.h"
#include "ntpServer.h"
#include "cores.h"
int main() // switch to main to use
{

    connect();

    getCores();

    char currentIpAddress[32] = {0};
    char newIpAddress[32] = "10.1.10.225";
    char ipMode[32] = {0};

    int res = readNtpServerIpAddress(currentIpAddress, sizeof(currentIpAddress));
    printf("result: %d\n", res);
    printf("read ip: %s\n", currentIpAddress);

    res = writeNtpServerIpAddress(newIpAddress, sizeof(newIpAddress));
    printf("result 1: %d\n", res);
    printf("wrote ip: %s\n", newIpAddress);

    res = readNtpServerIpAddress(currentIpAddress, sizeof(currentIpAddress));
    printf("result: %d\n", res);
    printf("read ip: %s\n", currentIpAddress);

    return 0;
}