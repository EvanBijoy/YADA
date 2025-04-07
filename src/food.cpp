#include "food.h"
#include "food_database.h"

Food::Food(const std::string& identifier, const std::vector<std::string>& keywords, 
           double calories, Type type)
    : m_identifier(identifier), m_keywords(keywords), 
      m_caloriesPerServing(calories), m_type(type) {}

std::string Food::getIdentifier() const { return m_identifier; }
std::vector<std::string> Food::getKeywords() const { return m_keywords; }
double Food::getCaloriesPerServing() const { return m_caloriesPerServing; }
Food::Type Food::getType() const { return m_type; }

void Food::setIdentifier(const std::string& identifier) { m_identifier = identifier; }
void Food::addKeyword(const std::string& keyword) { m_keywords.push_back(keyword); }
void Food::setCaloriesPerServing(double calories) { m_caloriesPerServing = calories; }

nlohmann::json Food::toJson() const {
    return {
        {"identifier", m_identifier},
        {"keywords", m_keywords},
        {"calories", m_caloriesPerServing},
        {"type", m_type == Type::BASIC ? "BASIC" : "COMPOSITE"}
    };
}

Food Food::fromJson(const nlohmann::json& j) {
    return Food(
        j.at("identifier").get<std::string>(),
        j.at("keywords").get<std::vector<std::string>>(),
        j.at("calories").get<double>(),
        j.at("type").get<std::string>() == "BASIC" ? Type::BASIC : Type::COMPOSITE
    );
}

CompositeFood::CompositeFood(const std::string& identifier, 
                              const std::vector<std::string>& keywords)
    : Food(identifier, keywords, 0, Type::COMPOSITE) {}

void CompositeFood::addComponent(const std::string& foodId, int servings) {
    m_components[foodId] += servings;
}

std::map<std::string, int> CompositeFood::getComponents() const {
    return m_components;
}

double CompositeFood::calculateTotalCalories(const FoodDatabase& database) const {
    double totalCalories = 0;
    for (const auto& [foodId, servings] : m_components) {
        auto food = database.findFood(foodId);
        if (food) {
            totalCalories += food->getCaloriesPerServing() * servings;
        }
    }
    return totalCalories;
}

nlohmann::json CompositeFood::toJson() const {
    auto baseJson = Food::toJson();
    std::map<std::string, int> components = m_components;
    baseJson["components"] = components;
    return baseJson;
}

CompositeFood CompositeFood::fromJson(const nlohmann::json& j) {
    CompositeFood compositeFood(
        j.at("identifier").get<std::string>(),
        j.at("keywords").get<std::vector<std::string>>()
    );
    
    if (j.contains("components")) {
        auto components = j.at("components").get<std::map<std::string, int>>();
        for (const auto& [foodId, servings] : components) {
            compositeFood.addComponent(foodId, servings);
        }
    }
    
    return compositeFood;
}