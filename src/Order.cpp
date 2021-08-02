#include "Order.hpp"

Order::Order(const ItemDatabase& db) :
    mDatabase(db),
    mTotalPrice(0),
    mCart{}
{}

bool Order::ScanItem(const std::string& name) {
    // Item must be in database
    if (std::nullopt == mDatabase.getItem(name)) {
        return false;
    }

}