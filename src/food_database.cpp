#include "food_database.h"
#include <fstream>
#include <iostream>
#include <algorithm>

FoodDatabase::FoodDatabase() {
    loadDatabase();
}

void FoodDatabase::loadDatabase() {
    // Load basic foods
    try {
        std::ifstream basicFoodsFile("data/basic_foods.json");
        if (basicFoodsFile.is_open()) {
            nlohmann::json basicFoodsJson;
            basicFoodsFile >> basicFoodsJson;

            for (const auto& foodJson : basicFoodsJson) {
                Food food = Food::fromJson(foodJson);
                m_basicFoods[food.getIdentifier()] = food;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading basic foods: " << e.what() << std::endl;
    }

    // Load composite foods
    try {
        std::ifstream compositeFoodsFile("data/composite_foods.json");
        if (compositeFoodsFile.is_open()) {
            nlohmann::json compositeFoodsJson;
            compositeFoodsFile >> compositeFoodsJson;

            for (const auto& foodJson : compositeFoodsJson) {
                CompositeFood food = CompositeFood::fromJson(foodJson);
                m_compositeFoods[food.getIdentifier()] = food;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading composite foods: " << e.what() << std::endl;
    }
}

void FoodDatabase::saveDatabase() {
    // Save basic foods
    {
        nlohmann::json basicFoodsJson = nlohmann::json::array();
        for (const auto& [_, food] : m_basicFoods) {
            basicFoodsJson.push_back(food.toJson());
        }

        std::ofstream basicFoodsFile("data/basic_foods.json");
        basicFoodsFile << basicFoodsJson.dump(4);
    }

    // Save composite foods
    {
        nlohmann::json compositeFoodsJson = nlohmann::json::array();
        for (const auto& [_, food] : m_compositeFoods) {
            compositeFoodsJson.push_back(food.toJson());
        }

        std::ofstream compositeFoodsFile("data/composite_foods.json");
        compositeFoodsFile << compositeFoodsJson.dump(4);
    }
}

void FoodDatabase::validateDuplicateFood(const std::string& identifier) const {
    if (m_basicFoods.find(identifier) != m_basicFoods.end() ||
        m_compositeFoods.find(identifier) != m_compositeFoods.end()) {
        throw std::runtime_error("Food with identifier '" + identifier + "' already exists.");
    }
}

void FoodDatabase::addBasicFood(const Food& food) {
    validateDuplicateFood(food.getIdentifier());
    m_basicFoods[food.getIdentifier()] = food;
}

void FoodDatabase::addCompositeFood(const CompositeFood& food) {
    validateDuplicateFood(food.getIdentifier());
    m_compositeFoods[food.getIdentifier()] = food;
}

void FoodDatabase::removeFood(const std::string& identifier) {
    m_basicFoods.erase(identifier);
    m_compositeFoods.erase(identifier);
}

std::optional<Food> FoodDatabase::findFood(const std::string& identifier) const {
    auto basicIt = m_basicFoods.find(identifier);
    if (basicIt != m_basicFoods.end()) {
        return basicIt->second;
    }

    auto compositeIt = m_compositeFoods.find(identifier);
    if (compositeIt != m_compositeFoods.end()) {
        return compositeIt->second;
    }

    return std::nullopt;
}

std::vector<Food> FoodDatabase::searchFoodByKeywords(
    const std::vector<std::string>& keywords, 
    bool matchAll
) const {
    std::vector<Food> results;

    // Search basic foods
    for (const auto& [_, food] : m_basicFoods) {
        bool matches = matchAll 
            ? std::all_of(keywords.begin(), keywords.end(), 
                [&food](const std::string& keyword) {
                    auto& foodKeywords = food.getKeywords();
                    return std::find(foodKeywords.begin(), foodKeywords.end(), keyword) 
                           != foodKeywords.end();
                })
            : std::any_of(keywords.begin(), keywords.end(), 
                [&food](const std::string& keyword) {
                    auto& foodKeywords = food.getKeywords();
                    return std::find(foodKeywords.begin(), foodKeywords.end(), keyword) 
                           != foodKeywords.end();
                });
        
        if (matches) results.push_back(food);
    }

    // Search composite foods
    for (const auto& [_, food] : m_compositeFoods) {
        bool matches = matchAll 
            ? std::all_of(keywords.begin(), keywords.end(), 
                [&food](const std::string& keyword) {
                    auto& foodKeywords = food.getKeywords();
                    return std::find(foodKeywords.begin(), foodKeywords.end(), keyword) 
                           != foodKeywords.end();
                })
            : std::any_of(keywords.begin(), keywords.end(), 
                [&food](const std::string& keyword) {
                    auto& foodKeywords = food.getKeywords();
                    return std::find(foodKeywords.begin(), foodKeywords.end(), keyword) 
                           != foodKeywords.end();
                });
        
        if (matches) results.push_back(food);
    }

    return results;
}

const std::unordered_map<std::string, Food>& FoodDatabase::getBasicFoods() const {
    return m_basicFoods;
}

const std::unordered_map<std::string, CompositeFood>& FoodDatabase::getCompositeFoods() const {
    return m_compositeFoods;
}