#pragma once

#include <tony/home_portal/ui/meter.h>
#include <rp/ui/style_manager.h>
#include <rp/ui/color.h>

using namespace std;
using namespace rp::ui;

namespace tony { namespace home_portal { namespace ui {

    Meter::Meter()
        : label_(new Label())
        , value_(new Label())
        , unit_(new Label())
    {
        auto style = StyleManager::defaultManager();

        std::string defaultFont = style->getString("Font.DefaultFontFace", "Verdana");

        meterFontFace_ = style->getString("Application.MeterFontFace", defaultFont);
        meterLabelFontSize_ = style->getFloat("Application.MeterLabelFontSize", 12);
        meterValueFontSize_ = style->getFloat("Application.MeterValueFontSize", 24);
        meterUnitFontSize_ = style->getFloat("Application.MeterUnitFontSize", 12);

        label_->setFontFace(meterFontFace_);
        label_->setFontColor(colors::silver);
        label_->setFontSize(meterLabelFontSize_);
        label_->setHorizontalAlignment(TextAlignmentNear);
        label_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(label_);

        value_->setFontFace(meterFontFace_);
        value_->setFontColor(colors::white);
        value_->setFontSize(meterValueFontSize_);
        value_->setHorizontalAlignment(TextAlignmentCenter);
        value_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(value_);

        unit_->setFontFace(meterFontFace_);
        unit_->setFontColor(colors::silver);
        unit_->setFontSize(meterUnitFontSize_);
        unit_->setHorizontalAlignment(TextAlignmentFar);
        unit_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(unit_);
    }

    Meter::~Meter()
    {}

    std::string Meter::getLabel()
    {
        return label_->getText();
    }

    void Meter::setLabel(const std::string& v)
    {
        label_->setText(v);
    }

    std::string Meter::getValue()
    {
        return value_->getText();
    }

    void Meter::setValue(const std::string& v)
    {
        value_->setText(v);
    }

    std::string Meter::getUnit()
    {
        return unit_->getText();
    }

    void Meter::setUnit(const std::string& v)
    {
        unit_->setText(v);
    }

    TextAlignment Meter::getHorizontalAlignment() const
    {
        return label_->getHorizontalAlignment();
    }

    void Meter::setHorizontalAlignment(TextAlignment alignment)
    {
        //label_->setHorizontalAlignment(alignment);
        value_->setHorizontalAlignment(alignment);
        //unit_->setHorizontalAlignment(alignment);
    }

    std::string Meter::getLabelFontFace() const
    {
        return label_->getFontFace();
    }

    void Meter::setLabelFontFace(const std::string& v)
    {
        label_->setFontFace(v);
    }

    float Meter::getLabelFontSize() const
    {
        return label_->getFontSize();
    }

    void Meter::setLabelFontSize(float v)
    {
        label_->setFontSize(v);
    }

    std::string Meter::getUnitFontFace() const
    {
        return unit_->getFontFace();
    }

    void Meter::setUnitFontFace(const std::string& v)
    {
        unit_->setFontFace(v);
    }

    float Meter::getUnitFontSize() const
    {
        return unit_->getFontSize();
    }

    void Meter::setUnitFontSize(float v)
    {
        unit_->setFontSize(v);
    }

    void Meter::onResizing(const Size& oldSize, const Size& newSize)
    {
        label_->setFrame(rp::ui::Rectangle(8, 0, newSize.width / 2 - 8, 20));
        unit_->setFrame(rp::ui::Rectangle(newSize.width / 2, 0, newSize.width  / 2 - 8, 20));
        value_->setFrame(rp::ui::Rectangle(8, 20, newSize.width - 16, newSize.height - 20));

        DisplayObjectContainer::onResizing(oldSize, newSize);
    }

} } }
