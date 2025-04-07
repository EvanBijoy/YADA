#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <vector>
#include "../json.hpp"

class Food {
public:
    enum class Type {
        BASIC,
        COMPOSITE
    };

    Food() = default;
    Food(const std::string& identifier, const std::vector<std::string>& keywords, double calories, Type type);

    // Getters
    std::string getIdentifier() const;
    std::vector<std::string> getKeywords() const;
    double getCaloriesPerServing() const;
    Type getType() const;

    // Setters
    void setIdentifier(const std::string& identifier);
    void addKeyword(const std::string& keyword);
    void setCaloriesPerServing(double calories);

    // JSON serialization
    nlohmann::json toJson() const;
    static Food fromJson(const nlohmann::json& j);

private:
    std::string m_identifier;
    std::vector<std::string> m_keywords;
    double m_caloriesPerServing;
    Type m_type;
};

class CompositeFood : public Food {
public:
    CompositeFood(const std::string& identifier, const std::vector<std::string>& keywords);

    void addComponent(const std::string& foodId, int servings);
    std::map<std::string, int> getComponents() const;
    double calculateTotalCalories(const class FoodDatabase& database) const;

    // JSON serialization
    nlohmann::json toJson() const;
    static CompositeFood fromJson(const nlohmann::json& j);

private:
    std::map<std::string, int> m_components;
};

#endif // FOOD_H