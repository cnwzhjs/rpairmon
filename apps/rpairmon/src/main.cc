//
//  main.cc
//  RoboPeak Mini USB Display SDK demo
//
//  Created by Tony Huang on 12/7/13.
//  Copyright (c) 2013 RoboPeak.com. All rights reserved.
//

#include <rp/infra_config.h>
#ifdef RP_INFRA_PLATFORM_WINDOWS
#	include <io.h>
#	include <process.h>
#else
#	include <unistd.h>
#endif

#include <rp/ui/ui_manager.h>
#include <rp/ui/style_manager.h>
#include <rp/ui/color.h>
#include <tony/home_portal/ui/application.h>

using namespace std;
using namespace rp::util;
using namespace rp::ui;
using namespace rp;

#define APPLICATION_RES_ROOT "res/"
#define APPLICATION_DEFAULT_FONT "Square 721 BT"
//#define APPLICATION_DEFAULT_FONT "eurostile"
//#define APPLICATION_DEFAULT_FONT "Helvetica LT Light"

static void setupLightScheme() {
    auto style = StyleManager::defaultManager();
    
    // Font
    style->setString("Font.DefaultFontFace", APPLICATION_DEFAULT_FONT);
    style->setString("Font.FontDir", APPLICATION_RES_ROOT "fonts");
    
    // Application
    style->setColor("Application.BackgroundColor", colors::white);
    style->setColor("Application.TextColor", colors::gray);
    
    style->setInteger("Application.StatusBarHeight", 20);
    
    // Buttons
    style->setColor("LabelButton.NormalColor", colors::blue);
    style->setColor("LabelButton.ActiveColor", colors::gray);
    
    style->setColor("PaneLabelButton.NormalColor", colors::white);
    style->setColor("PaneLabelButton.ActiveColor", colors::silver);
}

static void setupDarkScheme() {
    auto style = StyleManager::defaultManager();
    
    // Font
    style->setString("Font.DefaultFontFace", APPLICATION_DEFAULT_FONT);
    style->setString("Font.FontDir", APPLICATION_RES_ROOT "fonts");
    
    // Application
    style->setColor("Application.BackgroundColor", colors::black);
    style->setColor("Application.TextColor", Color(0xdd, 0xdd, 0xff, 0xff));

    style->setString("Application.WeatherIconMap",
        "        " "        " /* 0..7   8..15*/
        "    CZ  " "d 3 aaA6" /*16..23 24..31*/
        "122     " "        " /*32..39 40..47*/);
    
    style->setInteger("Application.StatusBarHeight", 54);
    style->setFloat("Application.StatusBarFontSize", 24);
    
    // Buttons
    style->setColor("LabelButton.NormalColor", Color::fromABGR32(0x8080ffff));
    style->setColor("LabelButton.ActiveColor", colors::gray);
    
    style->setColor("PaneBackgroundButton.NormalColor", colors::gray);
    style->setColor("PaneBackgroundButton.ActiveColor", colors::silver);
}

static void setupStyle() {
    setupDarkScheme();
}

#if 0
static void updateTime(std::shared_ptr<Label> label)
{
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    label->setText(std::ctime(&nowTime));
}
#endif

int main(void) {
    setupStyle();
    
    auto manager = UiManager::defaultManager();

#if 0
    auto style = StyleManager::defaultManager();

    std::shared_ptr<Scene> scene(new Scene("Tony Home Portal"));

    int sceneWidth = style->getInteger("Application.SceneWidth", 320);
    int sceneHeight = style->getInteger("Application.SceneHeight", 240);
    auto bgColor = style->getColor("Application.BackgroundColor");
    std::string defaultFont = style->getString("Font.DefaultFontFace", "Verdana");

    scene->setSize(Size(sceneWidth, sceneHeight));
    scene->setBackgroundColor(bgColor);

    std::shared_ptr<Label> label(new Label());
    label->setFrame(ui::Rectangle(8, 0, sceneWidth - 16, 24));
    label->setFontFace(defaultFont);
    label->setFontColor(colors::white);
    label->setFontSize(9);
    label->setHorizontalAlignment(TextAlignmentNear);
    label->setVerticalAlignment(TextAlignmentCenter);
    
    scene->appendChild(label);

    manager->setScene(scene);
    
    manager->setOnUpdate(std::bind(updateTime, label));
#else
    auto application = tony::home_portal::ui::Application::defaultApplication();
    application->start();

    manager->setScene(application->getScene());
#endif

    return manager->run();
}
