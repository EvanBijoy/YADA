#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cctype>

#include "command_manager.h"
#include "food_database.h"
#include "diet_profile.h"
#include "daily_log.h"

// Forward declarations for commands
class AddFoodToLogCommand;
class RemoveFoodFromLogCommand;
class AddFoodToDatabaseCommand;
class UpdateProfileCommand;

// Utility functions
std::string getCurrentDate();
std::string toLower(const std::string& str);
std::vector<std::string> splitString(const std::string& str, char delimiter);
void clearScreen();
void waitForEnter();

class YADAApp {
public:
    YADAApp() 
        : m_currentDate(getCurrentDate()), m_running(true) {
        m_database.loadDatabase();
        m_profile.loadProfile();
        m_dailyLog.loadLogs();
    }

    ~YADAApp() {
        // Save data before exiting
        m_database.saveDatabase();
        m_profile.saveProfile();
        m_dailyLog.saveLogs();
    }

    void run() {
        while (m_running) {
            displayMainMenu();
            int choice = getMenuChoice(1, 8);
            
            switch (choice) {
                case 1:
                    manageFood();
                    break;
                case 2:
                    manageDailyLog();
                    break;
                case 3:
                    manageDietProfile();
                    break;
                case 4:
                    viewDailyCalorieReport();
                    break;
                case 5:
                    saveAllData();
                    break;
                case 6:
                    if (m_commandManager.canUndo()) {
                        m_commandManager.undoLastCommand();
                        std::cout << "Last command undone." << std::endl;
                    } else {
                        std::cout << "Nothing to undo." << std::endl;
                    }
                    waitForEnter();
                    break;
                case 7:
                    about();
                    break;
                case 8:
                    m_running = false;
                    break;
                default:
                    std::cout << "Invalid choice." << std::endl;
                    waitForEnter();
            }
        }
    }

private:
    FoodDatabase m_database;
    DietProfile m_profile;
    DailyLog m_dailyLog;
    CommandManager m_commandManager;
    std::string m_currentDate;
    bool m_running;

    // Display functions
    void displayMainMenu() const {
        clearScreen();
        std::cout << "=========================================" << std::endl;
        std::cout << "           YADA - Diet Manager           " << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "Current Date: " << m_currentDate << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "1. Manage Foods" << std::endl;
        std::cout << "2. Manage Daily Log" << std::endl;
        std::cout << "3. Manage Diet Profile" << std::endl;
        std::cout << "4. View Daily Calorie Report" << std::endl;
        std::cout << "5. Save All Data" << std::endl;
        std::cout << "6. Undo Last Command" << std::endl;
        std::cout << "7. About" << std::endl;
        std::cout << "8. Exit" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Select an option (1-8): ";
    }

    // Menu functions
    void manageFood() {
        bool backToMainMenu = false;
        
        while (!backToMainMenu) {
            clearScreen();
            std::cout << "========== Food Management ==========" << std::endl;
            std::cout << "1. Add Basic Food" << std::endl;
            std::cout << "2. Create Composite Food" << std::endl;
            std::cout << "3. Search Foods" << std::endl;
            std::cout << "4. Import Foods from Web" << std::endl;
            std::cout << "5. Back to Main Menu" << std::endl;
            std::cout << "------------------------------------" << std::endl;
            std::cout << "Select an option (1-5): ";
            
            int choice = getMenuChoice(1, 5);
            
            switch (choice) {
                case 1:
                    addBasicFood();
                    break;
                case 2:
                    createCompositeFood();
                    break;
                case 3:
                    searchFoods();
                    break;
                case 4:
                    importFoodsFromWeb();
                    break;
                case 5:
                    backToMainMenu = true;
                    break;
                default:
                    std::cout << "Invalid choice." << std::endl;
                    waitForEnter();
            }
        }
    }

