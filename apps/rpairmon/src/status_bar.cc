#include <tony/home_portal/ui/status_bar.h>
#include <rp/ui/pane.h>
#include <rp/ui/label.h>
#include <rp/ui/style_manager.h>
#include <time.h>
#include <chrono>

#ifdef WIN32
#   include <windows.h>
#   define snprintf sprintf_s
    static int gettimeofday(struct timeval *tp, void *tzp)
    {
        time_t clock;
        struct tm tm;
        SYSTEMTIME wtm;
        GetLocalTime(&wtm);
        tm.tm_year = wtm.wYear - 1900;
        tm.tm_mon = wtm.wMonth - 1;
        tm.tm_mday = wtm.wDay;
        tm.tm_hour = wtm.wHour;
        tm.tm_min = wtm.wMinute;
        tm.tm_sec = wtm.wSecond;
        tm.tm_isdst = -1;
        clock = mktime(&tm);
        tp->tv_sec = clock;
        tp->tv_usec = wtm.wMilliseconds * 1000;
        return (0);
    }
#else
#   include <sys/time.h>
#endif

using namespace std;
using namespace rp::ui;

namespace tony { namespace home_portal { namespace ui {

    StatusBar::StatusBar()
        : background_(new Pane())
        , title_(new Label())
        , time_(new Label())
    {
        auto style = StyleManager::defaultManager();
        
        std::string defaultFont = style->getString("Font.DefaultFontFace", "Verdana");

        statusBarHeight_ = style->getInteger("Application.StatusBarHeight", 24);
        statusBarBackgroundColor_ = style->getColor("Application.StatusBarBackgroundColor");
        statusBarFontFace_ = style->getString("Application.StatusBarFontFace", defaultFont);
        statusBarFontSize_ = style->getFloat("Application.StatusBarFontSize", 9);
        statusBarFontColor_ = style->getColor("Application.StatusBarFontColor", colors::white);

        int sceneWidth = style->getInteger("Application.SceneWidth", 320);

        rp::ui::Rectangle frame(0, 0, sceneWidth, statusBarHeight_);
        setFrame(frame);

        background_->setFrame(frame);
        background_->setBackgroundColor(statusBarBackgroundColor_);
        appendChild(background_);

        title_->setFrame(rp::ui::Rectangle(8, 0, sceneWidth - 16 - 120, statusBarHeight_));
        title_->setFontFace(statusBarFontFace_);
        title_->setFontColor(statusBarFontColor_);
        title_->setFontSize(statusBarFontSize_);
        title_->setHorizontalAlignment(TextAlignmentNear);
        title_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(title_);

        time_->setFrame(rp::ui::Rectangle(sceneWidth - 8 - 120, 0, 120, statusBarHeight_));
        time_->setFontFace(statusBarFontFace_);
        time_->setFontColor(statusBarFontColor_);
        time_->setFontSize(statusBarFontSize_);
        time_->setHorizontalAlignment(TextAlignmentFar);
        time_->setVerticalAlignment(TextAlignmentCenter);
        appendChild(time_);
    }

    StatusBar::~StatusBar()
    {
    }

    std::string StatusBar::getTitle()
    {
        return title_->getText();
    }

    void StatusBar::setTitle(const std::string& text)
    {
        title_->setText(text);
    }

    void StatusBar::pushNotification(StatusBarStatus status, const std::string& text)
    {
        // TODO
    }

    static const char* daysOfWeek[] = {
        "SUN",
        "MON",
        "TUE",
        "WED",
        "THUR",
        "FRI",
        "SAT"
    };

    void StatusBar::update()
    {
        auto now = std::chrono::system_clock::now();
        time_t t = std::chrono::system_clock::to_time_t(now);

        tm* localTime = localtime(&t);

        char buffer[256];

        //snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", (int)localTime->tm_hour, (int)localTime->tm_min, (int)localTime->tm_sec);
        snprintf(buffer, sizeof(buffer), "%d:%d", (int)localTime->tm_hour, (int)localTime->tm_min);
        time_->setText(buffer);

        //snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %s", (int)localTime->tm_year + 1900, (int)localTime->tm_mon + 1, (int)localTime->tm_mday, daysOfWeek[localTime->tm_wday]);
        snprintf(buffer, sizeof(buffer), "%d-%02d %s", (int)localTime->tm_mon + 1, (int)localTime->tm_mday, daysOfWeek[localTime->tm_wday]);
        title_->setText(buffer);
    }

} } }
