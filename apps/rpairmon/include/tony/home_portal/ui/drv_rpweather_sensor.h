#pragma once

#include <string>
#include <memory>

namespace tony { namespace home_portal { namespace ui {

    class RpWeatherSensor {
    public:
        RpWeatherSensor(const std::string& serialPort);
        ~RpWeatherSensor();

    public:
        double getTemperature();
        double getHumidity();

        void update();

    private:
        void onNewLineReceived_();

    private:
        int fd_;

        double temperature_;
        double humidity_;

        std::string currentLine_;
    };

} } }
