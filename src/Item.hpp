#ifndef __ITEM_HPP__
#define __ITEM_HPP__

#include "Special.hpp"

#include <memory>
#include <string>

class Item
{
public:
    // Describes if item is sold per individual unit or by weight in $/lb
    enum class Sale_t { Unit, Weight };

    // Constructor. Price should be positive, if not the absolute value will be used.
    Item(const std::string& name, Sale_t type, float price);

    // Return name of item
    const std::string& getName() const;

    // Return Sale type of Item
    Sale_t getSaleType() const;

    // Return price of item
    float getPrice() const;

    // Set price of item. New price cannot be negative. Returns success of operation
    bool setPrice(float newPrice);

    // Return markdown of item
    float getMarkdown() const;

    // Set markdown of item. New markdown cannot be negative or greater than base price. Returns success of operation
    bool setMarkdown(float newMarkdown);

    // Set new special or nullptr to remove. Returns success of operation
    void setSpecial(const std::shared_ptr<Special>& special);

    // Returns raw pointer to current special or nullptr if none
    const Special* getSpecial() const;

private:
    std::string mName; // Name of item
    Sale_t mType;   // Sale type
    float mPrice;   // Price in dollars per unit or per pound
    float mMarkdown;    // Amount in dollars to lower price
    std::shared_ptr<Special> mSpecial; // Special if available
};

#endif