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

    // Returns copy of item information in the database if it exists
    std::optional<Item> getItem(const std::string& name) const;

    // Insert new item into database. Item names must be unique and not already
    // in database. Return status of operation.
    bool insertItem(const Item& item);

    // Set a new price for a desired item name. Price must be positive and item
    // must be in database
    bool setItemPrice(const std::string& name, float price);

private:
    std::vector<Item> mItems; // Items in database
};

#endif