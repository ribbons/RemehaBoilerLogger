// Copyright © 2017 Matt Robinson

#include <cstdlib>
#include <system_error>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

static char req_status[] = { 0x02, 0x52, 0x05, 0x06, 0x02, 0x00, 0x53, 0x03 };

int open_port(void)
{
    int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1)
    {
        throw std::system_error(errno, std::system_category(), "Unable to open /dev/ttyUSB0");
    }
    else
    {
        fcntl(fd, F_SETFL, 0);
    }

    struct termios options;
    int ret = tcgetattr(fd, &options);

    if(ret)
    {
        throw std::system_error(errno, std::system_category(), "Failed to get port attributes");
    }

    // Set the baud rate
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // Enable the receiver and set local mode
    options.c_cflag |= (CLOCAL | CREAD);

    // Read in 'raw mode'
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    ret = tcsetattr(fd, TCSANOW, &options);

    if(ret)
    {
        throw std::system_error(errno, std::system_category(), "Failed to set port attributes");
    }

    return (fd);
}

int main(int argc, char* argv[])
{
    int fd = open_port();

    ssize_t written = write(fd, req_status, sizeof(req_status));

    if(written == -1)
    {
        throw std::system_error(errno, std::system_category(), "Failed to write to port");
    }

    char readbytes[100];

    ssize_t readcount = read(fd, readbytes, sizeof(readbytes));

    if(readcount == -1)
    {
        throw std::system_error(errno, std::system_category(), "Failed to read from port");
    }

    for(int i = 0; i < readcount; i++)
    {
        printf(" %02x", readbytes[i]);
    }

    fputc('\n', stdout);

    int ret = close(fd);

    if(ret)
    {
        throw std::system_error(errno, std::system_category(), "Failed to close port fd");
    }

    return EXIT_SUCCESS;
}
