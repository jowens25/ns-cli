#include "axi.h"
#include "serialInterface.h"
#include <string.h>
// send a connect message
int connect(void)
{
	printf("connect called");

	char connectCommand[] = "$CC*00\r\n";
	char writeData[32] = {0};
	char readData[32] = {0};

	printf("write data array: %s\n", writeData);

	int ser = serialOpen("/dev/ttyUSB0");
	if (ser == -1)
	{
		close(ser);
		printf("Error opening serial port");
		return -1;
	}

	strcpy(writeData, connectCommand);

	int err = serialWrite(ser, writeData, strlen(writeData));
	usleep(1000); //

	if (err != 0)
	{
		printf("serWrite error");
		return -1;
	}

	err = serialRead(ser, readData, sizeof(readData));
	if (err != 0)
	{
		printf("serRead error");
		return -1;
	}

	close(ser);

	if (isChecksumCorrect(readData) != 0)
	{
		printf("connect check sum wrong");
		return -1;
	}

	printf("Connect");

	return 0;
}