#include "Order.hpp"

#include <optional>

Order::Order(const ItemDatabase& db) :
    mDatabase(db),
    mTotalPrice(0),
    mCart{}
{}

float Order::getTotalPrice() const {
    return mTotalPrice;
}

bool Order::ScanItem(const std::string& name) {
    // Item must be in database
    auto item = mDatabase.getItem(name);
    if (!item.has_value()) {
        return false;
    }

    // Item must be sold by unit
    if (Item::Sale_t::Unit != item->getSaleType()) {
        return false;
    }

    // Find current total price of item and increment quantity
    float prevPrice = 0;
    if (mCart.find(name) != mCart.end()) {
        prevPrice = getItemTotalPrice(item.value(), mCart[name]);
        mCart[name] = ++std::get<int>(mCart[name]);
    } else { // If item isnt already in cart then insert and set the amount to one
        mCart[name] = 1;
    }

    // Update overall cart total with updated total price of item.
    mTotalPrice += getItemTotalPrice(item.value(), mCart[name]) - prevPrice;

    return true;
}

bool Order::ScanItem(const std::string& name, float weight) {
    // Weight must be positive and non zero
    if (weight <= 0) {
        return false;
    }

    // Item must be in database
    auto item = mDatabase.getItem(name);
    if (!item.has_value()) {
        return false;
    }

    // Item must be sold by weight
    if (Item::Sale_t::Weight != item->getSaleType()) {
        return false;
    }

    // Find current total price of item and update weight
    float prevPrice = 0;
    if (mCart.find(name) != mCart.end()) {
        prevPrice = getItemTotalPrice(item.value(), mCart[name]);
        mCart[name] = std::get<float>(mCart[name]) + weight;
    } else { // If item isnt already in cart then insert and set the weight
        mCart[name] = weight;
    }

    // Update overall cart total with updated total price of item.
    mTotalPrice += getItemTotalPrice(item.value(), mCart[name]) - prevPrice;

    return true;
}

bool Order::RemoveItem(const std::string& name, int qty) {
    // Item must be in order
    auto cart_it = mCart.find(name);
    if (cart_it == mCart.end()) {
        return false;
    }

    // Grab item info from database
    auto item = mDatabase.getItem(name);
    if (!item.has_value()) {
        return false;
    }

    // Item must be sold by unit
    if (Item::Sale_t::Unit != item->getSaleType()) {
        return false;
    }

    // Quantity must be at least one and not greater than the current quantity in the cart
    if (qty < 1 || qty > std::get<int>(cart_it->second)) {
        return false;
    }

    return true;
}

float Order::getItemTotalPrice(const Item& item, const std::variant<int, float>& amt) const {
    if (Item::Sale_t::Unit == item.getSaleType()) {
        return (item.getPrice() - item.getMarkdown()) * std::get<int>(amt);
    } else {
        return (item.getPrice() - item.getMarkdown()) * std::get<float>(amt);
    }
}

