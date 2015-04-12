#pragma once

#include <rp/ui/display_object_container.h>
#include <rp/ui/color.h>
#include <rp/util/noncopyable.h>

#include <string>
#include <memory>

namespace rp { namespace ui {

    class Pane;
    class Label;

} }

namespace tony { namespace home_portal { namespace ui {

    enum StatusBarStatus {
        StatusBarStatusNormal,
        StatusBarStatusWarning,
        StatusBarStatusError
    };

    class StatusBar : public rp::ui::DisplayObjectContainer, private rp::util::noncopyable {
    public:
        StatusBar();
        ~StatusBar();

    public:
        std::string getTitle();
        void setTitle(const std::string& v);

        void pushNotification(StatusBarStatus status, const std::string& text);

        void update();

    private:
        int statusBarHeight_;
        rp::ui::Color statusBarBackgroundColor_;
        std::string statusBarFontFace_;
        float statusBarFontSize_;
        rp::ui::Color statusBarFontColor_;

        std::shared_ptr<rp::ui::Pane> background_;
        std::shared_ptr<rp::ui::Label> title_;
        std::shared_ptr<rp::ui::Label> time_;
    };

} } }