    void manageDailyLog() {
        bool backToMainMenu = false;
        
        while (!backToMainMenu) {
            clearScreen();
            std::cout << "========== Daily Log Management ==========" << std::endl;
            std::cout << "Current Date: " << m_currentDate << std::endl;
            std::cout << "1. Add Food to Log" << std::endl;
            std::cout << "2. Remove Food from Log" << std::endl;
            std::cout << "3. View Log for Current Date" << std::endl;
            std::cout << "4. Change Date" << std::endl;
            std::cout << "5. Back to Main Menu" << std::endl;
            std::cout << "-----------------------------------------" << std::endl;
            std::cout << "Select an option (1-5): ";
            
            int choice = getMenuChoice(1, 5);
            
            switch (choice) {
                case 1:
                    addFoodToLog();
                    break;
                case 2:
                    removeFoodFromLog();
                    break;
                case 3:
                    viewDailyLog();
                    break;
                case 4:
                    changeDate();
                    break;
                case 5:
                    backToMainMenu = true;
                    break;
                default:
                    std::cout << "Invalid choice." << std::endl;
                    waitForEnter();
            }
        }
    }

    void manageDietProfile() {
        bool backToMainMenu = false;
        
        while (!backToMainMenu) {
            clearScreen();
            std::cout << "========== Diet Profile Management ==========" << std::endl;
            displayCurrentProfile();
            std::cout << "1. Update Gender" << std::endl;
            std::cout << "2. Update Height" << std::endl;
            std::cout << "3. Update Weight" << std::endl;
            std::cout << "4. Update Age" << std::endl;
            std::cout << "5. Update Activity Level" << std::endl;
            std::cout << "6. Change Calorie Calculation Method" << std::endl;
            std::cout << "7. Back to Main Menu" << std::endl;
            std::cout << "--------------------------------------------" << std::endl;
            std::cout << "Select an option (1-7): ";
            
            int choice = getMenuChoice(1, 7);
            
            switch (choice) {
                case 1:
                    updateGender();
                    break;
                case 2:
                    updateHeight();
                    break;
                case 3:
                    updateWeight();
                    break;
                case 4:
                    updateAge();
                    break;
                case 5:
                    updateActivityLevel();
                    break;
                case 6:
                    updateCalorieMethod();
                    break;
                case 7:
                    backToMainMenu = true;
                    break;
                default:
                    std::cout << "Invalid choice." << std::endl;
                    waitForEnter();
            }
        }
    }

    void viewDailyCalorieReport() {
        clearScreen();
        std::cout << "========== Daily Calorie Report ==========" << std::endl;
        std::cout << "Date: " << m_currentDate << std::endl;
        
        // Calculate target calories
        double targetCalories = m_profile.calculateDailyCalorieTarget();
        
        // Calculate consumed calories
        double consumedCalories = m_dailyLog.calculateTotalCaloriesForDate(m_currentDate, m_database);
        
        // Calculate difference
        double difference = consumedCalories - targetCalories;
        
        std::cout << "Target Calories: " << targetCalories << std::endl;
        std::cout << "Consumed Calories: " << consumedCalories << std::endl;
        std::cout << "Difference: " << difference << " calories ";
        
        if (difference < 0) {
            std::cout << "(You can consume " << -difference << " more calories today)" << std::endl;
        } else if (difference > 0) {
            std::cout << "(You've consumed " << difference << " calories over your target)" << std::endl;
        } else {
            std::cout << "(You've hit your target exactly!)" << std::endl;
        }
        
        std::cout << std::endl << "Current Calculation Method: ";
        switch (m_profile.getCalorieCalculationMethod()) {
            case CalorieCalculationMethod::MIFFLIN_ST_JEOR:
                std::cout << "Mifflin-St Jeor" << std::endl;
                break;
            case CalorieCalculationMethod::HARRIS_BENEDICT:
                std::cout << "Harris-Benedict" << std::endl;
                break;
        }
        
        waitForEnter();
    }

    void saveAllData() {
        m_database.saveDatabase();
        m_profile.saveProfile();
        m_dailyLog.saveLogs();
        std::cout << "All data saved successfully." << std::endl;
        waitForEnter();
    }

    void about() {
        clearScreen();
        std::cout << "========== About YADA ==========" << std::endl;
        std::cout << "YADA (Yet Another Diet Assistant)" << std::endl;
        std::cout << "Version 1.0" << std::endl;
        std::cout << std::endl;
        std::cout << "A diet management system for tracking food consumption" << std::endl;
        std::cout << "and calorie intake to help maintain a healthy diet." << std::endl;
        waitForEnter();
    }

