#pragma once

#include <tony/home_portal/ui/drv_yahoo_weather.h>
#include <rp/ui/display_object_container.h>

namespace tony { namespace home_portal { namespace ui {

    class ConditionIndicator : public rp::ui::DisplayObjectContainer {
    public:
        ConditionIndicator();
        ~ConditionIndicator();

    public:
        LocationCondition getLocationCondition() const;
        void setLocationCondition(const LocationCondition& cond);

    public:
        virtual void onResizing(const rp::ui::Size& oldSize, const rp::ui::Size& newSize);

    private:
        LocationCondition locationCondition_;

    private:
        std::shared_ptr<rp::ui::Label> conditionLabel_;

        std::shared_ptr<rp::ui::Label> conditionImage_;
        std::shared_ptr<rp::ui::Label> tempLabel_;
        std::shared_ptr<rp::ui::Label> humidityLabel_;
    };

} } }
