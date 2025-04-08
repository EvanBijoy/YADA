#ifndef PROFILE_H
#define PROFILE_H

#include <bits/stdc++.h>
using namespace std;

class DietProfile {
public:
    void displayOptions();
    int calculateTargetCalories() const;
    void loadFromFile();

private:
    string gender;
    int age = 0;
    double height = 0.0;
    double weight = 0.0;
    string activityLevel;
    string profileFile = "data/profile.txt";
    void updateProfile();
    void viewProfile();
};

#endif
