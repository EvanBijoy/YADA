#include "food.h"
#include <iostream>
#include <sstream>

void FoodList::displayOptions() {
    while (true) {
        cout << "\nFood Database Options:\n";
        cout << "1. Add new food\n";
        cout << "2. Search foods\n";
        cout << "3. Save database\n";
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
            case 1: addFood(); break;
            case 2: searchFood(); break;
            case 3: 
                saveToFiles();
                cout << "Database saved successfully.\n";
                break;
            case 4: return;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}

void FoodList::addFood() {
    cout << "\nAdd Food:\n";
    cout << "1. Basic food\n";
    cout << "2. Composite food\n";
    cout << "Enter choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 1) addBasicFood();
    else if (choice == 2) addCompositeFood();
    else cout << "Invalid choice.\n";
}

void FoodList::addBasicFood() {
    string name;
    string keywordInput;
    vector<string> keywords;
    int calories;
    
    cout << "Enter food name: ";
    getline(cin, name);
    
    cout << "Enter keywords (space separated): ";
    getline(cin, keywordInput);
    
    istringstream iss(keywordInput);
    string keyword;
    while (iss >> keyword) {
        keywords.push_back(keyword);
    }
    
    cout << "Enter calories per serving: ";
    cin >> calories;
    cin.ignore();
    
    foods.emplace_back(name, keywords, calories, false);
    cout << "Basic food added successfully.\n";
}

void FoodList::addCompositeFood() {
    string name;
    string keywordInput;
    vector<string> keywords;
    int totalCalories = 0;
    
    cout << "Enter composite food name: ";
    getline(cin, name);
    
    cout << "Enter keywords (space separated): ";
    getline(cin, keywordInput);
    
    istringstream iss(keywordInput);
    string keyword;
    while (iss >> keyword) {
        keywords.push_back(keyword);
    }
    
    cout << "\nAvailable basic foods:\n";
    int basicCount = 0;
    for (size_t i = 0; i < foods.size(); ++i) {
        if (!foods[i].isComposite) {
            cout << ++basicCount << ". " << foods[i].name << " (" << foods[i].calories << " cal)\n";
        }
    }
    
    if (basicCount == 0) {
        cout << "No basic foods available. Please add basic foods first.\n";
        return;
    }
    
    cout << "\nEnter numbers of basic foods to include (space separated, 0 to finish):\n";
    vector<int> selections;
    int selection;
    while (cin >> selection && selection != 0) {
        if (selection > 0 && selection <= basicCount) {
            // Find the actual index in foods vector
            int actualIndex = -1;
            int count = 0;
            for (size_t i = 0; i < foods.size(); ++i) {
                if (!foods[i].isComposite) {
                    if (++count == selection) {
                        actualIndex = i;
                        break;
                    }
                }
            }
            if (actualIndex != -1) {
                selections.push_back(actualIndex);
            }
        }
    }
    cin.ignore();
    
    for (int idx : selections) {
        totalCalories += foods[idx].calories;
    }
    
    foods.emplace_back(name, keywords, totalCalories, true);
    cout << "Composite food added successfully with " << totalCalories << " calories.\n";
}

void FoodList::searchFood() {
    string keywordInput;
    cout << "Enter keywords to search (space separated): ";
    getline(cin, keywordInput);
    
    cout << "Match:\n1. Any keyword\n2. All keywords\nEnter choice: ";
    int choice;
    cin >> choice;
    cin.ignore();
    
    istringstream iss(keywordInput);
    vector<string> keywords;
    string keyword;
    while (iss >> keyword) {
        keywords.push_back(keyword);
    }
    
    vector<Food> results = searchByKeywords(keywords, choice == 2);
    
    cout << "\nSearch Results:\n";
    if (results.empty()) {
        cout << "No foods found.\n";
    } else {
        for (const auto& food : results) {
            food.print();
            cout << endl;
        }
    }
}

vector<Food> FoodList::searchByKeywords(const vector<string>& keywords, bool matchAll) {
    vector<Food> results;
    
    for (const auto& food : foods) {
        bool found = matchAll;
        
        for (const auto& keyword : keywords) {
            bool keywordFound = false;
            for (const auto& foodKeyword : food.keywords) {
                if (foodKeyword == keyword) {
                    keywordFound = true;
                    break;
                }
            }
            
            if (matchAll) {
                found = found && keywordFound;
                if (!found) break;
            } else {
                found = found || keywordFound;
                if (found) break;
            }
        }
        
        if (found) {
            results.push_back(food);
        }
    }
    
    return results;
}

void FoodList::saveToFiles() {
    saveToFile(basicFile, false);
    saveToFile(compositeFile, true);
}

void FoodList::saveToFile(const string& filename, bool isComposite) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }
    
    for (const auto& food : foods) {
        if (food.isComposite == isComposite) {
            outFile << food.name << " " << food.calories;
            for (const auto& keyword : food.keywords) {
                outFile << " " << keyword;
            }
            outFile << "\n";
        }
    }
    
    outFile.close();
}

void FoodList::loadFromFiles() {
    foods.clear();
    loadFromFile(basicFile, false);
    loadFromFile(compositeFile, true);
}

void FoodList::loadFromFile(const string& filename, bool isComposite) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "File not found, starting with empty " << (isComposite ? "composite" : "basic") 
             << " food database: " << filename << endl;
        return;
    }
    
    string line;
    while (getline(inFile, line)) {
        istringstream iss(line);
        vector<string> tokens;
        string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 2) {
            string name = tokens[0];
            int calories;
            try {
                calories = stoi(tokens[1]);
            } catch (...) {
                cerr << "Invalid calorie value in file: " << tokens[1] << endl;
                continue;
            }
            
            vector<string> keywords;
            if (tokens.size() > 2) {
                keywords.assign(tokens.begin() + 2, tokens.end());
            }
            
            foods.emplace_back(name, keywords, calories, isComposite);
        }
    }
    
    inFile.close();
}