    // Helper functions for food management
    void addBasicFood() {
        clearScreen();
        std::cout << "========== Add Basic Food ==========" << std::endl;
        
        std::string identifier;
        std::cout << "Enter food identifier: ";
        std::getline(std::cin, identifier);
        
        // Check if food already exists
        auto existingFood = m_database.findFood(identifier);
        if (existingFood) {
            std::cout << "A food with this identifier already exists." << std::endl;
            waitForEnter();
            return;
        }
        
        std::string keywordsStr;
        std::cout << "Enter keywords (comma-separated): ";
        std::getline(std::cin, keywordsStr);
        
        std::vector<std::string> keywords = splitString(keywordsStr, ',');
        
        double calories;
        std::cout << "Enter calories per serving: ";
        std::cin >> calories;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        Food newFood(identifier, keywords, calories, Food::Type::BASIC);
        
        auto command = std::make_unique<AddFoodToDatabaseCommand>(m_database, newFood);
        m_commandManager.executeCommand(std::move(command));
        
        std::cout << "Basic food added successfully." << std::endl;
        waitForEnter();
    }

    void createCompositeFood() {
        clearScreen();
        std::cout << "========== Create Composite Food ==========" << std::endl;
        
        std::string identifier;
        std::cout << "Enter composite food identifier: ";
        std::getline(std::cin, identifier);
        
        // Check if food already exists
        auto existingFood = m_database.findFood(identifier);
        if (existingFood) {
            std::cout << "A food with this identifier already exists." << std::endl;
            waitForEnter();
            return;
        }
        
        std::string keywordsStr;
        std::cout << "Enter keywords (comma-separated): ";
        std::getline(std::cin, keywordsStr);
        
        std::vector<std::string> keywords = splitString(keywordsStr, ',');
        
        CompositeFood compositeFood(identifier, keywords);
        
        bool addingComponents = true;
        while (addingComponents) {
            std::cout << std::endl << "Search for food to add (enter keywords, or 'done' to finish): ";
            std::string searchQuery;
            std::getline(std::cin, searchQuery);
            
            if (toLower(searchQuery) == "done") {
                addingComponents = false;
                continue;
            }
            
            std::vector<std::string> searchKeywords = splitString(searchQuery, ' ');
            std::vector<Food> foundFoods = m_database.searchFoodByKeywords(searchKeywords, false);
            
            if (foundFoods.empty()) {
                std::cout << "No foods found matching those keywords." << std::endl;
                continue;
            }
            
            std::cout << "Found foods:" << std::endl;
            for (size_t i = 0; i < foundFoods.size(); ++i) {
                std::cout << i + 1 << ". " << foundFoods[i].getIdentifier() 
                          << " (" << foundFoods[i].getCaloriesPerServing() << " calories/serving)" << std::endl;
            }
            
            std::cout << "Select a food (1-" << foundFoods.size() << "), or 0 to search again: ";
            int foodChoice = getMenuChoice(0, foundFoods.size());
            
            if (foodChoice == 0) {
                continue;
            }
            
            int servings;
            std::cout << "Enter number of servings: ";
            std::cin >> servings;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (servings <= 0) {
                std::cout << "Servings must be greater than 0." << std::endl;
                continue;
            }
            
            compositeFood.addComponent(foundFoods[foodChoice - 1].getIdentifier(), servings);
            std::cout << "Added " << servings << " serving(s) of " << foundFoods[foodChoice - 1].getIdentifier() << std::endl;
        }
        
        if (compositeFood.getComponents().empty()) {
            std::cout << "Composite food must have at least one component." << std::endl;
            waitForEnter();
            return;
        }
        
        // Calculate calories
        double totalCalories = compositeFood.calculateTotalCalories(m_database);
        std::cout << "Total calories per serving: " << totalCalories << std::endl;
        
        // Add to database
        auto command = std::make_unique<AddFoodToDatabaseCommand>(m_database, compositeFood);
        m_commandManager.executeCommand(std::move(command));
        
        std::cout << "Composite food added successfully." << std::endl;
        waitForEnter();
    }

