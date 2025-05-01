#include "axi.h"
#include "ntpServer.h"
#include "config.h"
int main()
{

    connect();

    readConfig();
    char mac[32] = {0};

    readNtpServerMacAddress(mac, sizeof(mac));

    printf("this happens to be the mac address of serial port hardcoded in my stuff: %s\n", mac);

    return 0;
}