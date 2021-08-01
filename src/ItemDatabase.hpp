#ifndef __ITEMDATABASE_HPP__
#define __ITEMDATABASE_HPP__

#include "Item.hpp"

#include <vector>
#include <optional>

// Database that stores available item information
class ItemDatabase {
public:
    // Default constructor
    ItemDatabase() {};

    // Insert new item into database. Item names must be unique and not already
    // in database. Return status of operation.
    bool insertItem(Item item);

    // Return price of desired item if in database
    std::optional<float> getItemPrice(const std::string& name) const;

    // Set a new price for a desired item name. Price must be positive and item
    // must be in database
    bool setItemPrice(const std::string& name, float price);

private:
    std::vector<Item> mItems;
};

#endif