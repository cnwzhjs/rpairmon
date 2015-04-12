#pragma once

#include <string>
#include <mutex>
#include <thread>
#include <memory>
#include <vector>

namespace tony { namespace home_portal { namespace ui {

    struct LocationCondition {
        int code;
        std::string codeText;
        
        double temperature;
        double humidity;
        double pressure;
        double visibility;
        
        std::string sunrise;
        std::string sunset;

        double windChill;
        double windDirection;
        double windSpeed;
    };

    struct Forecast {
        int code;
        std::string codeText;
        std::string date;
        std::string day;
        double low;
        double high;
    };

    class YahooWeather : public std::enable_shared_from_this<YahooWeather>
    {
    public:
        YahooWeather();
        ~YahooWeather();

    public:
        void start();
        void stop();

    public:
        LocationCondition getLocationCondition() const;
        std::vector<Forecast> getForecast() const;

    private:
        void worker_();

    private:
        static size_t curlWriteCallback_(char* ptr, size_t size, size_t nmemb, void* userdata);

    private:
        mutable std::mutex lock_;
        std::thread workingThread_;
        
        bool working_;

    private:
        LocationCondition locationCondition_;
        std::vector<Forecast> forecast_;
    };

} } }
