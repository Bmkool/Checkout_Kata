#ifndef __ITEMDATABASE_HPP__
#define __ITEMDATABASE_HPP__

#include "Item.hpp"

#include <vector>

// Database that stores available item information
class ItemDatabase {
public:
    // Default constructor
    ItemDatabase() {};

    // Insert new item into database. Item names must be unique and not already
    // in database. Return status of operation.
    bool insertItem(Item item);


private:
    std::vector<Item> mItems;
};

#endif