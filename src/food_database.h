#ifndef FOOD_DATABASE_H
#define FOOD_DATABASE_H

#include <unordered_map>
#include <optional>
#include "../json.hpp"
#include "food.h"

class FoodDatabase {
public:
    FoodDatabase();

    void loadDatabase();
    void saveDatabase();

    void addBasicFood(const Food& food);
    void addCompositeFood(const CompositeFood& food);
    void removeFood(const std::string& identifier);

    std::optional<Food> findFood(const std::string& identifier) const;
    std::vector<Food> searchFoodByKeywords(const std::vector<std::string>& keywords, bool matchAll) const;

    const std::unordered_map<std::string, Food>& getBasicFoods() const;
    const std::unordered_map<std::string, CompositeFood>& getCompositeFoods() const;

private:
    std::unordered_map<std::string, Food> m_basicFoods;
    std::unordered_map<std::string, CompositeFood> m_compositeFoods;

    void validateDuplicateFood(const std::string& identifier) const;
};

#endif // FOOD_DATABASE_H