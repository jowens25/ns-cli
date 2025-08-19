#include <stdint.h>
#include <stddef.h>

#include "mySerial.h"

void append(char *dest, char *src, size_t dest_size)
{
    strncat(dest, src, dest_size - strlen(dest) - 1);
}

unsigned char CalculateChecksum(char *data)
{
    // char out[3] = {0};

    unsigned char checksum = 0;
    for (int i = 1; i < strlen(data); i++)
    {
        if ('$' == data[i])
        {
            continue;
        }
        if ('*' == data[i])
        {
            break;
        }

        checksum = checksum ^ data[i];
    }

    // sprintf(out, "%02X", checksum); // convert to two chars wide

    return checksum;
}

int IsChecksumCorrect(char *message)
{
    char calculatedChecksum[64];
    char *messageChecksum;
    char *cmdAddressData;

    if (0 == strlen(message))
    {
        return -1;
    }

    cmdAddressData = strtok(message, "*");

    if (cmdAddressData == NULL)
    {
        return -2;
    }

    messageChecksum = strtok(NULL, "*"); // assign token to pointer then add a zero to end the string right after the two checksum digits

    if (messageChecksum == NULL)
    {
        return -3;
    }

    messageChecksum[2] = 0;

    // printf("cmd AddressData var: %s\n", cmdAddressData);

    sprintf(calculatedChecksum, "%02X", CalculateChecksum(cmdAddressData)); // important formats checksum

    if (strncmp(calculatedChecksum, messageChecksum, 3) == 0)
    {
        return 0;
    }
    return -1;
}

int ReadValue(char *command)
{
    char writeData[64] = {0};
    char readData[64] = {0};
    // char tempData[32] = {0};
    char hexAddr[64] = {0};
    char hexData[64] = {0};
    char hexChecksum[3] = {0};

    int ser = MySerialOpen("/dev/ttyUSB0");
    if (ser == -1)
    {

        printf("r Error opening serial port\n");
        return -1;
    }

    // build message
    append(writeData, "$", sizeof(writeData));

    append(writeData, command, sizeof(writeData));

    append(writeData, "*", sizeof(writeData));

    char checksum = CalculateChecksum(writeData);
    sprintf(hexChecksum, "%02X", checksum); // convert to hex string

    append(writeData, hexChecksum, sizeof(writeData));
    append(writeData, "\r\n", sizeof(writeData));

    printf("writeRegister: %s", writeData);

    // send message
    int err = MySerialWrite(ser, writeData, strlen(writeData));
    if (err != 0)
    {
        printf("serWrite error");
        return -1;
    }

    // usleep(50000);
    //    receive message
    err = MySerialRead(ser, readData, sizeof(readData));
    printf("read data: %s \n", readData);

    if (err != 0)
    {
        printf("read - serRead error\n");
        return -1;
    }
    // close
    MySerialClose(ser);

    if (IsChecksumCorrect(readData) != 0)
    {
        printf("read reg - wrong checksum\n");
        return -1;
    }

    for (int i = 0; i < 8; i++)
    {
        hexData[i] = readData[i + 17];
    }

    //*data = (int64_t)strtol(hexData, NULL, 16);
    // printf("Read Response: %s \n", readData);

    return 0;
}

int WriteValue(char *command, char *parameter)
{
    char writeData[64] = {0};
    char readData[64] = {0};
    // char tempData[32] = {0};
    char hexAddr[64] = {0};
    char hexData[64] = {0};
    char hexChecksum[3] = {0};

    int ser = MySerialOpen("/dev/ttymxc2");
    if (ser == -1)
    {

        printf("r Error opening serial port\n");
        return -1;
    }

    // build message
    append(writeData, "$", sizeof(writeData));
    append(writeData, command, sizeof(writeData));
    append(writeData, "=", sizeof(writeData));
    append(writeData, parameter, sizeof(writeData));
    append(writeData, "*", sizeof(writeData));

    char checksum = CalculateChecksum(writeData);
    sprintf(hexChecksum, "%02X", checksum); // convert to hex string

    append(writeData, hexChecksum, sizeof(writeData));
    append(writeData, "\r\n", sizeof(writeData));

    printf("writeRegister: %s", writeData);

    // send message
    int err = MySerialWrite(ser, writeData, strlen(writeData));
    if (err != 0)
    {
        printf("serWrite error");
        return -1;
    }

    // usleep(50000);
    //    receive message
    err = MySerialRead(ser, readData, sizeof(readData));
    printf("read data: %s \n", readData);

    if (err != 0)
    {
        printf("read - serRead error\n");
        return -1;
    }
    // close
    MySerialClose(ser);

    if (IsChecksumCorrect(readData) != 0)
    {
        printf("read reg - wrong checksum\n");
        return -1;
    }

    for (int i = 0; i < 8; i++)
    {
        hexData[i] = readData[i + 17];
    }

    //*data = (int64_t)strtol(hexData, NULL, 16);
    // printf("Read Response: %s \n", readData);

    return 0;
}

struct termios mcu_tty;

int MySerialOpen(char fileDescriptor[])
{
    int fd = open(fileDescriptor, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        printf("open error\n");
        return -1;
    }

    SetupTermios(fd);

    return fd;
}

int MySerialClose(int fileDescriptor)
{

    close(fileDescriptor);

    return 0;
}

int MySerialRead(int ser, char data[], size_t dataLength)
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

int MySerialWrite(int ser, char data[], size_t dataLength)
{
    int numWrote = write(ser, data, dataLength);
    if (numWrote <= 0)
    {
        printf("serial write error\n");
        return -1;
    }
    // printf("Serial Write %d bytes: %s", numWrote, data);
    return 0;
}

int SetupTermios(int fd)
{
    // printf("setup termios\n");
    memset(&mcu_tty, 0, sizeof(mcu_tty));

    if (tcgetattr(fd, &mcu_tty) != 0)
    {
        printf("tcgetattr");
        close(fd);
        return -1;
    }

    cfsetospeed(&mcu_tty, B38400); // Use a standard baud rate unless you know otherwise
    cfsetispeed(&mcu_tty, B38400);

    // 8N1 configuration
    mcu_tty.c_cflag &= ~CSIZE;
    mcu_tty.c_cflag |= CS8;
    mcu_tty.c_cflag &= ~PARENB;
    mcu_tty.c_cflag &= ~CSTOPB;
    // mcu_tty.c_cflag &= ~CRTSCTS; // Disable hardware flow control
    mcu_tty.c_cflag |= (CLOCAL | CREAD);

    // Input processing - disable all special processing
    mcu_tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    mcu_tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Output processing - raw output
    mcu_tty.c_oflag &= ~OPOST;

    // CRITICAL FIX: Disable canonical mode for character-by-character reading
    mcu_tty.c_lflag &= ~ICANON; // Raw mode - read character by character
    mcu_tty.c_lflag &= ~(ECHO | ECHONL | ISIG | IEXTEN);

    // Timeout settings for raw mode
    mcu_tty.c_cc[VMIN] = 0;  // Don't wait for minimum characters
    mcu_tty.c_cc[VTIME] = 5; // Timeout in deciseconds (0.5 seconds)

    if (tcsetattr(fd, TCSANOW, &mcu_tty) != 0)
    {
        printf("tcsetattr error? \n");
        perror("tcsetattr");

        close(fd);
        return -1;
    }

    tcflush(fd, TCIOFLUSH);
    return 0;
}