#ifndef __ORDER_HPP__
#define __ORDER_HPP__

#include "ItemDatabase.hpp"

#include <string>
#include <unordered_map>
#include <variant>

class Order
{
public:
    // Constructor. All items that can be added to the order must be in the ItemDatabase
    Order(const ItemDatabase& db);

    // Return total price of the order
    float getTotalPrice() const;

    // Scans item by unit into cart. Item must exist in database and
    // be sold by unit.  Returns status of operation and updates total price when successful.
    bool ScanItem(const std::string& name);

    // Scans item by weight into cart. Item must exist in database and
    // be sold by weight. Weight must be > 0. Returns status of operation and updates total price when successful.
    bool ScanItem(const std::string& name, float weight);

    // Removes item from cart by quantity and updates order total. Item must exist in order and
    // be sold by unit, and quantity must be greater than 0. If quantity is greater than current total in cart the excess will be ignored and item removed.
    // Returns status of operation and updates total price when successful.
    bool RemoveItem(const std::string& name, unsigned int qty);

    // Removes item from cart by weight and updates order total. Item must exist in order and
    // be sold by weight, and weight must greater than 0. If weight is greater than current total in cart the excess will be ignored and item removed.
    // Returns status of operation and updates total price when successful.
    bool RemoveItem(const std::string& name, float weight);

private:
    // Get the total price of the item based on amount and account for specials
    float getItemTotalPrice(const Item& item, const std::variant<unsigned int, float>& amt) const;

private:
    // Database of available items
    const ItemDatabase& mDatabase;
    // Price of order
    float mTotalPrice;
    // Items that have been scanned into the cart and the corresponding total quantity or weight per item
    std::unordered_map<std::string, std::variant<unsigned int, float>> mCart;
};

#endif