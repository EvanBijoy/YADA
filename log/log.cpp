#include "log.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>

void LogManager::displayOptions(const FoodList& foodList) {
    while (true) {
        cout << "\nFood Log Options:\n";
        cout << "1. Add food to log\n";
        cout << "2. View today's log\n";
        cout << "3. View log for specific date\n";
        cout << "4. Delete entry\n";
        cout << "5. Undo\n";
        cout << "6. Save Log\n";
        cout << "7. Return to main menu\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        
        switch (choice) {
            case 1: {
                const vector<Food>& foods = foodList.getFoods();
                if (foods.empty()) {
                    cout << "No foods available. Please add foods first.\n";
                    break;
                }
                
                cout << "\nAvailable foods:\n";
                for (size_t i = 0; i < foods.size(); ++i) {
                    cout << i+1 << ". " << foods[i].name << " (" << foods[i].calories << " cal)\n";
                }
                
                cout << "Select food (number): ";
                int foodChoice;
                cin >> foodChoice;
                
                cout << "Enter number of servings: ";
                int servings;
                cin >> servings;
                cin.ignore();
                
                if (foodChoice > 0 && foodChoice <= static_cast<int>(foods.size())) {
                    addEntry(foods[foodChoice-1].name, servings, foods[foodChoice-1].calories);
                    cout << "Entry added.\n";
                }
                else {
                    cout << "Invalid selection.\n";
                }
                break;
            }
            case 2:
                viewLog();
                break;
            case 3: {
                cout << "Enter date (YYYY-MM-DD): ";
                string dateStr;
                cin >> dateStr;
            
                tm tm = {};
                istringstream iss(dateStr);
                iss >> get_time(&tm, "%Y-%m-%d");
            
                if (iss.fail() || !iss.eof()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid date format. Please use YYYY-MM-DD.\n";
                    continue;
                }
            
                time_t date = mktime(&tm);
                if (date == -1) {
                    cout << "Invalid date.\n";
                    continue;
                }
            
                viewLog(date);
                break;
            }
            case 4: {
                viewLog();
                if (!entries.empty()) {
                    cout << "Enter entry number to delete: ";
                    int index;
                    cin >> index;
                    cin.ignore();
                    
                    if (index > 0 && index <= static_cast<int>(entries.size())) {
                        deleteEntry(index - 1);
                        cout << "Entry deleted.\n";
                    } else {
                        cout << "Invalid entry number.\n";
                    }
                }
                break;
            }
            case 5:
                undo();
                break;
            case 6:
                saveToFile("data/log.txt");
                cout << "Log saved successfully.\n";
                break;
            case 7:
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}

void LogManager::addEntry(const string& foodName, int servings, int calories, time_t date) {
    saveState();
    
    LogEntry entry;
    entry.foodName = foodName;
    entry.servings = servings;
    entry.calories = calories * servings;
    entry.date = (date == 0) ? time(nullptr) : date;
    
    entries.push_back(entry);
}

void LogManager::deleteEntry(int index) {
    if (index >= 0 && index < static_cast<int>(entries.size())) {
        saveState();
        entries.erase(entries.begin() + index);
    }
}

void LogManager::undo() {
    if (!undoStack.empty()) {
        entries = undoStack.back();
        undoStack.pop_back();
        cout << "Undo successful.\n";
    } 
    else {
        cout << "Nothing to undo.\n";
    }
}

void LogManager::saveState() {
    undoStack.push_back(entries);
    // Keep undo stack manageable
    if (undoStack.size() > 50) {
        undoStack.erase(undoStack.begin());
    }
}

void LogManager::viewLog(time_t date) const {
    if (date == 0) {
        date = time(nullptr);
    }

    tm* searchTime = localtime(&date);
    searchTime->tm_hour = 0;
    searchTime->tm_min = 0;
    searchTime->tm_sec = 0;
    time_t normalizedDate = mktime(searchTime);

    cout << "\nFood Log:\n";
    bool found = false;
    int totalCalories = 0;

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&normalizedDate));
    cout << "Date: " << buffer << "\n\n";

    for (size_t i = 0; i < entries.size(); ++i) {
        tm* entryTime = localtime(&entries[i].date);
        entryTime->tm_hour = 0;
        entryTime->tm_min = 0;
        entryTime->tm_sec = 0;
        time_t normalizedEntryDate = mktime(entryTime);

        if (normalizedEntryDate == normalizedDate) {
            cout << i + 1 << ". " << entries[i].foodName
                 << " - Servings: " << entries[i].servings
                 << ", Calories: " << entries[i].calories << "\n";
            totalCalories += entries[i].calories;
            found = true;
        }
    }

    if (!found) {
        cout << "No entries for this date.\n";
    }
    else {
        cout << "\nTotal calories: " << totalCalories << "\n";
    }
}

int LogManager::getTotalCalories(time_t date) const {
    int total = 0;
    for (const auto& entry : entries) {
        tm* entryTime = localtime(&entry.date);
        tm* searchTime = localtime(&date);
        
        if (entryTime->tm_year == searchTime->tm_year &&
            entryTime->tm_mon == searchTime->tm_mon &&
            entryTime->tm_mday == searchTime->tm_mday) {
            total += entry.calories;
        }
    }
    return total;
}

void LogManager::saveToFile(const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }
    
    for (const auto& entry : entries) {
        outFile << entry.foodName << "|" << entry.servings << "|" 
               << entry.calories << "|" << entry.date << "\n";
    }
    
    outFile.close();
}

void LogManager::loadFromFile() {
    string filename = logFile;
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "File not found, starting with empty log: " << filename << endl;
        return;
    }
    
    entries.clear();
    string line;
    while (getline(inFile, line)) {
        istringstream iss(line);
        string token;
        vector<string> tokens;
        
        while (getline(iss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() == 4) {
            LogEntry entry;
            entry.foodName = tokens[0];
            entry.servings = stoi(tokens[1]);
            entry.calories = stoi(tokens[2]);
            entry.date = stol(tokens[3]);
            entries.push_back(entry);
        }
    }
    
    inFile.close();
}