    void searchFoods() {
        clearScreen();
        std::cout << "========== Search Foods ==========" << std::endl;
        
        std::string searchQuery;
        std::cout << "Enter search keywords: ";
        std::getline(std::cin, searchQuery);
        
        std::vector<std::string> keywords = splitString(searchQuery, ' ');
        
        bool matchAll;
        std::cout << "Match all keywords? (y/n): ";
        char matchChoice;
        std::cin >> matchChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        matchAll = (toLower(std::string(1, matchChoice)) == "y");
        
        std::vector<Food> foundFoods = m_database.searchFoodByKeywords(keywords, matchAll);
        
        if (foundFoods.empty()) {
            std::cout << "No foods found matching those keywords." << std::endl;
        } else {
            std::cout << "Found " << foundFoods.size() << " food(s):" << std::endl;
            for (const auto& food : foundFoods) {
                std::cout << "- " << food.getIdentifier() 
                          << " (" << food.getCaloriesPerServing() << " calories/serving)";
                
                if (food.getType() == Food::Type::COMPOSITE) {
                    std::cout << " [Composite]";
                }
                
                std::cout << std::endl;
                
                std::cout << "  Keywords: ";
                const auto& foodKeywords = food.getKeywords();
                for (size_t i = 0; i < foodKeywords.size(); ++i) {
                    std::cout << foodKeywords[i];
                    if (i < foodKeywords.size() - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << std::endl;
            }
        }
        
        waitForEnter();
    }

    void importFoodsFromWeb() {
        clearScreen();
        std::cout << "========== Import Foods from Web ==========" << std::endl;
        std::cout << "1. Import from McDonald's" << std::endl;
        std::cout << "2. Back" << std::endl;
        std::cout << "Select an option: ";
        
        int choice = getMenuChoice(1, 2);
        
        if (choice == 1) {
            std::cout << "Importing from McDonald's..." << std::endl;
            // This would call the web scraper functionality
            std::cout << "Feature not fully implemented in this demo." << std::endl;
            waitForEnter();
        }
    }

    // Helper functions for daily log management
    void addFoodToLog() {
        clearScreen();
        std::cout << "========== Add Food to Log ==========" << std::endl;
        std::cout << "Date: " << m_currentDate << std::endl;
        
        std::string searchQuery;
        std::cout << "Search for food (enter keywords): ";
        std::getline(std::cin, searchQuery);
        
        std::vector<std::string> keywords = splitString(searchQuery, ' ');
        std::vector<Food> foundFoods = m_database.searchFoodByKeywords(keywords, false);
        
        if (foundFoods.empty()) {
            std::cout << "No foods found matching those keywords." << std::endl;
            waitForEnter();
            return;
        }
        
        std::cout << "Found foods:" << std::endl;
        for (size_t i = 0; i < foundFoods.size(); ++i) {
            std::cout << i + 1 << ". " << foundFoods[i].getIdentifier() 
                      << " (" << foundFoods[i].getCaloriesPerServing() << " calories/serving)" << std::endl;
        }
        
        std::cout << "Select a food (1-" << foundFoods.size() << "), or 0 to cancel: ";
        int foodChoice = getMenuChoice(0, foundFoods.size());
        
        if (foodChoice == 0) {
            return;
        }
        
        int servings;
        std::cout << "Enter number of servings: ";
        std::cin >> servings;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (servings <= 0) {
            std::cout << "Servings must be greater than 0." << std::endl;
            waitForEnter();
            return;
        }
        
        std::string selectedFoodId = foundFoods[foodChoice - 1].getIdentifier();
        
        auto command = std::make_unique<AddFoodToLogCommand>(m_dailyLog, m_currentDate, selectedFoodId, servings);
        m_commandManager.executeCommand(std::move(command));
        
        std::cout << "Added " << servings << " serving(s) of " << selectedFoodId << " to log." << std::endl;
        waitForEnter();
    }

    void removeFoodFromLog() {
        clearScreen();
        std::cout << "========== Remove Food from Log ==========" << std::endl;
        std::cout << "Date: " << m_currentDate << std::endl;
        
        std::vector<LogEntry> logEntries = m_dailyLog.getFoodLogForDate(m_currentDate);
        
        if (logEntries.empty()) {
            std::cout << "No foods in log for this date." << std::endl;
            waitForEnter();
            return;
        }
        
        std::cout << "Foods in log:" << std::endl;
        for (size_t i = 0; i < logEntries.size(); ++i) {
            auto food = m_database.findFood(logEntries[i].foodId);
            if (food) {
                std::cout << i + 1 << ". " << logEntries[i].foodId 
                          << " (" << logEntries[i].servings << " serving(s), " 
                          << (food->getCaloriesPerServing() * logEntries[i].servings) << " total calories)" << std::endl;
            } else {
                std::cout << i + 1 << ". " << logEntries[i].foodId 
                          << " (" << logEntries[i].servings << " serving(s))" << std::endl;
            }
        }
        
        std::cout << "Select a food to remove (1-" << logEntries.size() << "), or 0 to cancel: ";
        int logChoice = getMenuChoice(0, logEntries.size());
        
        if (logChoice == 0) {
            return;
        }
        
        std::string selectedFoodId = logEntries[logChoice - 1].foodId;
        
        auto command = std::make_unique<RemoveFoodFromLogCommand>(m_dailyLog, m_currentDate, selectedFoodId, logEntries[logChoice - 1].servings);
        m_commandManager.executeCommand(std::move(command));
        
        std::cout << "Removed " << selectedFoodId << " from log." << std::endl;
        waitForEnter();
    }

    void viewDailyLog() {
        clearScreen();
        std::cout << "========== Daily Log ==========" << std::endl;
        std::cout << "Date: " << m_currentDate << std::endl;
        
        std::vector<LogEntry> logEntries = m_dailyLog.getFoodLogForDate(m_currentDate);
        
        if (logEntries.empty()) {
            std::cout << "No foods in log for this date." << std::endl;
            waitForEnter();
            return;
        }
        
        double totalCalories = 0.0;
        
        std::cout << "Foods consumed:" << std::endl;
        for (const auto& entry : logEntries) {
            auto food = m_database.findFood(entry.foodId);
            if (food) {
                double entryCalories = food->getCaloriesPerServing() * entry.servings;
                totalCalories += entryCalories;
                
                std::cout << "- " << entry.foodId 
                          << " (" << entry.servings << " serving(s), " 
                          << entryCalories << " calories)" << std::endl;
            } else {
                std::cout << "- " << entry.foodId 
                          << " (" << entry.servings << " serving(s), food not found in database)" << std::endl;
            }
        }
        
        std::cout << std::endl << "Total calories: " << totalCalories << std::endl;
        
        // Display target calories
        double targetCalories = m_profile.calculateDailyCalorieTarget();
        double difference = totalCalories - targetCalories;
        
        std::cout << "Target calories: " << targetCalories << std::endl;
        std::cout << "Difference: " << difference << " calories" << std::endl;
        
        waitForEnter();
    }

    void changeDate() {
        clearScreen();
        std::cout << "========== Change Date ==========" << std::endl;
        std::cout << "Current date: " << m_currentDate << std::endl;
        
        std::cout << "Available dates in log:" << std::endl;
        std::vector<std::string> availableDates = m_dailyLog.getAvailableDates();
        
        if (!availableDates.empty()) {
            for (size_t i = 0; i < availableDates.size(); ++i) {
                std::cout << i + 1 << ". " << availableDates[i] << std::endl;
            }
            
            std::cout << std::endl;
        }
        
        std::cout << "Enter new date (YYYY-MM-DD), or 'today' for current date: ";
        std::string newDate;
        std::getline(std::cin, newDate);
        
        if (toLower(newDate) == "today") {
            m_currentDate = getCurrentDate();
            std::cout << "Date changed to today: " << m_currentDate << std::endl;
        } else {
            // Validate date format
            try {
                // This is a simple validation, a more robust implementation would check for valid day/month combinations
                if (newDate.length() == 10 && newDate[4] == '-' && newDate[7] == '-') {
                    m_currentDate = newDate;
                    std::cout << "Date changed to: " << m_currentDate << std::endl;
                } else {
                    std::cout << "Invalid date format. Please use YYYY-MM-DD." << std::endl;
                }
            } catch (...) {
                std::cout << "Invalid date format. Please use YYYY-MM-DD." << std::endl;
            }
        }
        
        waitForEnter();
    }

    // Helper functions for diet profile management
    void displayCurrentProfile() const {
        std::cout << "Current Profile:" << std::endl;
        std::cout << "Gender: " << m_profile.getGender() << std::endl;
        std::cout << "Height: " << m_profile.getHeight() << " cm" << std::endl;
        std::cout << "Weight: " << m_profile.getWeight() << " kg" << std::endl;
        std::cout << "Age: " << m_profile.getAge() << " years" << std::endl;
        
        std::cout << "Activity Level: ";
        switch (m_profile.getActivityLevel()) {
            case ActivityLevel::SEDENTARY:
                std::cout << "Sedentary";
                break;
            case ActivityLevel::LIGHTLY_ACTIVE:
                std::cout << "Lightly Active";
                break;
            case ActivityLevel::MODERATELY_ACTIVE:
                std::cout << "Moderately Active";
                break;
            case ActivityLevel::VERY_ACTIVE:
                std::cout << "Very Active";
                break;
            case ActivityLevel::EXTRA_ACTIVE:
                std::cout << "Extra Active";
                break;
        }
        std::cout << std::endl;
        
        std::cout << "Calorie Calculation Method: ";
        switch (m_profile.getCalorieCalculationMethod()) {
            case CalorieCalculationMethod::MIFFLIN_ST_JEOR:
                std::cout << "Mifflin-St Jeor";
                break;
            case CalorieCalculationMethod::HARRIS_BENEDICT:
                std::cout << "Harris-Benedict";
                break;
        }
        std::cout << std::endl << std::endl;
    }

    void updateGender() {
        clearScreen();
        std::cout << "========== Update Gender ==========" << std::endl;
        std::cout << "Current gender: " << m_profile.getGender() << std::endl;
        
        std::cout << "Enter new gender (male/female): ";
        std::string gender;
        std::getline(std::cin, gender);
        
        gender = toLower(gender);
        if (gender != "male" && gender != "female") {
            std::cout << "Invalid gender. Please enter male or female." << std::endl;
            waitForEnter();
            return;
        }
        
        auto command = std::make_unique<UpdateProfileCommand>(m_profile, "gender", gender);
        m_commandManager.executeCommand(std::move(command));
        
        std::cout << "Gender updated successfully." << std::endl;
        waitForEnter();
    }

    void updateHeight() {
        clearScreen();
        std::cout << "========== Update Height ==========" << std::endl;
        std::cout << "Current height: " << m_profile.getHeight() << " cm" << std::endl;
        
        std::cout << "Enter new height (in cm): ";
        double height;
        std::cin >> height;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (height <= 0) {
            std::cout << "Height must be greater than 0." << std::endl;
            waitForEnter();
            return;
        }
        
        auto command = std::make_unique<UpdateProfileCommand>(m_profile, "height", std::to_string(height));
        m_commandManager.executeCommand(std::move(command));
        
        std::cout << "Height updated successfully." << std::endl;
        waitForEnter();
    }

    void updateWeight() {
        clearScreen();
        std::cout << "========== Update Weight ==========" << std::endl;
        std::cout << "Current weight: " << m_profile.getWeight() << " kg" << std::endl;
        
        std::cout << "Enter new weight (in kg): ";
        double weight;
        std::cin >> weight;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (weight <= 0) {
            std::cout << "Weight must be greater than 0." << std::endl;
            waitForEnter();
            return;
        }
        
        auto command = std::make_unique<UpdateProfileCommand>(m_profile, "weight", std::to_string(weight));
        m_commandManager.executeCommand(std::move(command));
        
        std::cout << "Weight updated successfully." << std::endl;
        waitForEnter();
    }

    void updateAge() {
        clearScreen();
        std::cout << "========== Update Age ==========" << std::endl;
        std::cout << "Current age: " << m_profile.getAge() << " years" << std::endl;
        
        std::cout << "Enter new age: ";
        int age;
        std::cin >> age;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (age <= 0) {
            std::cout << "Age must be greater than 0." << std::endl;
            waitForEnter();
            return;
        }
        
        auto command = std::make_unique<UpdateProfileCommand>(m_profile, "age", std::to_string(age));
        m_commandManager.executeCommand(std::move(command));
        
        std::cout << "Age updated successfully." << std::endl;
        waitForEnter();
    }

    void updateActivityLevel() {
        clearScreen();
        std::cout << "========== Update Activity Level ==========" << std::endl;
        std::cout << "Current activity level: ";
        switch (m_profile.getActivityLevel()) {
            case ActivityLevel::SEDENTARY: std::cout << "Sedentary"; break;
            case ActivityLevel::LIGHTLY_ACTIVE: std::cout << "Lightly Active"; break;
            case ActivityLevel::MODERATELY_ACTIVE: std::cout << "Moderately Active"; break;
            case ActivityLevel::VERY_ACTIVE: std::cout << "Very Active"; break;
            case ActivityLevel::EXTRA_ACTIVE: std::cout << "Extra Active"; break;
        }
        std::cout << std::endl << std::endl;
        
        std::cout << "Select new activity level:" << std::endl;
        std::cout << "1. Sedentary (little or no exercise)" << std::endl;
        std::cout << "2. Lightly Active (light exercise 1-3 days/week)" << std::endl;
        std::cout << "3. Moderately Active (moderate exercise 3-5 days/week)" << std::endl;
        std::cout << "4. Very Active (hard exercise 6-7 days/week)" << std::endl;
        std::cout << "5. Extra Active (very hard exercise & physical job)" << std::endl;
        std::cout << "Enter choice (1-5): ";
        
        int choice = getMenuChoice(1, 5);
        ActivityLevel newLevel;
        
        switch (choice) {
            case 1: newLevel = ActivityLevel::SEDENTARY; break;
            case 2: newLevel = ActivityLevel::LIGHTLY_ACTIVE; break;
            case 3: newLevel = ActivityLevel::MODERATELY_ACTIVE; break;
            case 4: newLevel = ActivityLevel::VERY_ACTIVE; break;
            case 5: newLevel = ActivityLevel::EXTRA_ACTIVE; break;
        }
        
        auto command = std::make_unique<UpdateProfileCommand>(m_profile, "activity", std::to_string(static_cast<int>(newLevel)));
        m_commandManager.executeCommand(std::move(command));
        
        std::cout << "Activity level updated successfully." << std::endl;
        waitForEnter();
    }

    void updateCalorieMethod() {
        clearScreen();
        std::cout << "========== Update Calorie Calculation Method ==========" << std::endl;
        std::cout << "Current method: ";
        switch (m_profile.getCalorieCalculationMethod()) {
            case CalorieCalculationMethod::MIFFLIN_ST_JEOR: std::cout << "Mifflin-St Jeor"; break;
            case CalorieCalculationMethod::HARRIS_BENEDICT: std::cout << "Harris-Benedict"; break;
        }
        std::cout << std::endl << std::endl;
        
        std::cout << "Select calculation method:" << std::endl;
        std::cout << "1. Mifflin-St Jeor (more accurate for obese individuals)" << std::endl;
        std::cout << "2. Harris-Benedict (original equation, slightly less accurate)" << std::endl;
        std::cout << "Enter choice (1-2): ";
        
        int choice = getMenuChoice(1, 2);
        CalorieCalculationMethod newMethod;
        
        switch (choice) {
            case 1: newMethod = CalorieCalculationMethod::MIFFLIN_ST_JEOR; break;
            case 2: newMethod = CalorieCalculationMethod::HARRIS_BENEDICT; break;
        }
        
        auto command = std::make_unique<UpdateProfileCommand>(m_profile, "method", std::to_string(static_cast<int>(newMethod)));
        m_commandManager.executeCommand(std::move(command));
        
        std::cout << "Calculation method updated successfully." << std::endl;
        waitForEnter();
    }

    // Utility functions
    int getMenuChoice(int min, int max) const {
        int choice;
        while (true) {
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (std::cin.fail() || choice < min || choice > max) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
            } else {
                break;
            }
        }
        return choice;
    }
};

// Command implementations
class AddFoodToDatabaseCommand : public Command {
public:
    AddFoodToDatabaseCommand(FoodDatabase& db, const Food& food) 
        : m_db(db), m_food(food) {}

