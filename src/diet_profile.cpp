#include "diet_profile.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

DietProfile::DietProfile() : 
    m_gender(""),
    m_height(0),
    m_weight(0),
    m_age(0),
    m_activityLevel(ActivityLevel::SEDENTARY),
    m_calorieCalculationMethod(CalorieCalculationMethod::MIFFLIN_ST_JEOR) {
    loadProfile();
}

void DietProfile::loadProfile() {
    try {
        std::ifstream profileFile("data/diet_profile.json");
        if (profileFile.is_open()) {
            nlohmann::json profileJson;
            profileFile >> profileJson;

            m_gender = profileJson.value("gender", "");
            m_height = profileJson.value("height", 0.0);
            m_weight = profileJson.value("weight", 0.0);
            m_age = profileJson.value("age", 0);
            
            // Convert string to enum
            std::string activityLevelStr = profileJson.value("activityLevel", "SEDENTARY");
            if (activityLevelStr == "SEDENTARY") m_activityLevel = ActivityLevel::SEDENTARY;
            else if (activityLevelStr == "LIGHTLY_ACTIVE") m_activityLevel = ActivityLevel::LIGHTLY_ACTIVE;
            else if (activityLevelStr == "MODERATELY_ACTIVE") m_activityLevel = ActivityLevel::MODERATELY_ACTIVE;
            else if (activityLevelStr == "VERY_ACTIVE") m_activityLevel = ActivityLevel::VERY_ACTIVE;
            else if (activityLevelStr == "EXTRA_ACTIVE") m_activityLevel = ActivityLevel::EXTRA_ACTIVE;

            std::string calculationMethodStr = profileJson.value("calorieCalculationMethod", "MIFFLIN_ST_JEOR");
            m_calorieCalculationMethod = (calculationMethodStr == "HARRIS_BENEDICT") 
                ? CalorieCalculationMethod::HARRIS_BENEDICT 
                : CalorieCalculationMethod::MIFFLIN_ST_JEOR;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading diet profile: " << e.what() << std::endl;
    }
}

void DietProfile::saveProfile() {
    nlohmann::json profileJson;

    profileJson["gender"] = m_gender;
    profileJson["height"] = m_height;
    profileJson["weight"] = m_weight;
    profileJson["age"] = m_age;

    // Convert enum to string
    switch (m_activityLevel) {
        case ActivityLevel::SEDENTARY: 
            profileJson["activityLevel"] = "SEDENTARY"; 
            break;
        case ActivityLevel::LIGHTLY_ACTIVE: 
            profileJson["activityLevel"] = "LIGHTLY_ACTIVE"; 
            break;
        case ActivityLevel::MODERATELY_ACTIVE: 
            profileJson["activityLevel"] = "MODERATELY_ACTIVE"; 
            break;
        case ActivityLevel::VERY_ACTIVE: 
            profileJson["activityLevel"] = "VERY_ACTIVE"; 
            break;
        case ActivityLevel::EXTRA_ACTIVE: 
            profileJson["activityLevel"] = "EXTRA_ACTIVE"; 
            break;
    }

    profileJson["calorieCalculationMethod"] = 
        (m_calorieCalculationMethod == CalorieCalculationMethod::HARRIS_BENEDICT) 
        ? "HARRIS_BENEDICT" : "MIFFLIN_ST_JEOR";

    std::ofstream profileFile("data/diet_profile.json");
    profileFile << profileJson.dump(4);
}

// Setters
void DietProfile::setGender(const std::string& gender) { 
    if (gender != "male" && gender != "female") {
        throw std::invalid_argument("Gender must be 'male' or 'female'");
    }
    m_gender = gender; 
}

void DietProfile::setHeight(double height) { 
    if (height <= 0) {
        throw std::invalid_argument("Height must be positive");
    }
    m_height = height; 
}

void DietProfile::setWeight(double weight) { 
    if (weight <= 0) {
        throw std::invalid_argument("Weight must be positive");
    }
    m_weight = weight; 
}

void DietProfile::setAge(int age) { 
    if (age <= 0 || age > 120) {
        throw std::invalid_argument("Age must be between 1 and 120");
    }
    m_age = age; 
}

void DietProfile::setActivityLevel(ActivityLevel level) { 
    m_activityLevel = level; 
}

void DietProfile::setCalorieCalculationMethod(CalorieCalculationMethod method) { 
    m_calorieCalculationMethod = method; 
}

// Getters
std::string DietProfile::getGender() const { return m_gender; }
double DietProfile::getHeight() const { return m_height; }
double DietProfile::getWeight() const { return m_weight; }
int DietProfile::getAge() const { return m_age; }
ActivityLevel DietProfile::getActivityLevel() const { return m_activityLevel; }
CalorieCalculationMethod DietProfile::getCalorieCalculationMethod() const { 
    return m_calorieCalculationMethod; 
}

double DietProfile::calculateDailyCalorieTarget() const {
    double baseCalories = (m_calorieCalculationMethod == CalorieCalculationMethod::MIFFLIN_ST_JEOR)
        ? calculateMifflinStJeorCalories()
        : calculateHarrisBenedictCalories();

    return baseCalories * getActivityMultiplier();
}

double DietProfile::calculateMifflinStJeorCalories() const {
    // Mifflin-St Jeor Equation
    double baseCalories = (10 * m_weight) + (6.25 * m_height) - (5 * m_age);
    baseCalories += (m_gender == "male") ? 5 : -161;
    return baseCalories;
}

double DietProfile::calculateHarrisBenedictCalories() const {
    // Harris-Benedict Equation
    double baseCalories = (m_gender == "male")
        ? 66.5 + (13.75 * m_weight) + (5.003 * m_height) - (6.75 * m_age)
        : 655.1 + (9.563 * m_weight) + (1.850 * m_height) - (4.676 * m_age);
    return baseCalories;
}

double DietProfile::getActivityMultiplier() const {
    switch (m_activityLevel) {
        case ActivityLevel::SEDENTARY: return 1.2;
        case ActivityLevel::LIGHTLY_ACTIVE: return 1.375;
        case ActivityLevel::MODERATELY_ACTIVE: return 1.55;
        case ActivityLevel::VERY_ACTIVE: return 1.725;
        case ActivityLevel::EXTRA_ACTIVE: return 1.9;
        default: return 1.2;
    }
}