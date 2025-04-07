#ifndef FOOD_H
#define FOOD_H

#include <bits/stdc++.h>
using namespace std;

struct Food {
    string name;
    vector<string> keywords;
    int calories;
    bool isComposite;

    Food(string name = "", vector<string> keywords = {}, int calories = 0, bool isComposite = false) {
        this->name = name;
        this->keywords = keywords;
        this->calories = calories;
        this->isComposite = isComposite;
    }

    void print() const {
        cout << "Name: " << name << endl;
        cout << "Type: " << (isComposite ? "Composite" : "Basic") << endl;
        cout << "Keywords: ";
        for (const auto& keyword : keywords) {
            cout << keyword << " ";
        }
        cout << endl;
        cout << "Calories: " << calories << endl;
    }
};

class FoodList {
public:
    void addFood();
    void searchFood();
    void displayOptions();
    void saveToFiles();
    void loadFromFiles();
    const vector<Food>& getFoods() const { return foods; }
    
private:
    vector<Food> foods;
    string basicFile = "data/basicFood.txt";
    string compositeFile = "data/complexFood.txt";
    void addBasicFood();
    void addCompositeFood();
    vector<Food> searchByKeywords(const vector<string>& keywords, bool matchAll);
    void saveToFile(const string& filename, bool isComposite);
    void loadFromFile(const string& filename, bool isComposite);
};

#endif // FOOD_H