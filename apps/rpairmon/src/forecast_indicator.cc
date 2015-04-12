#include <tony/home_portal/ui/forecast_indicator.h>
#include <rp/ui/label.h>
#include <rp/ui/color.h>
#include <rp/ui/style_manager.h>

#ifdef _WIN32
#   define snprintf sprintf_s
#endif

using namespace std;
using namespace rp::ui;

namespace tony { namespace home_portal { namespace ui {

    static const int forecastDayCount = 3;

    ForecastIndicator::ForecastIndicator()
        : presureLabel_(new Label())
        , presureValueLabel_(new Label())
        , sunLabel_(new Label())
        , sunriseLabel_(new Label())
        , sunsetLabel_(new Label())
        , windLabel_(new Label())
        , windDirectionImage_(new Label())
        , windSpeed_(new Label())
        , windUnit_(new Label())
    {
        auto style = StyleManager::defaultManager();

        std::string defaultFont = style->getString("Font.DefaultFontFace", "Verdana");

        auto meterFontFace = style->getString("Application.MeterFontFace", defaultFont);
        auto meterLabelFontSize = style->getFloat("Application.MeterLabelFontSize", 9);
        auto conditionImageFontSize = style->getFloat("Application.ConditionImageFontSize", 36);
        auto meterUnitFontSize = style->getFloat("Application.MeterUnitFontSize", 9);
        auto weatherFontFace = style->getString("Application.WeatherFontFace", "artill_clean_icons");
        weatherIconMap_ = style->getString("Application.WeatherIconMap",
            "        " "        " /* 0..7   8..15*/
            "    CZ  " "d   aaA6" /*16..23 24..31*/
            "12      " "        " /*32..39 40..47*/);

        presureLabel_->setFontFace(meterFontFace);
        presureLabel_->setFontColor(colors::silver);
        presureLabel_->setFontSize(meterLabelFontSize);
        presureLabel_->setHorizontalAlignment(TextAlignmentNear);
        presureLabel_->setVerticalAlignment(TextAlignmentCenter);
        presureLabel_->setText("Pressure");
        appendChild(presureLabel_);

        presureValueLabel_->setFontFace(meterFontFace);
        presureValueLabel_->setFontColor(colors::white);
        presureValueLabel_->setFontSize(conditionImageFontSize / 3);
        presureValueLabel_->setHorizontalAlignment(TextAlignmentNear);
        presureValueLabel_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(presureValueLabel_);

        sunLabel_->setFontFace(meterFontFace);
        sunLabel_->setFontColor(colors::silver);
        sunLabel_->setFontSize(meterLabelFontSize);
        sunLabel_->setHorizontalAlignment(TextAlignmentNear);
        sunLabel_->setVerticalAlignment(TextAlignmentCenter);
        sunLabel_->setText("Sun");
        appendChild(sunLabel_);

        sunriseLabel_->setFontFace(meterFontFace);
        sunriseLabel_->setFontColor(colors::white);
        sunriseLabel_->setFontSize(conditionImageFontSize / 3);
        sunriseLabel_->setHorizontalAlignment(TextAlignmentNear);
        sunriseLabel_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(sunriseLabel_);

        sunsetLabel_->setFontFace(meterFontFace);
        sunsetLabel_->setFontColor(colors::white);
        sunsetLabel_->setFontSize(conditionImageFontSize / 3);
        sunsetLabel_->setHorizontalAlignment(TextAlignmentNear);
        sunsetLabel_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(sunsetLabel_);

        for (int i = 0; i < forecastDayCount; i++)
        {
            std::shared_ptr<rp::ui::Label> day(new Label());
            std::shared_ptr<rp::ui::Label> conditionImage(new Label());
            std::shared_ptr<rp::ui::Label> conditionText(new Label());
            std::shared_ptr<rp::ui::Label> temp(new Label());

            day->setFontFace(meterFontFace);
            day->setFontColor(colors::silver);
            day->setFontSize(meterLabelFontSize);
            day->setHorizontalAlignment(TextAlignmentFar);
            day->setVerticalAlignment(TextAlignmentCenter);
            appendChild(day);

            conditionImage->setFontFace(weatherFontFace);
            conditionImage->setFontColor(colors::white);
            conditionImage->setFontSize(conditionImageFontSize);
            conditionImage->setHorizontalAlignment(TextAlignmentFar);
            conditionImage->setVerticalAlignment(TextAlignmentCenter);
            appendChild(conditionImage);

            conditionText->setFontFace(meterFontFace);
            conditionText->setFontColor(colors::silver);
            conditionText->setFontSize(meterLabelFontSize * 7 / 9);
            conditionText->setHorizontalAlignment(TextAlignmentFar);
            conditionText->setVerticalAlignment(TextAlignmentCenter);
            appendChild(conditionText);

            temp->setFontFace(meterFontFace);
            temp->setFontColor(colors::silver);
            temp->setFontSize(meterLabelFontSize);
            temp->setHorizontalAlignment(TextAlignmentFar);
            temp->setVerticalAlignment(TextAlignmentCenter);
            appendChild(temp);

            ForecastDayElements elements = { day, conditionImage, conditionText, temp };

            forecastDays.push_back(elements);
        }

        windLabel_->setFontFace(meterFontFace);
        windLabel_->setFontColor(colors::silver);
        windLabel_->setFontSize(meterLabelFontSize);
        windLabel_->setHorizontalAlignment(TextAlignmentCenter);
        windLabel_->setVerticalAlignment(TextAlignmentCenter);
        windLabel_->setText("Wind");
        appendChild(windLabel_);

        windDirectionImage_->setFontFace(weatherFontFace);
        windDirectionImage_->setFontColor(colors::white);
        windDirectionImage_->setFontSize(conditionImageFontSize);
        windDirectionImage_->setHorizontalAlignment(TextAlignmentCenter);
        windDirectionImage_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(windDirectionImage_);

        windSpeed_->setFontFace(meterFontFace);
        windSpeed_->setFontColor(colors::silver);
        windSpeed_->setFontSize(meterLabelFontSize);
        windSpeed_->setHorizontalAlignment(TextAlignmentCenter);
        windSpeed_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(windSpeed_);

        windUnit_->setFontFace(meterFontFace);
        windUnit_->setFontColor(colors::silver);
        windUnit_->setFontSize(meterLabelFontSize);
        windUnit_->setHorizontalAlignment(TextAlignmentCenter);
        windUnit_->setVerticalAlignment(TextAlignmentCenter);
        windUnit_->setText("km/h");
        appendChild(windUnit_);
    }

