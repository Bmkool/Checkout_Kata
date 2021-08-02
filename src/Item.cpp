#include "Item.hpp"

#include <cmath>

Item::Item(std::string name, Sale_t type, float price) :
    mName(name), mType(type), mPrice(fabs(price)), mMarkdown(0) {}

const std::string& Item::getName() const {
    return mName;
}

Item::Sale_t Item::getSaleType() const {
    return mType;
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

float Item::getMarkdown() const {
    return mMarkdown;
}

bool Item::setMarkdown(float newMarkdown) {
    if (newMarkdown < 0 || newMarkdown > mPrice) {
        return false;
    }

    mMarkdown = newMarkdown;
    return true;
}