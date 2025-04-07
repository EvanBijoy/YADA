#include "daily_log.h"
#include <fstream>
#include <regex>
#include <iostream>

DailyLog::DailyLog() {
    loadLogs();
}

void DailyLog::validateDateFormat(const std::string& date) const {
    std::regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
    if (!std::regex_match(date, dateRegex)) {
        throw std::invalid_argument("Invalid date format. Use YYYY-MM-DD");
    }
}

void DailyLog::loadLogs() {
    try {
        std::ifstream logsFile("data/daily_logs.json");
        if (logsFile.is_open()) {
            nlohmann::json logsJson;
            logsFile >> logsJson;

            for (const auto& [date, entries] : logsJson.items()) {
                std::vector<LogEntry> dateEntries;
                for (const auto& entryJson : entries) {
                    dateEntries.push_back(LogEntry::fromJson(entryJson));
                }
                m_dailyLogs[date] = dateEntries;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading daily logs: " << e.what() << std::endl;
    }
}

void DailyLog::saveLogs() {
    nlohmann::json logsJson;

    for (const auto& [date, entries] : m_dailyLogs) {
        nlohmann::json dateEntries = nlohmann::json::array();
        for (const auto& entry : entries) {
            dateEntries.push_back(entry.toJson());
        }
        logsJson[date] = dateEntries;
    }

    std::ofstream logsFile("data/daily_logs.json");
    logsFile << logsJson.dump(4);
}

void DailyLog::addFoodToLog(const std::string& date, const std::string& foodId, int servings) {
    validateDateFormat(date);

    LogEntry newEntry{foodId, servings};
    m_dailyLogs[date].push_back(newEntry);
}

void DailyLog::removeFoodFromLog(const std::string& date, const std::string& foodId) {
    validateDateFormat(date);

    auto& dateEntries = m_dailyLogs[date];
    dateEntries.erase(
        std::remove_if(dateEntries.begin(), dateEntries.end(), 
            [&foodId](const LogEntry& entry) { return entry.foodId == foodId; }),
        dateEntries.end()
    );
}

std::vector<LogEntry> DailyLog::getFoodLogForDate(const std::string& date) const {
    validateDateFormat(date);

    auto it = m_dailyLogs.find(date);
    return it != m_dailyLogs.end() ? it->second : std::vector<LogEntry>{};
}

double DailyLog::calculateTotalCaloriesForDate(
    const std::string& date, 
    const FoodDatabase& foodDatabase
) const {
    validateDateFormat(date);

    double totalCalories = 0.0;
    auto entries = getFoodLogForDate(date);

    for (const auto& entry : entries) {
        auto food = foodDatabase.findFood(entry.foodId);
        if (food) {
            totalCalories += food->getCaloriesPerServing() * entry.servings;
        }
    }

    return totalCalories;
}

std::vector<std::string> DailyLog::getAvailableDates() const {
    std::vector<std::string> dates;
    for (const auto& [date, _] : m_dailyLogs) {
        dates.push_back(date);
    }
    return dates;
}