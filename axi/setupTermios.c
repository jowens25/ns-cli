#include "axi.h"
struct termios tty;

int setupTermios(int fd)
{
    // printf("setup termios\n");
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(fd, &tty) != 0)
    {
        printf("tcgetattr");
        close(fd);
        return -1;
    }

    cfsetospeed(&tty, B115200); // Use a standard baud rate unless you know otherwise
    cfsetispeed(&tty, B115200);

    // 8N1 configuration
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    // tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= (CLOCAL | CREAD);

    // Input processing for \r\n data
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR);
    tty.c_iflag |= ICRNL; // Convert \r to \n (helps canonical mode see \r\n as line end)
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Output processing
    tty.c_oflag &= ~OPOST;

    // Enable canonical mode for line-buffered input
    tty.c_lflag |= ICANON;
    tty.c_lflag &= ~(ECHO | ECHONL | ISIG | IEXTEN);

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        printf("tcsetattr error? \n");
        perror("tcsetattr");

        close(fd);
        return -1;
    }

    tcflush(fd, TCIOFLUSH);
    return 0;
}