    ForecastIndicator::~ForecastIndicator()
    {}

    LocationCondition ForecastIndicator::getLocationCondition() const
    {
        return locationCondition_;
    }

    void ForecastIndicator::setLocationCondition(const LocationCondition& cond)
    {
        locationCondition_ = cond;

        sunriseLabel_->setText(cond.sunrise);
        sunsetLabel_->setText(cond.sunset);

        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%.1f", cond.pressure);
        presureValueLabel_->setText(buffer);

        snprintf(buffer, sizeof(buffer), "%.1f", cond.windSpeed);
        windSpeed_->setText(buffer);

        int dir = cond.windDirection;

        if (dir > (360 - 22.5) || dir < 22.5)
        {
            windDirectionImage_->setText("¿");
        }
        else if (dir < (45 + 22.5))
        {
            windDirectionImage_->setText("\"");
        }
        else if (dir < (90 + 22.5))
        {
            windDirectionImage_->setText("'");
        }
        else if (dir < (135 + 22.5))
        {
            windDirectionImage_->setText(";");
        }
        else if (dir < (180 + 22.5))
        {
            windDirectionImage_->setText("#");
        }
        else if (dir < (225 + 22.5))
        {
            windDirectionImage_->setText(".");
        }
        else if (dir < (270 + 22.5))
        {
            windDirectionImage_->setText("·");
        }
        else
        {
            windDirectionImage_->setText("?");
        }
    }

    std::vector<Forecast> ForecastIndicator::getForecast() const
    {
        return forecast_;
    }

