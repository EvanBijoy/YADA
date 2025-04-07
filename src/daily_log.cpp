#include "daily_log.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <sstream>

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
        std::ifstream logsFile("data/daily_logs.txt");
        if (logsFile.is_open()) {
            std::string line;
            std::string currentDate;
            
            while (std::getline(logsFile, line)) {
                if (line.empty()) continue;
                
                // Check if line is a date header (format: [YYYY-MM-DD])
                if (line.front() == '[' && line.back() == ']') {
                    currentDate = line.substr(1, line.size() - 2);
                    validateDateFormat(currentDate);
                } 
                else {
                    // Parse food entry (format: foodId,servings)
                    std::istringstream iss(line);
                    std::string foodId;
                    std::string servingsStr;
                    
                    if (std::getline(iss, foodId, ',') && std::getline(iss, servingsStr)) {
                        try {
                            int servings = std::stoi(servingsStr);
                            m_dailyLogs[currentDate].emplace_back(LogEntry{foodId, servings});
                        } 
                        catch (const std::exception& e) {
                            std::cerr << "Error parsing servings for food " << foodId 
                                      << ": " << e.what() << std::endl;
                        }
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading daily logs: " << e.what() << std::endl;
    }
}

void DailyLog::saveLogs() {
    std::ofstream logsFile("data/daily_logs.txt");
    
    for (const auto& [date, entries] : m_dailyLogs) {
        // Write date header
        logsFile << "[" << date << "]\n";
        
        // Write all entries for this date
        for (const auto& entry : entries) {
            logsFile << entry.foodId << "," << entry.servings << "\n";
        }
        
        // Add empty line between dates for readability
        logsFile << "\n";
    }
}

void DailyLog::addFoodToLog(const std::string& date, const std::string& foodId, int servings) {
    validateDateFormat(date);
    m_dailyLogs[date].emplace_back(LogEntry{foodId, servings});
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