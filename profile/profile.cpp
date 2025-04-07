#include "profile.h"
#include <iostream>

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
        cin.ignore();
        
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
    cout << "Enter gender (male/female): ";
    getline(cin, gender);

    cout << "Enter age: ";
    cin >> age;

    cout << "Enter height (cm): ";
    cin >> height;

    cout << "Enter weight (kg): ";
    cin >> weight;
    cin.ignore();

    cout << "Enter activity level (sedentary/light/moderate/active/very active): ";
    getline(cin, activityLevel);

    cout << "Profile updated.\n";
}
