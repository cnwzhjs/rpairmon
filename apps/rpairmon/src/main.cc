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
    style->setString("Application.WeatherFontFace", "weathericons-regular-webfont");
    
    // Application
    style->setColor("Application.BackgroundColor", colors::black);
    style->setColor("Application.TextColor", Color(0xdd, 0xdd, 0xff, 0xff));
    
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

int main(void) {
    setupStyle();
    
    auto manager = UiManager::defaultManager();

    auto application = tony::home_portal::ui::Application::defaultApplication();
    application->start();

    manager->setScene(application->getScene());

    return manager->run();
}
