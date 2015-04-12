#pragma once

#include <tony/home_portal/ui/drv_yahoo_weather.h>
#include <rp/ui/display_object_container.h>

namespace rp { namespace ui {

    class Label;
    class Size;

} }

namespace tony { namespace home_portal { namespace ui {

    struct ForecastDayElements {
        std::shared_ptr<rp::ui::Label> day;
        std::shared_ptr<rp::ui::Label> conditionImage;
        std::shared_ptr<rp::ui::Label> conditionText;
        std::shared_ptr<rp::ui::Label> temp;
    };

    class ForecastIndicator : public rp::ui::DisplayObjectContainer {
    public:
        ForecastIndicator();
        ~ForecastIndicator();

    public:
        LocationCondition getLocationCondition() const;
        void setLocationCondition(const LocationCondition& cond);

        std::vector<Forecast> getForecast() const;
        void setForecast(const std::vector<Forecast>& forecast);
        
    public:
        virtual void onResizing(const rp::ui::Size& oldSize, const rp::ui::Size& newSize);

    private:
        LocationCondition locationCondition_;
        std::vector<Forecast> forecast_;

    private:
        std::shared_ptr<rp::ui::Label> presureLabel_;
        std::shared_ptr<rp::ui::Label> presureValueLabel_;

        std::shared_ptr<rp::ui::Label> sunLabel_;
        std::shared_ptr<rp::ui::Label> sunriseLabel_;
        std::shared_ptr<rp::ui::Label> sunsetLabel_;

        std::vector<ForecastDayElements> forecastDays;

        std::shared_ptr<rp::ui::Label> windLabel_;
        std::shared_ptr<rp::ui::Label> windDirectionImage_;
        std::shared_ptr<rp::ui::Label> windSpeed_;
        std::shared_ptr<rp::ui::Label> windUnit_;

    private:
        std::string weatherIconMap_;
    };

} } }
