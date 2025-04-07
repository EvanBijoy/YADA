#include "profile.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <limits>

using namespace std;

void DietProfile::displayOptions() {
    while (true) {
        cout << "\nDiet Profile Options:\n";
        cout << "1. View profile\n";
        cout << "2. Update profile\n";
        cout << "3. View calorie target\n";
        cout << "4. Return to main menu\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                cout << "\nCurrent Profile:\n";
                cout << "Gender: " << (gender.empty() ? "Not set" : gender) << "\n";
                cout << "Age: " << age << "\n";
                cout << "Height: " << height << " cm\n";
                cout << "Weight: " << weight << " kg\n";
                cout << "Activity Level: " << activityLevel << "\n";
                break;
            case 2:
                updateProfile();
                break;
            case 3:
                cout << "\nDaily Calorie Target: " << calculateTargetCalories() << " cal\n";
                break;
            case 4:
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}

int DietProfile::calculateTargetCalories() const {
    // Harris-Benedict equation
    double bmr;
    if (gender == "male") {
        bmr = 88.362 + (13.397 * weight) + (4.799 * height) - (5.677 * age);
    } else {
        bmr = 447.593 + (9.247 * weight) + (3.098 * height) - (4.330 * age);
    }

    // Activity multiplier
    double activityMultiplier = 1.2; // Sedentary default
    if (activityLevel == "light") activityMultiplier = 1.375;
    else if (activityLevel == "moderate") activityMultiplier = 1.55;
    else if (activityLevel == "active") activityMultiplier = 1.725;
    else if (activityLevel == "very active") activityMultiplier = 1.9;

    return static_cast<int>(bmr * activityMultiplier);
}

void DietProfile::updateProfile() {
    while (true) {
        cout << "Select gender:\n";
        cout << "1. Male\n";
        cout << "2. Female\n";
        cout << "Enter choice: ";
        int genderChoice;
        cin >> genderChoice;

        if (cin.fail() || (genderChoice != 1 && genderChoice != 2)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 1 for Male or 2 for Female.\n";
            continue;
        }

        gender = (genderChoice == 1) ? "male" : "female";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
    }

    while (true) {
        cout << "Enter age: ";
        cin >> age;

        if (cin.fail() || age <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid age.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
    }

    while (true) {
        cout << "Enter height (cm): ";
        cin >> height;

        if (cin.fail() || height <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid height.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
    }

    while (true) {
        cout << "Enter weight (kg): ";
        cin >> weight;

        if (cin.fail() || weight <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid weight.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
    }

    while (true) {
        cout << "Select activity level:\n";
        cout << "1. Sedentary\n";
        cout << "2. Light\n";
        cout << "3. Moderate\n";
        cout << "4. Active\n";
        cout << "5. Very Active\n";
        cout << "Enter choice: ";
        int activityChoice;
        cin >> activityChoice;

        if (cin.fail() || activityChoice < 1 || activityChoice > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please select a valid activity level (1-5).\n";
            continue;
        }

        switch (activityChoice) {
            case 1: activityLevel = "sedentary"; break;
            case 2: activityLevel = "light"; break;
            case 3: activityLevel = "moderate"; break;
            case 4: activityLevel = "active"; break;
            case 5: activityLevel = "very active"; break;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
    }

    cout << "Profile updated.\n";
}
