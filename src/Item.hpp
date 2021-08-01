#ifndef __ITEM_HPP__
#define __ITEM_HPP__

#include <memory>
#include <string>

class Item
{
public:
    // Describes if item is sold per individual unit or by weight in $/lb
    enum class Sale_t { Unit, Weight };

    // Constructor
    Item(std::string name, Sale_t type, float price);

    // Return name of item
    const std::string& getName() const;

private:
    std::string mName; // Name of item
    Sale_t mType;   // Sale type
    float mPrice;   // Price in dollars per unit or per pound
};

#endif