#include <rp/infra_config.h>
#include <tony/home_portal/ui/application.h>
#include <tony/home_portal/ui/status_bar.h>
#include <tony/home_portal/ui/drv_rpweather_sensor.h>
#include <tony/home_portal/ui/drv_yahoo_weather.h>
#include <tony/home_portal/ui/meter.h>
#include <tony/home_portal/ui/forecast_indicator.h>
#include <tony/home_portal/ui/condition_indicator.h>
#include <rp/ui/scene.h>
#include <rp/ui/style_manager.h>
#include <rp/ui/ui_manager.h>
#include <thread>
#include <math.h>

#ifdef _WIN32
#   define snprintf sprintf_s
#endif

using namespace std;
using namespace rp::ui;

namespace tony { namespace home_portal { namespace ui {

    Application::Application()
        : scene_(new Scene("Tony Home Portal"))
        , statusBar_(new StatusBar())
        , weatherSensor_(new RpWeatherSensor("/dev/ttyUSB0"))
        , yahooWeather_(new YahooWeather())
        , temperatureMeter_(new Meter())
        , humidityMeter_(new Meter())
        , conditionIndicator_(new ConditionIndicator())
        , forecastIndicator_(new ForecastIndicator())
        , started_(false)
    {
        auto style = StyleManager::defaultManager();

        int sceneWidth = style->getInteger("Application.SceneWidth", 320);
        int sceneHeight = style->getInteger("Application.SceneHeight", 240);
        auto bgColor = style->getColor("Application.BackgroundColor");
        std::string defaultFont = style->getString("Font.DefaultFontFace", "Verdana");
        auto weatherFontFace = style->getString("Application.WeatherFontFace", "artill_clean_icons");

        rp::ui::Rectangle frame(0, 0, sceneWidth, sceneHeight);

        scene_->setFrame(frame);
        scene_->setBackgroundColor(bgColor);

        temperatureMeter_->setLabel("‘");
        temperatureMeter_->setValue("N/A");
        temperatureMeter_->setUnit("");
        temperatureMeter_->setFrame(rp::ui::Rectangle(0, statusBar_->getHeight(), sceneWidth / 3, (sceneHeight - statusBar_->getHeight())*3/8 - 8));
        temperatureMeter_->setHorizontalAlignment(TextAlignmentNear);
        temperatureMeter_->setLabelFontFace(weatherFontFace);
        temperatureMeter_->setUnitFontFace(weatherFontFace);
        temperatureMeter_->setLabelFontSize(24);
        temperatureMeter_->setUnitFontSize(24);
        scene_->appendChild(temperatureMeter_);

        humidityMeter_->setLabel("5");
        humidityMeter_->setValue("N/A");
        humidityMeter_->setUnit("");
        humidityMeter_->setFrame(rp::ui::Rectangle(temperatureMeter_->getWidth(), statusBar_->getHeight(), sceneWidth / 3, temperatureMeter_->getHeight()));
        humidityMeter_->setHorizontalAlignment(TextAlignmentNear);
        humidityMeter_->setLabelFontFace(weatherFontFace);
        humidityMeter_->setLabelFontSize(24);
        scene_->appendChild(humidityMeter_);

        conditionIndicator_->setFrame(rp::ui::Rectangle(humidityMeter_->getX() + humidityMeter_->getWidth(), statusBar_->getHeight(), sceneWidth / 3, humidityMeter_->getHeight()));
        scene_->appendChild(conditionIndicator_);

        forecastIndicator_->setFrame(rp::ui::Rectangle(
            0, humidityMeter_->getY() + humidityMeter_->getHeight() + 8,
            sceneWidth, sceneHeight - humidityMeter_->getY() - humidityMeter_->getHeight() - 8));
        forecastIndicator_->setLocationCondition(yahooWeather_->getLocationCondition());
        forecastIndicator_->setForecast(yahooWeather_->getForecast());
        scene_->appendChild(forecastIndicator_);

        scene_->appendChild(statusBar_);
    }

    Application::~Application()
    {
        yahooWeather_->stop();
    }

    void Application::start()
    {
        if (started_)
            return;

        started_ = true;

        weak_ptr<Application> self(shared_from_this());

        auto manager = UiManager::defaultManager();
        manager->setOnUpdate(std::bind(&Application::onUpdateWrap_, self));

        yahooWeather_->start();
    }

    std::shared_ptr<Scene> Application::getScene()
    {
        return scene_;
    }

    static shared_ptr<Application> defaultApplication_;
    static once_flag once_;

    static void createDefaultApplication_()
    {
        defaultApplication_ = shared_ptr<Application>(new Application());
    }

    shared_ptr<Application> Application::defaultApplication()
    {
        call_once(once_, createDefaultApplication_);
        return defaultApplication_;
    }

    void Application::onUpdate_()
    {
        weatherSensor_->update();

        char buffer[256];
        snprintf(buffer, 256, "%.1f", weatherSensor_->getTemperature());
        temperatureMeter_->setValue(buffer);

        snprintf(buffer, 256, "%.0f%%", weatherSensor_->getHumidity());
        humidityMeter_->setValue(buffer);

        auto cond = yahooWeather_->getLocationCondition();

        conditionIndicator_->setLocationCondition(cond);
        forecastIndicator_->setLocationCondition(cond);
        forecastIndicator_->setForecast(yahooWeather_->getForecast());

        statusBar_->update();
    }

    void Application::onUpdateWrap_(std::weak_ptr<Application> self)
    {
        auto lockedSelf = self.lock();

        if (!lockedSelf)
            return;

        lockedSelf->onUpdate_();
    }

} } }
