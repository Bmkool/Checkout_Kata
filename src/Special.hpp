#ifndef __SPECIAL_HPP__
#define __SPECIAL_HPP__

// Special abstract base class
class Special {
public:
    Special() {} // Default constructor
    virtual ~Special() {}
    // Returns total price of the items after the special. If price/num items are negative
    // absolute value will be used
    virtual float calcPrice(float numItems, float price) const = 0;

protected:
    // Correct arguments of calcPrice to ensure they are positive
    void checkArgs(float& numItems, float& price) const;
};

// BOGO X% off for items sold in whole units
class BuyOneGetOneUnit : public Special {
public:
    // Constructor. PercentOff must be between [0, 100] else a default of 0% off is used. Optional limit
    BuyOneGetOneUnit(unsigned int needed, unsigned int receive, float percent, unsigned int limit = 0);
    float calcPrice(float numItems, float price) const override;

private:
    unsigned int mNeeded;   // Number of items needed to receive the special
    unsigned int mReceive;  // How many items receive the discount
    float mPercentOff;  // Percentage off of base price (as decimal [0,1])
    unsigned int mLimit; // Limit on number of items available per special. 0 = no limit
};

// BOGO X% off for items sold in weight units
class BuyOneGetOneWeight : public Special {
public:
    // Constructor. PercentOff must be between [0, 100] else a default of 0% off is used.
    // If needed or receive are negative the absolute value will be used.
    BuyOneGetOneWeight(float needed, float receive, float percent, float limit = 0);
    float calcPrice(float numItems, float price) const override;

private:
    float mNeeded;   // Weight of items needed to receive the special
    float mReceive;  // How much weight to receive the discount
    float mPercentOff;  // Percentage off of base price (as decimal [0,1])
    float mLimit; // Limit on number of items available per special. 0 = no limit
};

class NforX : public Special {
public:
    // Constructor. If price is negative absolute value will be used.
    NforX(unsigned int needed, float price, unsigned int limit = 0);
    float calcPrice(float numItems, float price) const override;

private:
    unsigned int mNeeded; // Number of items needed to receive the special
    float mDiscPrice;    // Overall price for mNeeded items
    unsigned int mLimit; // Limit on number of items available per special. 0 = no limit
};

#endif