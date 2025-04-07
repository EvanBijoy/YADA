#ifndef PROFILE_H
#define PROFILE_H

#include <string>

class DietProfile {
public:
    void displayOptions();
    int calculateTargetCalories() const;

private:
    std::string gender;
    int age = 0;
    double height = 0.0;
    double weight = 0.0;
    std::string activityLevel;

    void updateProfile();
};

#endif
