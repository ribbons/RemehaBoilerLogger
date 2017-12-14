/*
 * This file is part of Remeha Boiler Logger.
 * Copyright © 2017 by the authors - see the AUTHORS file for details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstring>
#include <iostream>
#include <system_error>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "Serial.h"

Serial::Serial(const std::string& path)
{
    fd = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1)
    {
        throw std::system_error(errno, std::system_category(), "Unable to open " + path);
    }

    int flags = fcntl(fd, F_GETFL, 0);

    if(flags == -1)
    {
        throw std::system_error(errno, std::system_category(), "fcntl(F_GETFL) failed for port");
    }

    // Make calls to read() and write() blocking
    flags &= ~O_NONBLOCK;

    int ret = fcntl(fd, F_SETFL, flags);

    if(ret == -1)
    {
        throw std::system_error(errno, std::system_category(), "fcntl(F_SETFL) failed for port");
    }

    struct termios options;
    ret = tcgetattr(fd, &options);

    if(ret)
    {
        throw std::system_error(errno, std::system_category(), "tcgetattr() for port failed");
    }

    // Set the baud rate
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // Disable parity, 8 data bits, 1 stop-bit (8N1)
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // Enable the receiver and set local mode
    options.c_cflag |= (CLOCAL | CREAD);

    // Read in 'raw mode'
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Disable input options which could modify the data
    options.c_iflag &= ~(BRKINT | INLCR | IGNCR | ICRNL | IXON | IXOFF);

    // Disable all processing of the output data
    options.c_oflag &= ~(OPOST);

    ret = tcsetattr(fd, TCSANOW, &options);

    if(ret)
    {
        throw std::system_error(errno, std::system_category(), "tcsetattr() for port failed");
    }

    // Set up the pollfd structure ready for poll() in ReadBytes
    memset(fds, 0, sizeof(fds));
    fds[0].fd = fd;
    fds[0].events = POLLIN;
}

Serial::~Serial()
{
    int ret = close(fd);

    if(ret)
    {
        std::cerr << "Failed to close port fd";
        std::terminate();
    }
}

std::vector<uint8_t> Serial::ReadBytes()
{
    char readBuffer[256];
    std::vector<uint8_t> allRead;

    // Wait for up to a second before timing out at first
    int pollTimeout = 1000;

    do
    {
        int ret = poll(fds, (sizeof(fds) / sizeof(fds[0])), pollTimeout);

        if(ret < 0)
        {
            throw std::system_error(errno, std::system_category(), "poll() for port data failed");
        }
        else if(ret == 0)
        {
            return allRead;
        }

        ssize_t readCount = read(fd, readBuffer, sizeof(readBuffer));

        if(readCount == -1)
        {
            throw std::system_error(errno, std::system_category(), "read() from port failed");
        }

        allRead.insert(allRead.end(), readBuffer, readBuffer + readCount);

        // As some data has now been read, drop the timeout so we return much
        // faster after the last byte is received
        pollTimeout = 25;
    }
    while(true);
}

void Serial::WriteBytes(const std::vector<uint8_t>& bytes)
{
    ssize_t written = write(fd, bytes.data(), bytes.size());

    if(written == -1)
    {
        throw std::system_error(errno, std::system_category(), "write() to port failed");
    }
}
