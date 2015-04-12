#include <tony/home_portal/ui/drv_yahoo_weather.h>
#include <chrono>
#include <string.h>

#ifndef _WIN32
#   include <curl/curl.h>
#   include <json/json.h>
#else
    typedef int CURLcode;
    class CURL;

    CURL* curl_easy_init();
    void curl_easy_setopt(CURL* curl, int opt, void* value);
    CURLcode curl_easy_perform(CURL* curl);
    void curl_easy_cleanup(CURL* curl);
    const char* curl_easy_strerror(CURLcode code);

#   define CURLOPT_URL 0
#   define CURLOPT_FOLLOWLOCATION 1
#   define CURLOPT_WRITEDATA 2
#   define CURLOPT_WRITEFUNCTION 3

#   define CURLE_OK 0
#endif

namespace tony { namespace home_portal { namespace ui {

    static const char* yahooWeatherQueryUrl = "http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20weather.bylocation%20where%20location%3D%22shanghai%2C%20cn%22%20and%20unit%3D%22c%22&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys";

    YahooWeather::YahooWeather()
    {
        locationCondition_.code = 0;
        locationCondition_.codeText = "Updating...";
        locationCondition_.temperature = 0;
        locationCondition_.humidity = 0;
        locationCondition_.pressure = 0;
        locationCondition_.visibility = 0;
        locationCondition_.windChill = 0;
        locationCondition_.windDirection = 0;
        locationCondition_.windSpeed = 0;
    }

    YahooWeather::~YahooWeather()
    {
        stop();
    }

    void YahooWeather::start()
    {
        std::lock_guard<std::mutex> guard(lock_);

        if (working_)
            return;

        working_ = true;

        workingThread_ = std::move(std::thread(std::bind(&YahooWeather::worker_, shared_from_this())));
    }

    void YahooWeather::stop()
    {
        {
            std::lock_guard<std::mutex> guard(lock_);

            if (!working_)
                return;

            working_ = false;
        }

        if (workingThread_.joinable())
            workingThread_.join();
    }

    LocationCondition YahooWeather::getLocationCondition() const
    {
        std::lock_guard<std::mutex> guard(lock_);
        return locationCondition_;
    }

    std::vector<Forecast> YahooWeather::getForecast() const
    {
        std::lock_guard<std::mutex> guard(lock_);
        return forecast_;
    }

    void YahooWeather::worker_()
    {
        std::vector<char> buffer;
        CURL* curl = curl_easy_init();

        if (!curl)
        {
            fprintf(stderr, "Failed to initialize curl\n");

            std::lock_guard<std::mutex> guard(lock_);
            working_ = false;
            return;
        }

        while (true)
        {
            {
                std::lock_guard<std::mutex> guard(lock_);

                if (!working_)
                    break;
            }

            curl_easy_setopt(curl, CURLOPT_URL, yahooWeatherQueryUrl);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &YahooWeather::curlWriteCallback_);

            auto startTime = std::chrono::high_resolution_clock::now();

            CURLcode result = curl_easy_perform(curl);

            if (result == CURLE_OK)
            {
                Json::Reader reader;
                Json::Value root;

                const char* begin = &buffer[0];
                const char* end = begin + buffer.size();

                if (reader.parse(begin, end, root, false))
                {
                    const Json::Value& weather = root["query"]["results"]["weather"]["rss"]["channel"];

                    LocationCondition locationCondition;
                    std::vector<Forecast> forecast;

                    locationCondition.code = std::stol(weather["item"]["condition"]["code"].asString());
                    locationCondition.codeText = weather["item"]["condition"]["text"].asString();
                    locationCondition.temperature = std::stod(weather["item"]["condition"]["temp"].asString());
                    locationCondition.humidity = std::stod(weather["atmosphere"]["humidity"].asString());
                    locationCondition.pressure = std::stod(weather["atmosphere"]["pressure"].asString());
                    locationCondition.visibility = std::stod(weather["atmosphere"]["visibility"].asString());
                    locationCondition.sunrise = weather["astronomy"]["sunrise"].asString();
                    locationCondition.sunset = weather["astronomy"]["sunset"].asString();
                    locationCondition.windChill = std::stod(weather["wind"]["chill"].asString());
                    locationCondition.windDirection = std::stod(weather["wind"]["direction"].asString());
                    locationCondition.windSpeed = std::stod(weather["wind"]["speed"].asString());

                    const Json::Value& forecastParentNode = weather["item"]["forecast"];

                    for (size_t i = 0; i < forecastParentNode.size(); i++)
                    {
                        const Json::Value& forecastNode = forecastParentNode[(int)i];

                        Forecast currentForecast;

                        currentForecast.code = std::stol(forecastNode["code"].asString());
                        currentForecast.codeText = forecastNode["text"].asString();
                        currentForecast.date = forecastNode["date"].asString();
                        currentForecast.day = forecastNode["day"].asString();
                        currentForecast.low = std::stod(forecastNode["low"].asString());
                        currentForecast.high = std::stod(forecastNode["high"].asString());

                        forecast.push_back(currentForecast);
                    }

                    std::lock_guard<std::mutex> guard(lock_);
                    locationCondition_ = locationCondition;
                    forecast_ = forecast;
                }

                buffer.clear();
            }
            else
            {
                fprintf(stderr, "Failed to fetch Yahoo weather data: %s\n", curl_easy_strerror(result));
            }
            auto endTime = std::chrono::high_resolution_clock::now();

            int usedSeconds = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

            if (usedSeconds < 60)
            {
                std::this_thread::sleep_for(std::chrono::seconds(60 - usedSeconds));
            }
        }

        if (curl)
        {
            curl_easy_cleanup(curl);
        }
    }

    size_t YahooWeather::curlWriteCallback_(char* ptr, size_t size, size_t nmemb, void* userdata)
    {
        if ((size * nmemb) == 0)
            return 0;

        std::vector<char>& buffer = *reinterpret_cast<std::vector<char>*>(userdata);

        size_t readBytes = size * nmemb;
        size_t originalSize = buffer.size();

        buffer.resize(originalSize + readBytes);
        memcpy(&buffer[originalSize], ptr, readBytes);

        return readBytes;
    }



} } }
