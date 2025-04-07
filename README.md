# Project overview

This console-based application manages a food database, tracks your daily food consumption, and maintains a diet profile. It provides a simple command-line menu with these functionalities:

## 1. Food Database
Launches `FoodList::displayOptions`, which has its own menu:
* Add new food (basic or composite) using `FoodList::addFood`.
* Search foods by keyword with `FoodList::searchFood`.
* Save the database via `FoodList::saveToFiles`.

## 2. Food Log
Launches `LogManager::displayOptions`, offering these features:
* Add food entries (with date and servings).
* View today’s log or a log for a specific date using `LogManager::viewLog`.
* Delete existing entries.
* Undo the last change via `LogManager::undo`.

## 3. Diet Profile
Uses `DietProfile::displayOptions`:
* View current profile (gender, age, height, weight, activity level).
* Update profile information with `DietProfile::updateProfile`.
* View calculated daily calorie target (calculateTargetCalories).

## 4. View Daily Summary
Prints total calories for the current day and compares against your target calories.

## 5. Exit
Saves data (using `FoodList::saveToFiles` and `LogManager::saveToFile`) and ends the program.

# How to Run the Program

To compile and run the program, use the following command:

```
g++ main.cpp food/food.cpp log/log.cpp profile/profile.cpp -Ifood -Ilog -Iprofile -std=c++17 -o yada
```