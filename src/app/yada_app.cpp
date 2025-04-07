#pragma once

#include "core/command_manager.h"
#include "core/food_database.h"
#include "core/diet_profile.h"
#include "core/daily_log.h"
#include "menus/main_menu.h"

class YADAApp {
public:
    YADAApp();
    ~YADAApp();
    void run();

private:
    FoodDatabase m_database;
    DietProfile m_profile;
    DailyLog m_dailyLog;
    CommandManager m_commandManager;
    std::string m_currentDate;
    bool m_running;
    MainMenu m_mainMenu;
};