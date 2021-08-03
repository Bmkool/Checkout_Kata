#ifndef __SPECIAL_HPP__
#define __SPECIAL_HPP__

// Special abstract base class
class Special {
public:
    // Constructor
    Special() {} // Default constructor
    virtual ~Special() {}
    // Returns total price of the items after the special. If price/num items is negative
    // absolute value of price is used
    virtual float calcPrice(float numItems, float price) const = 0;
};

// BOGO X% off for items sold in whole units
class BuyOneGetOneUnit : public Special {
public:
    // Constructor. PercentOff must be between [0, 100] else a default of 0% off is used.
    BuyOneGetOneUnit(unsigned int needed, unsigned int receive, float percent);
    float calcPrice(float numItems, float price) const override;

private:
    unsigned int mNeeded;   // Number of items needed to receive the special
    unsigned int mReceive;  // How many items receive the discount
    float mPercentOff;  // Percentage off of base price (as decimal [0,1])
};

// BOGO X% off for items sold in weight units
class BuyOneGetOneWeight : public Special {
public:
    // Constructor. PercentOff must be between [0, 100] else a default of 0% off is used.
    // If needed or receive are negative the absolutt
    BuyOneGetOneWeight(float needed, float receive, float percent);
    float calcPrice(float numItems, float price) const override;

private:
    float mNeeded;   // Weight of items needed to receive the special
    float mReceive;  // How much weight to receive the discount
    float mPercentOff;  // Percentage off of base price (as decimal [0,1])
};

#endif