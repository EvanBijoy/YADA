#include <iostream>
#include "food/food.h"
#include "log/log.h"
#include "profile/profile.h"
#include <filesystem>

using namespace std;

int main() {
    FoodList foodList;
    LogManager logManager;
    DietProfile profile;
    
    // Create data directory if it doesn't exist
    filesystem::create_directory("data");
    
    // Load data
    foodList.loadFromFiles();
    logManager.loadFromFile();
    profile.loadFromFile();
    
    while (true) {
        cout << "\nMain Menu:\n";
        cout << "1. Food Database\n";
        cout << "2. Food Log\n";
        cout << "3. Diet Profile\n";
        cout << "4. View Daily Summary\n";
        cout << "5. Exit\n";
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
            case 1:
                foodList.displayOptions();
                break;
            case 2:
                logManager.displayOptions(foodList);
                break;
            case 3:
                profile.displayOptions();
                break;
            case 4: {
                time_t today = time(nullptr);
                int totalCalories = logManager.getTotalCalories(today);
                int targetCalories = profile.calculateTargetCalories();
                int difference = totalCalories - targetCalories;
                
                char buffer[80];
                strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&today));
                
                cout << "\nDaily Summary for " << buffer << ":\n";
                cout << "Calories consumed: " << totalCalories << "\n";
                cout << "Target calories: " << targetCalories << "\n";
                cout << "Difference: " << difference << " (" 
                     << (difference > 0 ? "over" : "under") << " target)\n";
                break;
            }
            case 5:
                // Save data
                foodList.saveToFiles();
                logManager.saveToFile("data/log.txt");
                cout << "Data saved. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
    
    return 0;
}