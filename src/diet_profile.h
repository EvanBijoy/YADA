#ifndef DIET_PROFILE_H
#define DIET_PROFILE_H

#include <string>
#include "../json.hpp"

enum class ActivityLevel {
    SEDENTARY,
    LIGHTLY_ACTIVE,
    MODERATELY_ACTIVE,
    VERY_ACTIVE,
    EXTRA_ACTIVE
};

enum class CalorieCalculationMethod {
    MIFFLIN_ST_JEOR,
    HARRIS_BENEDICT
};

class DietProfile {
public:
    DietProfile();

    void loadProfile();
    void saveProfile();

    // Setters
    void setGender(const std::string& gender);
    void setHeight(double height);
    void setWeight(double weight);
    void setAge(int age);
    void setActivityLevel(ActivityLevel level);
    void setCalorieCalculationMethod(CalorieCalculationMethod method);

    // Getters
    std::string getGender() const;
    double getHeight() const;
    double getWeight() const;
    int getAge() const;
    ActivityLevel getActivityLevel() const;
    CalorieCalculationMethod getCalorieCalculationMethod() const;

    // Calorie calculation methods
    double calculateDailyCalorieTarget() const;

private:
    std::string m_gender;
    double m_height;  // in cm
    double m_weight;  // in kg
    int m_age;
    ActivityLevel m_activityLevel;
    CalorieCalculationMethod m_calorieCalculationMethod;

    // Helper methods for calorie calculation
    double calculateMifflinStJeorCalories() const;
    double calculateHarrisBenedictCalories() const;
    double getActivityMultiplier() const;
};

#endif // DIET_PROFILE_H