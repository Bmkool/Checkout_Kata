#include "Item.hpp"

#include <cmath>

Item::Item(std::string name, Sale_t type, float price) :
    mName(name), mType(type), mPrice(fabs(price))
{
}

const std::string& Item::getName() const {
    return mName;
}

float Item::getPrice() const {
    return mPrice;
}

bool Item::setPrice(float newPrice) {
    if (newPrice < 0) {
        return false;
    }

    mPrice = newPrice;
    return true;
}