#include "ItemDatabase.hpp"

bool ItemDatabase::insertItem(Item item) {
    // Check to make sure item isn't already in database
    for (const auto& it : mItems) {
        if (it.getName() == item.getName()) {
            return false;
        }
    }

    mItems.push_back(item);
    return true;
}