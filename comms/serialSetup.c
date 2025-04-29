#include "serialInterface.h"
#include <string.h>
struct termios tty;

int serialSetup(int fd)
{

    memset(&tty, 0, sizeof tty);

    if (tcgetattr(fd, &tty) != 0)
    {
        perror("tcgetattr");
        close(fd);
        return -1;
    }

    cfsetospeed(&tty, B1000000); // Use a standard baud rate unless you know otherwise
    cfsetispeed(&tty, B1000000);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 0.001; // .10 second timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        perror("tcsetattr");
        close(fd);
        return -1;
    }

    return 0;
}