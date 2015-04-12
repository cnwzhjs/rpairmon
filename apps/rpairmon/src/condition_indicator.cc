#include <tony/home_portal/ui/condition_indicator.h>
#include <rp/ui/label.h>
#include <rp/ui/color.h>
#include <rp/ui/style_manager.h>

#ifdef _WIN32
#   define snprintf sprintf_s
#endif

using namespace std;
using namespace rp::ui;

namespace tony { namespace home_portal { namespace ui {

    ConditionIndicator::ConditionIndicator()
        : conditionLabel_(new Label())
        , conditionImage_(new Label())
        , tempLabel_(new Label())
        , humidityLabel_(new Label())
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

        conditionLabel_->setFontFace(meterFontFace);
        conditionLabel_->setFontColor(colors::silver);
        conditionLabel_->setFontSize(meterLabelFontSize);
        conditionLabel_->setHorizontalAlignment(TextAlignmentFar);
        conditionLabel_->setVerticalAlignment(TextAlignmentCenter);
        conditionLabel_->setText("Outdoor");
        appendChild(conditionLabel_);

        conditionImage_->setFontFace(weatherFontFace);
        conditionImage_->setFontColor(colors::white);
        conditionImage_->setFontSize(conditionImageFontSize);
        conditionImage_->setHorizontalAlignment(TextAlignmentCenter);
        conditionImage_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(conditionImage_);

        tempLabel_->setFontFace(meterFontFace);
        tempLabel_->setFontColor(colors::white);
        tempLabel_->setFontSize(conditionImageFontSize / 3);
        tempLabel_->setHorizontalAlignment(TextAlignmentFar);
        tempLabel_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(tempLabel_);

        humidityLabel_->setFontFace(meterFontFace);
        humidityLabel_->setFontColor(colors::white);
        humidityLabel_->setFontSize(conditionImageFontSize / 3);
        humidityLabel_->setHorizontalAlignment(TextAlignmentFar);
        humidityLabel_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(humidityLabel_);
    }

    ConditionIndicator::~ConditionIndicator()
    {}

    LocationCondition ConditionIndicator::getLocationCondition() const
    {
        return locationCondition_;
    }

    void ConditionIndicator::setLocationCondition(const LocationCondition& cond)
    {
        locationCondition_ = cond;

        conditionImage_->setText(weatherIconMap_.substr(cond.code, 1));

        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%.0f", cond.temperature);
        tempLabel_->setText(buffer);

        snprintf(buffer, sizeof(buffer), "%.0f%%", cond.humidity);
        humidityLabel_->setText(buffer);
    }

    void ConditionIndicator::onResizing(const rp::ui::Size& oldSize, const rp::ui::Size& newSize)
    {
        conditionLabel_->setFrame(rp::ui::Rectangle(0, 0, newSize.width - 8, 20));

        conditionImage_->setFrame(rp::ui::Rectangle(8, 20, newSize.height - conditionLabel_->getHeight(), newSize.height - conditionLabel_->getHeight()));

        tempLabel_->setFrame(rp::ui::Rectangle(conditionImage_->getX() + conditionImage_->getWidth(), conditionImage_->getY(), newSize.width - conditionImage_->getWidth() - conditionImage_->getX() - 8, conditionImage_->getHeight() / 2));
        humidityLabel_->setFrame(rp::ui::Rectangle(tempLabel_->getX(), tempLabel_->getY() + tempLabel_->getHeight(), tempLabel_->getWidth(), tempLabel_->getHeight()));

        DisplayObjectContainer::onResizing(oldSize, newSize);
    }

} } }
