#ifndef DAILY_LOG_H
#define DAILY_LOG_H

#include <unordered_map>
#include <map>
#include <string>
#include <optional>
#include "../json.hpp"
#include "food.h"
#include "food_database.h"

struct LogEntry {
    std::string foodId;
    int servings;
    
    nlohmann::json toJson() const {
        return {
            {"foodId", foodId},
            {"servings", servings}
        };
    }

    static LogEntry fromJson(const nlohmann::json& j) {
        return {
            j.at("foodId").get<std::string>(),
            j.at("servings").get<int>()
        };
    }
};

class DailyLog {
public:
    DailyLog();

    void loadLogs();
    void saveLogs();

    void addFoodToLog(const std::string& date, const std::string& foodId, int servings);
    void removeFoodFromLog(const std::string& date, const std::string& foodId);
    
    std::vector<LogEntry> getFoodLogForDate(const std::string& date) const;
    double calculateTotalCaloriesForDate(const std::string& date, const FoodDatabase& foodDatabase) const;

    std::vector<std::string> getAvailableDates() const;

private:
    std::map<std::string, std::vector<LogEntry>> m_dailyLogs;

    void validateDateFormat(const std::string& date) const;
};

#endif // DAILY_LOG_H