    void execute() override {
        if (m_food.getType() == Food::Type::BASIC) {
            m_db.addBasicFood(m_food);
        } else {
            // For composite foods, we need to cast to CompositeFood
            const CompositeFood& composite = dynamic_cast<const CompositeFood&>(m_food);
            m_db.addCompositeFood(composite);
        }
    }

    void undo() override {
        m_db.removeFood(m_food.getIdentifier());
    }

private:
    FoodDatabase& m_db;
    Food m_food;
};

class AddFoodToLogCommand : public Command {
public:
    AddFoodToLogCommand(DailyLog& log, const std::string& date, const std::string& foodId, int servings)
        : m_log(log), m_date(date), m_foodId(foodId), m_servings(servings) {}

    void execute() override {
        m_log.addFoodToLog(m_date, m_foodId, m_servings);
    }

    void undo() override {
        m_log.removeFoodFromLog(m_date, m_foodId);
    }

private:
    DailyLog& m_log;
    std::string m_date;
    std::string m_foodId;
    int m_servings;
};

class RemoveFoodFromLogCommand : public Command {
public:
    RemoveFoodFromLogCommand(DailyLog& log, const std::string& date, const std::string& foodId, int servings)
        : m_log(log), m_date(date), m_foodId(foodId), m_servings(servings) {}