    static std::string replace(const std::string& s, char f, char r)
    {
        std::string output(s);

        for (size_t i = 0; i < output.size(); i++)
            if (output[i] == f)
                output[i] = r;

        return output;
    }

    static std::string summary(const std::string& s, size_t maxSize)
    {
        if (s.size() <= maxSize)
            return s;

        return s.substr(0, maxSize - 2) + "..";
    }

    void ForecastIndicator::setForecast(const std::vector<Forecast>& v)
    {
        forecast_ = v;

        for (size_t i = 0; i < forecastDays.size(); i++)
        {
            auto& elems = forecastDays[i];

            if (i >= v.size())
            {
                elems.day->setText("");
                elems.conditionImage->setText("");
                elems.conditionText->setText("");
                elems.temp->setText("");
            }
            else
            {
                elems.day->setText(v[i].day);
                elems.conditionImage->setText(weatherIconMap_.substr(v[i].code, 1));
                elems.conditionText->setText(summary(v[i].codeText, 10));
                char buffer[256];
                snprintf(buffer, sizeof(buffer), "%.0f~%.0f", v[i].low, v[i].high);
                elems.temp->setText(buffer);
            }
        }
    }

    void ForecastIndicator::onResizing(const rp::ui::Size& oldSize, const rp::ui::Size& newSize)
    {
        const int parts = (int)forecastDays.size() + 2;
        static const int forecastGap = 8;
        int forecastDayWidth = (int)((newSize.width - forecastGap * parts) / (parts + 0.5));

        presureLabel_->setFrame(rp::ui::Rectangle(forecastGap, 0, forecastDayWidth * 3 / 2, 20));
        presureValueLabel_->setFrame(rp::ui::Rectangle(presureLabel_->getX(), 20, presureLabel_->getWidth(), (newSize.height - 40) / 3));

        sunLabel_->setFrame(rp::ui::Rectangle(forecastGap, presureValueLabel_->getY() + presureValueLabel_->getHeight(), presureLabel_->getWidth(), 20));
        sunriseLabel_->setFrame(rp::ui::Rectangle(sunLabel_->getX(), sunLabel_->getY() + sunLabel_->getHeight(), sunLabel_->getWidth(), (newSize.height - 40) / 3));
        sunsetLabel_->setFrame(rp::ui::Rectangle(sunLabel_->getX(), sunriseLabel_->getY() + sunriseLabel_->getHeight(), sunriseLabel_->getWidth(), sunriseLabel_->getHeight()));

        windLabel_->setFrame(rp::ui::Rectangle(
            sunLabel_->getX() + sunLabel_->getWidth() + forecastGap, 0,
            forecastDayWidth, 20));

        windDirectionImage_->setFrame(rp::ui::Rectangle(
            windLabel_->getX(), 20, windLabel_->getWidth(), newSize.height - 60));

        windSpeed_->setFrame(rp::ui::Rectangle(
            windLabel_->getX(), newSize.height - 40, windLabel_->getWidth(), 20));

        windUnit_->setFrame(rp::ui::Rectangle(
            windLabel_->getX(), newSize.height - 20, windLabel_->getWidth(), 20));

        int forecastStartX = windLabel_->getX() + windLabel_->getWidth();

        for (size_t i = 0; i < forecastDays.size(); i++)
        {
            auto& elems = forecastDays[i];

            elems.day->setFrame(rp::ui::Rectangle(
                forecastStartX + (int)i * (forecastDayWidth + forecastGap), 0,
                forecastDayWidth, 20));

            elems.conditionImage->setFrame(rp::ui::Rectangle(
                elems.day->getX(), 20, elems.day->getWidth(), newSize.height - 60));

            elems.conditionText->setFrame(rp::ui::Rectangle(
                elems.day->getX(), newSize.height - 40, elems.day->getWidth(), 20));

            elems.temp->setFrame(rp::ui::Rectangle(
                elems.day->getX(), newSize.height - 20, elems.day->getWidth(), 20));
        }

        DisplayObjectContainer::onResizing(oldSize, newSize);
    }

} } }
