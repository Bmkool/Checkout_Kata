#include "Special.hpp"

#include <iostream>
#include <cmath>

BuyOneGetOneUnit::BuyOneGetOneUnit(unsigned int needed, unsigned int receive, float percent) :
    Special(needed), mReceive(receive)
{
    if (percent < 0 || percent > 100) {
        std::cerr << "Invalid percentage. Must be between 0 and 100. Setting to 0" << std::endl;
        mPercentOff = 0;
    } else {
        mPercentOff = percent/100;
    }
}



float BuyOneGetOneUnit::calcPrice(unsigned int numItems, float price) {
    if (price < 0) {
        std::cerr << "Negative price entered. Using absolute value" << std::endl;
        price = fabs(price);
    }
    // Find how many specials are applicable
    unsigned int specials = numItems / (mNeeded + mReceive);

    // Retrieve leftover items
    numItems %= (mNeeded + mReceive);

    // Apply deal to number of applicable specials
    float total = specials * ((mNeeded * price) + mReceive * (price * (1 - mPercentOff)));

    // Add leftover items to total
    total += numItems * price;

    return total;
}