    void execute() override {
        m_log.removeFoodFromLog(m_date, m_foodId);
    }

    void undo() override {
        m_log.addFoodToLog(m_date, m_foodId, m_servings);
    }

private:
    DailyLog& m_log;
    std::string m_date;
    std::string m_foodId;
    int m_servings;
};

class UpdateProfileCommand : public Command {
public:
    UpdateProfileCommand(DietProfile& profile, const std::string& field, const std::string& value)
        : m_profile(profile), m_field(field), m_newValue(value) {
        
        // Store old value for undo
        if (field == "gender") m_oldValue = m_profile.getGender();
        else if (field == "height") m_oldValue = std::to_string(m_profile.getHeight());
        else if (field == "weight") m_oldValue = std::to_string(m_profile.getWeight());
        else if (field == "age") m_oldValue = std::to_string(m_profile.getAge());
        else if (field == "activity") m_oldValue = std::to_string(static_cast<int>(m_profile.getActivityLevel()));
        else if (field == "method") m_oldValue = std::to_string(static_cast<int>(m_profile.getCalorieCalculationMethod()));
    }

    void execute() override {
        applyValue(m_newValue);
    }

    void undo() override {
        applyValue(m_oldValue);
    }

private:
    void applyValue(const std::string& value) {
        if (m_field == "gender") {
            m_profile.setGender(value);
        } else if (m_field == "height") {
            m_profile.setHeight(std::stod(value));
        } else if (m_field == "weight") {
            m_profile.setWeight(std::stod(value));
        } else if (m_field == "age") {
            m_profile.setAge(std::stoi(value));
        } else if (m_field == "activity") {
            m_profile.setActivityLevel(static_cast<ActivityLevel>(std::stoi(value)));
        } else if (m_field == "method") {
            m_profile.setCalorieCalculationMethod(static_cast<CalorieCalculationMethod>(std::stoi(value)));
        }
    }

    DietProfile& m_profile;
    std::string m_field;
    std::string m_newValue;
    std::string m_oldValue;
};

// Utility function implementations
std::string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
    
    return std::string(buffer);
}

std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), 
                   [](unsigned char c){ return std::tolower(c); });
    return lowerStr;
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter)) {
        // Trim whitespace from each token
        token.erase(token.begin(), std::find_if(token.begin(), token.end(), [](int ch) { return !std::isspace(ch); }));
        token.erase(std::find_if(token.rbegin(), token.rend(), [](int ch) { return !std::isspace(ch); }).base(), token.end());
        
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void waitForEnter() {
    std::cout << std::endl << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    YADAApp app;
    app.run();
    return 0;
}