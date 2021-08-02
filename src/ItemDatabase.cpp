#include "ItemDatabase.hpp"

#include <algorithm>

std::optional<Item> ItemDatabase::getItem(const std::string& name) const {
    auto it = std::find_if(mItems.begin(), mItems.end(), [&name](const Item& obj) { return obj.getName() == name; });
    if (it == mItems.end()) {
        return std::nullopt;
    }
    return *it;
}

bool ItemDatabase::insertItem(const Item& item) {
    // Check to make sure item isn't already in database
    if (getItem(item.getName()).has_value()) {
        return false;
    }

    mItems.push_back(item);
    return true;
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

