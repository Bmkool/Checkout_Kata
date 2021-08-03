#include "Order.hpp"

#include <optional>
#include <iostream>

Order::Order(const ItemDatabase& db) :
    mDatabase(db), mTotalPrice(0), mCart{}
{}

float Order::getTotalPrice() const {
    return mTotalPrice;
}

bool Order::ScanItem(const std::string& name) {
    // Item must be in database
    auto item = mDatabase.getItem(name);
    if (!item.has_value()) {
        std::cerr << "Item not in database" << std::endl;
        return false;
    }

    // Item must be sold by unit
    if (Item::Sale_t::Unit != item->getSaleType()) {
        std::cerr << "Item not sold by unit" << std::endl;
        return false;
    }

    // Find current total price of item and increment quantity
    float prevPrice = 0;
    if (mCart.find(name) != mCart.end()) {
        prevPrice = getItemTotalPrice(item.value(), mCart[name]);
        mCart[name] = ++std::get<unsigned int>(mCart[name]);
    } else { // If item isnt already in cart then insert and set the amount to one
        mCart[name] = 1U;
    }

    // Update overall cart total with updated total price of item.
    mTotalPrice += getItemTotalPrice(item.value(), mCart[name]) - prevPrice;

    return true;
}

bool Order::ScanItem(const std::string& name, float weight) {
    // Weight must be positive and non zero
    if (weight <= 0) {
        std::cerr << "Weight must be positive and non-zero" << std::endl;
        return false;
    }

    // Item must be in database
    auto item = mDatabase.getItem(name);
    if (!item.has_value()) {
        std::cerr << "Item not in database" << std::endl;
        return false;
    }

    // Item must be sold by weight
    if (Item::Sale_t::Weight != item->getSaleType()) {
        std::cerr << "Item not sold by weight" << std::endl;
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

bool Order::RemoveItem(const std::string& name, unsigned int qty) {
    // Item must be in order
    auto cart_it = mCart.find(name);
    if (cart_it == mCart.end()) {
        std::cerr << "Item not found in order" << std::endl;
        return false;
    }

    // Grab item info from database
    auto item = mDatabase.getItem(name);
    if (!item.has_value()) {
        std::cerr << "Item not in database" << std::endl; // shouldnt be possible
        return false;
    }

    // Item must be sold by unit
    if (Item::Sale_t::Unit != item->getSaleType()) {
        std::cerr << "Item not sold by unit" << std::endl;
        return false;
    }

    // Quantity must be at least one
    unsigned int curQty = std::get<unsigned int>(cart_it->second);
    if (qty == 0) {
        std::cerr << "Removal quantity cannot be zero" << std::endl;
        return false;
    }

    // Find current total price of item
    float prevPrice = getItemTotalPrice(item.value(), cart_it->second);

    //  Update item quantity and overall cart total
    if (qty >= curQty) {
        // Remove item fully from cart
        mCart.erase(cart_it);
        mTotalPrice -= prevPrice;
    } else {
        cart_it->second = (curQty - qty);
        mTotalPrice += getItemTotalPrice(item.value(), cart_it->second) - prevPrice;
    }

    return true;
}

bool Order::RemoveItem(const std::string& name, float weight) {
    // Item must be in order
    auto cart_it = mCart.find(name);
    if (cart_it == mCart.end()) {
        std::cerr << "Item not found in order" << std::endl;
        return false;
    }

    // Grab item info from database
    auto item = mDatabase.getItem(name);
    if (!item.has_value()) {
        std::cerr << "Item not in database" << std::endl; // shouldnt be possible
        return false;
    }

    // Item must be sold by unit
    if (Item::Sale_t::Weight != item->getSaleType()) {
        std::cerr << "Item not sold by weight" << std::endl;
        return false;
    }

    // Quantity must be at least one and not greater than the current quantity in the cart
    float curWeight = std::get<float>(cart_it->second);
    if (weight <= 0) {
        std::cerr << "Removal quantity must be greater than zero" << std::endl;
        return false;
    }

    // Find current total price of item
    float prevPrice = getItemTotalPrice(item.value(), cart_it->second);

    //  Update item weight and overall cart total
    if (weight >= curWeight) {
        // Remove item fully from cart
        mCart.erase(cart_it);
        mTotalPrice -= prevPrice;
    } else {
        cart_it->second = (curWeight - weight);
        mTotalPrice += getItemTotalPrice(item.value(), cart_it->second) - prevPrice;
    }

    return true;
}

float Order::getItemTotalPrice(const Item& item, const std::variant<unsigned int, float>& amt) const {
    auto spec = item.getSpecial();
    float amount = (Item::Sale_t::Unit == item.getSaleType()) ? std::get<unsigned int>(amt) : std::get<float>(amt);
    // Use special if available otherwise calculate manually
    if (spec) {
        return spec->calcPrice(amount, item.getPrice() - item.getMarkdown());
    } else {
        return (item.getPrice() - item.getMarkdown()) * amount;
    }
}

