#include "Item.hpp"

Item::Item(std::string name, Sale_t type, float price) :
    mName(name), mType(type), mPrice(price)
{
}

const std::string& Item::getName() const {
    return mName;
}