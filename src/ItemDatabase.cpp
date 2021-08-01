#include "ItemDatabase.hpp"

#include <algorithm>

bool ItemDatabase::insertItem(Item item) {
    // Check to make sure item isn't already in database
    auto it = std::find_if(mItems.begin(), mItems.end(), [&item](const Item& obj) { return obj.getName() == item.getName(); });
    if (mItems.end() != it) {
        return false;
    }

    mItems.push_back(item);
    return true;
}

std::optional<float> ItemDatabase::getItemPrice(const std::string& name) const{
    // Find item in database
    auto item = std::find_if(mItems.begin(), mItems.end(), [&name](const Item& obj) { return obj.getName() == name; });
    if (item == mItems.end()) {
        // Item not in database
        return std::nullopt;
    }
    return item->getPrice();
}

bool ItemDatabase::setItemPrice(const std::string& name, float price) {
    // Find item in database
    auto item = std::find_if(mItems.begin(), mItems.end(), [&name](const Item& obj) { return obj.getName() == name; });
    if (item == mItems.end()) {
        // Item not in database
        return false;
    }

    return item->setPrice(price);
}

