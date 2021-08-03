#ifndef __ITEMDATABASE_HPP__
#define __ITEMDATABASE_HPP__

#include "Item.hpp"

#include <vector>
#include <optional>

// Database that stores available item information
class ItemDatabase {
public:
    // Default constructor
    ItemDatabase() {}

    // Returns copy of item information in the database if it exists
    std::optional<Item> getItem(const std::string& name) const;

    // Insert new item into database. Item names must be unique and not already
    // in database. Return status of operation.
    bool insertItem(const Item& item);

    // Set a new price for a desired item name. Price must be positive and item
    // must be in database
    bool setItemPrice(const std::string& name, float price);

    // Set a new markdown for a desired item name. Markdown must be positive, less than base price
    // and item must be in database
    bool setItemMarkdown(const std::string& name, float price);

    // Set the BOGO special for Unit
    bool setItemSpecial(const std::string& name, unsigned int needed, unsigned int receive, float percent, unsigned int limit = 0);

    // Set the BOGO special for Weight
    bool setItemSpecial(const std::string& name, float needed, float receive, float percent, float limit = 0);

    // Set the NforX special
    bool setItemSpecial(const std::string& name, unsigned int needed, float price, unsigned int limit = 0);

private:
    std::vector<Item> mItems; // Items in database
};

#endif