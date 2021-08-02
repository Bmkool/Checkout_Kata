#ifndef __SPECIAL_HPP__
#define __SPECIAL_HPP__

// Special abstract base class
class Special {
public:
    // Constructor
    Special(unsigned int needed) : mNeeded(needed) {};
    virtual ~Special() {};
    // Returns total price of the items after the special. If price is negative
    // absolute value of price is used
    virtual float calcPrice(unsigned int numItems, float price) = 0;

protected:
    unsigned int mNeeded;   // Number of items needed to receive the special
};

// BOGO X% off for items sold in whole units
class BuyOneGetOneUnit : public Special {
public:
    // Constructor. PercentOff must be between [0, 100] else a default of 0% off is used.
    BuyOneGetOneUnit(unsigned int needed, unsigned int receive, float percent);
    float calcPrice(unsigned int numItems, float price) override;

private:
    unsigned int mReceive;  // How many items receive the discount
    float mPercentOff;  // Percentage off of base price (as decimal [0,1])
};

#endif