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
                viewProfile();
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
    cout << "\nSelect Calorie Calculation Method:\n";
    cout << "1. Harris-Benedict Equation\n";
    cout << "2. Mifflin-St Jeor Equation\n";
    cout << "3. Katch-McArdle Equation (assumes 20% body fat)\n";
    cout << "Enter choice: ";

    int method;
    cin >> method;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    double bmr = 0.0;

    switch (method) {
        case 1:
            if (gender == "male") {
                bmr = 88.362 + (13.397 * weight) + (4.799 * height) - (5.677 * age);
            } else {
                bmr = 447.593 + (9.247 * weight) + (3.098 * height) - (4.330 * age);
            }
            break;
        case 2:
            if (gender == "male") {
                bmr = (10 * weight) + (6.25 * height) - (5 * age) + 5;
            } else {
                bmr = (10 * weight) + (6.25 * height) - (5 * age) - 161;
            }
            break;
        case 3:
            {
                double leanMass = weight * 0.8;
                bmr = 370 + (21.6 * leanMass);
            }
            break;
        default:
            cout << "Invalid choice. Using Harris-Benedict as default.\n";
            if (gender == "male") {
                bmr = 88.362 + (13.397 * weight) + (4.799 * height) - (5.677 * age);
            } else {
                bmr = 447.593 + (9.247 * weight) + (3.098 * height) - (4.330 * age);
            }
    }

    double activityMultiplier = 1.2;
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

    ofstream outFile(profileFile, ios::trunc); // Open file in truncate mode to overwrite
    if (!outFile) {
        cerr << "Error opening file for writing: " << profileFile << endl;
        return;
    }
    
    outFile << "Gender: " << gender << "\n";
    outFile << "Age: " << age << "\n";
    outFile << "Height: " << height << "\n";
    outFile << "Weight: " << weight << "\n";
    outFile << "Activity Level: " << activityLevel << "\n";
    
    outFile.close();

    cout << "Profile updated.\n";
}

void DietProfile::viewProfile() {
    ifstream inFile(profileFile);
    if (!inFile || inFile.peek() == ifstream::traits_type::eof()) {
        cout << "No profile found. Redirecting to update profile...\n";
        inFile.close();
        updateProfile();
        return;
    }
    
    cout << "\nCurrent Profile:\n";
    string line;
    while (getline(inFile, line)) {
        cout << line << "\n";
    }
    
    inFile.close();
}
