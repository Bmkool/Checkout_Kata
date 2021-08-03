#include "Special.hpp"

#include <iostream>
#include <cmath>

void Special::checkArgs(float& amount, float& price) const {
    if (amount < 0) {
        std::cerr << "Negative amount entered. Using absolute value" << std::endl;
        amount = fabs(amount);
    }
    if (price < 0) {
        std::cerr << "Negative price entered. Using absolute value" << std::endl;
        price = fabs(price);
    }
}

BuyOneGetOneUnit::BuyOneGetOneUnit(unsigned int needed, unsigned int receive, float percent) :
    Special(), mNeeded(needed), mReceive(receive)
{
    if (percent < 0 || percent > 100) {
        std::cerr << "Invalid percentage. Must be between 0 and 100. Setting to 0" << std::endl;
        mPercentOff = 0;
    } else {
        mPercentOff = percent/100;
    }
}

float BuyOneGetOneUnit::calcPrice(float numItems, float price) const {
    checkArgs(numItems, price);

    // Convert numItems to int
    unsigned int numItemsInt = static_cast<unsigned int>(numItems);

    // Find how many specials are applicable
    unsigned int specials = numItemsInt / (mNeeded + mReceive);

    // Retrieve leftover items
    numItemsInt %= (mNeeded + mReceive);

    // Apply deal to number of applicable specials
    float total = specials * ((mNeeded * price) + mReceive * (price * (1 - mPercentOff)));

    // Add leftover items to total
    total += numItemsInt * price;

    return total;
}

BuyOneGetOneWeight::BuyOneGetOneWeight(float needed, float receive, float percent) :
    Special(), mNeeded(fabs(needed)), mReceive(fabs(receive))
{
    if (percent < 0 || percent > 100) {
        std::cerr << "Invalid percentage. Must be between 0 and 100. Setting to 0" << std::endl;
        mPercentOff = 0;
    } else {
        mPercentOff = percent/100;
    }
}

float BuyOneGetOneWeight::calcPrice(float weight, float price) const {
    checkArgs(weight, price);

    float total = 0;
    // Loop until no more applicable specials
    while (weight > mNeeded) {
        weight -= mNeeded;                                                      // Remove needed weight for special
        float d_weight = (weight < mReceive) ? weight : mReceive;               // How much weight is available for discount?
        total += (mNeeded * price) + (d_weight * price * (1 - mPercentOff));    // Calculate price total of current special
        weight -= d_weight;                                                     // Remove discounted weight from total
    }

    // Add leftover weight to total
    total += weight * price;

    return total;
}

NforX::NforX(unsigned int needed, float disc_price) :
    Special(), mNeeded(needed), mDiscPrice(disc_price)
{}

float NforX::calcPrice(float numItems, float price) const {
    checkArgs(numItems, price);

    // Convert numItems to int
    unsigned int numItemsInt = static_cast<unsigned int>(numItems);

    // Find how many specials are applicable
    unsigned int specials = numItemsInt / mNeeded;

    // Retrieve leftover items
    numItemsInt %= mNeeded;

    return  specials * mNeeded * mDiscPrice +  numItemsInt * price;
}