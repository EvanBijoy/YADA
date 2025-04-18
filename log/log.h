#ifndef LOG_H
#define LOG_H

#include <bits/stdc++.h>
#include "../food/food.h"
using namespace std;

struct LogEntry {
    string foodName;
    int servings;
    time_t date;
    int calories;
};

class LogManager {
public:
    void displayOptions(const FoodList& foodList);
    void addEntry(const string& foodName, int servings, int calories, time_t date = 0);
    void deleteEntry(int index);
    void undo();
    void saveToFile(const string& filename);
    void loadFromFile();
    void viewLog(time_t date = 0) const;
    int getTotalCalories(time_t date) const;
    
private:
    vector<LogEntry> entries;
    string logFile = "data/log.txt";
    vector<vector<LogEntry>> undoStack;
    void saveState();
};

#endif // LOG_H