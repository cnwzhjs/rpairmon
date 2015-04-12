#pragma once

#include <rp/ui/display_object_container.h>
#include <rp/ui/label.h>

namespace tony { namespace home_portal { namespace ui {

    class Meter : public rp::ui::DisplayObjectContainer {
    public:
        Meter();
        ~Meter();

    public:
        std::string getLabel();
        void setLabel(const std::string&);

        std::string getValue();
        void setValue(const std::string&);

        std::string getUnit();
        void setUnit(const std::string&);

        rp::ui::TextAlignment getHorizontalAlignment() const;
        void setHorizontalAlignment(rp::ui::TextAlignment alignment);

    public:
        std::string getLabelFontFace() const;
        void setLabelFontFace(const std::string& v);

        float getLabelFontSize() const;
        void setLabelFontSize(float v);

        std::string getUnitFontFace() const;
        void setUnitFontFace(const std::string& v);

        float getUnitFontSize() const;
        void setUnitFontSize(float v);

    public:
        virtual void onResizing(const rp::ui::Size& oldSize, const rp::ui::Size& newSize);

    private:
        std::string meterFontFace_;
        float meterLabelFontSize_;
        float meterValueFontSize_;
        float meterUnitFontSize_;

    private:
        std::shared_ptr<rp::ui::Label> label_;
        std::shared_ptr<rp::ui::Label> value_;
        std::shared_ptr<rp::ui::Label> unit_;
    };

} } }
