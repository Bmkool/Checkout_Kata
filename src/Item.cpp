#include "Item.hpp"
#include "Special.hpp"

#include <cmath>
#include <iostream>

Item::Item(std::string name, Sale_t type, float price) :
    mName(name), mType(type), mPrice(fabs(price)), mMarkdown(0), mSpecial(nullptr)
{}

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
        std::cerr << "Price cannot be negative" << std::endl;
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
        std::cerr << "Markdown cannot be negative or greater than original price" << std::endl;
        return false;
    }

    mMarkdown = newMarkdown;
    return true;
}

void Item::setSpecial(std::shared_ptr<Special> special) {
    if (!special) {
        mSpecial.reset(); // If nullptr then remove special
    }
    mSpecial = special;
}

const Special* Item::getSpecial() const {
    return mSpecial.get();
}
