#include "axi.h"
#include "serialInterface.h"
#include <string.h>
// send a connect message
int connect(void)
{
	char connectCommand[] = "$CC*00\r\n";
	char writeData[32] = {0};
	char readData[32] = {0};

	// printf("write data array: %s\n", writeData);

	int ser = serialOpen("/dev/ttyUSB0");

	if (ser == -1)
	{
		serialClose(ser);
		return -1;
	}

	strcpy(writeData, connectCommand);

	int err = serialWrite(ser, writeData, strlen(writeData));

	// usleep(1000); // 1ms
	serialSleep();

	if (err != 0)
	{
		return -1;
	}

	err = serialRead(ser, readData, sizeof(readData));
	if (err != 0)
	{
		return -1;
	}

	serialClose(ser);

	if (isChecksumCorrect(readData) != 0)
	{
		// perror("connect check sum wrong");
		return -1;
	}

	return 0;
}