// driver.c
#include "axi.h"
#include "ntpServer.h"
#include "cores.h"
int main() // switch to main to use
{

    connect();

    getCores();

    char firstIp[32] = {0};
    char secondIp[32] = {0};
    char newIp[32] = "10.1.10.225";

    int res = readNtpServerIpAddress(firstIp, sizeof(firstIp));
    printf("result: %d\n", res);
    printf("read ip: %s\n", firstIp);

    res = writeNtpServerIpAddress(newIp, sizeof(newIp));
    printf("result: %d\n", res);
    printf("wrote ip: %s\n", newIp);

    res = readNtpServerIpAddress(secondIp, sizeof(secondIp));
    printf("result: %d\n", res);
    printf("read ip: %s\n", secondIp);

    res = writeNtpServerIpAddress(firstIp, sizeof(firstIp));
    printf("result: %d\n", res);
    printf("wrote ip: %s\n", firstIp);

    res = readNtpServerIpAddress(secondIp, sizeof(secondIp));
    printf("result: %d\n", res);
    printf("read ip: %s\n", secondIp);

    return 0;
}