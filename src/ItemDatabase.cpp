#include "ItemDatabase.hpp"

#include <algorithm>
#include <iostream>

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
        std::cerr << "Item already exists" << std::endl;
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
        std::cerr << "Item not found" << std::endl;
        return false;
    }

    return item->setPrice(price);
}

bool ItemDatabase::setItemMarkdown(const std::string& name, float markdown) {
    // Find item in database
    auto item = std::find_if(mItems.begin(), mItems.end(), [&name](const Item& obj) { return obj.getName() == name; });
    if (item == mItems.end()) {
        // Item not in database
        std::cerr << "Item not found" << std::endl;
        return false;
    }

    return item->setMarkdown(markdown);
}

bool ItemDatabase::setItemSpecial(const std::string& name, unsigned int needed, unsigned int receive, float percent, unsigned int limit) {
    // Find item in database
    auto item = std::find_if(mItems.begin(), mItems.end(), [&name](const Item& obj) { return obj.getName() == name; });
    if (item == mItems.end()) {
        // Item not in database
        std::cerr << "Item not found" << std::endl;
        return false;
    }

    if (Item::Sale_t::Weight == item->getSaleType()) {
        std::cerr << "Item not sold by unit" << std::endl;
        return false;
    }

    // Create the BOGO special
    item->setSpecial(std::make_shared<BuyOneGetOneUnit>(needed, receive, percent, limit));

    return true;
}