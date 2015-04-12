#include <tony/home_portal/ui/drv_rpweather_sensor.h>

#ifndef _WIN32
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#endif

namespace tony { namespace home_portal { namespace ui {

    RpWeatherSensor::RpWeatherSensor(const std::string& serialPort)
        : temperature_(0)
        , humidity_(0)
    {
#ifndef _WIN32
        fd_ = open(serialPort.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

        if (fd_ == -1)
        {
            fprintf(stderr, "Failed to open serial port: %s\n", serialPort.c_str());
            return;
        }

        struct termios config;

        memset(&config, 0, sizeof(config));

        config.c_iflag = 0;
        config.c_oflag = 0;
        config.c_cflag = CS8 | CREAD | CLOCAL;
        config.c_lflag = 0;
        config.c_cc[VMIN] = 1;
        config.c_cc[VTIME] = 5;

        if (cfsetispeed(&config, B38400) < 0 || cfsetospeed(&config, B38400) < 0)
        {
            fprintf(stderr, "Failed to config serial port\n");
            close(fd_);
            fd_ = -1;
            return;
        }

        if (tcsetattr(fd_, TCSAFLUSH, &config) < 0)
        {
            fprintf(stderr, "Failed to config serial port\n");
            close(fd_);
            fd_ = -1;
            return;
        }
#endif
    }

    RpWeatherSensor::~RpWeatherSensor()
    {
#ifndef _WIN32
        if (fd_ != -1)
        {
            close(fd_);
            fd_ = -1;
        }
#endif
    }

    double RpWeatherSensor::getTemperature()
    {
        return temperature_;
    }

    double RpWeatherSensor::getHumidity()
    {
        return humidity_;
    }

    void RpWeatherSensor::update()
    {
#ifndef _WIN32
        if (fd_ == -1)
            return;

        char buffer[256];
        int readBytes = read(fd_, buffer, sizeof(buffer) - 1);

        if (readBytes <= 0)
            return;

        int pos = 0;

        while (pos < readBytes)
        {
            int start = pos;

            for (; pos < readBytes; pos++)
            {
                if (buffer[pos] == '\n')
                    break;
            }

            buffer[pos] = 0;

            currentLine_ += &buffer[start];

            if (pos < readBytes)
            {
                onNewLineReceived_();
                pos++;
            }
        }
#endif
    }

    void RpWeatherSensor::onNewLineReceived_()
    {
        std::string line;

        std::swap(line, currentLine_);

        if (line.size() <= 10)
            return;

        std::string prefix = line.substr(0, 4);

        if (prefix == "temp")
            temperature_ = std::stod(line.substr(line.find(':') + 1));
        else if (prefix == "humi")
            humidity_ = std::stod(line.substr(line.find(':') + 1));
    }

} } }
