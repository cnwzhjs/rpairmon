#pragma once

#include <rp/infra_config.h>
#include <rp/util/noncopyable.h>
#include <memory>

namespace rp { namespace ui {

    class Label;
    class Button;
    class Pane;
    class Scene;

} }

namespace tony { namespace home_portal { namespace ui {

    class StatusBar;
    class RpWeatherSensor;
    class YahooWeather;
    class Meter;
    class ForecastIndicator;
    class ConditionIndicator;

    class Application : public std::enable_shared_from_this<Application>, private rp::util::noncopyable {
    public:
        Application();
        ~Application();

    public:
        void start();

    public:
        std::shared_ptr<rp::ui::Scene> getScene();

    public:
        static std::shared_ptr<Application> defaultApplication();

    private:
        void onUpdate_();

        static void onUpdateWrap_(std::weak_ptr<Application> self);

    private:
        std::shared_ptr<rp::ui::Scene> scene_;
        std::shared_ptr<StatusBar> statusBar_;
        std::shared_ptr<Meter> temperatureMeter_;
        std::shared_ptr<Meter> humidityMeter_;
        std::shared_ptr<ConditionIndicator> conditionIndicator_;
        std::shared_ptr<ForecastIndicator> forecastIndicator_;

    private:
        std::shared_ptr<RpWeatherSensor> weatherSensor_;
        std::shared_ptr<YahooWeather> yahooWeather_;

    private:
        bool started_;
    };

} } }
