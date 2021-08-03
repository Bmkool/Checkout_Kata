#include <gtest/gtest.h>
#include <optional>
#include <cmath>

#include "../src/Item.hpp"
#include "../src/ItemDatabase.hpp"
#include "../src/Order.hpp"
#include "../src/Special.hpp"

TEST(ItemTests, GetItemName) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ASSERT_EQ("Chips", chip.getName());
}


TEST(ItemTests, GetItemSaleType) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ASSERT_EQ(Item::Sale_t::Unit, chip.getSaleType());
}

TEST(ItemTests, GetSetPrice) {
    // Negative price gets set to absolute value
    Item chip("Chips", Item::Sale_t::Unit, -3);
    ASSERT_FLOAT_EQ(3, chip.getPrice());

    // Invalid price doesnt change item price
    ASSERT_FALSE(chip.setPrice(-1));
    ASSERT_FLOAT_EQ(3, chip.getPrice());

    // Valid price change
    ASSERT_TRUE(chip.setPrice(2.5));
    ASSERT_FLOAT_EQ(2.5, chip.getPrice());
}

TEST(ItemTests, InvalidMarkdown) {
    // Negative price gets set to absolute value
    Item chip("Chips", Item::Sale_t::Unit, -3);
    ASSERT_FLOAT_EQ(0, chip.getMarkdown());

    // Negative markdown doesnt change current markdown
    ASSERT_FALSE(chip.setMarkdown(-1));
    ASSERT_FLOAT_EQ(0, chip.getMarkdown());

    // Markdown cant be greater than current price
    ASSERT_FALSE(chip.setMarkdown(chip.getPrice() + .01));
    ASSERT_FLOAT_EQ(0, chip.getMarkdown());
}

TEST(ItemTests, ValidMarkdown) {
    // Negative price gets set to absolute value
    Item chip("Chips", Item::Sale_t::Unit, 3);

    ASSERT_TRUE(chip.setMarkdown(.5));
    ASSERT_FLOAT_EQ(.5, chip.getMarkdown());

    // Markdown doesnt change base price
    ASSERT_EQ(3, chip.getPrice());
}

TEST(DatabaseTests, InsertIntoDatabase) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;
    ASSERT_TRUE(db.insertItem(chip));
}

TEST(DatabaseTests, InsertSameItemIntoDatabaseFail) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;
    ASSERT_TRUE(db.insertItem(chip));
    ASSERT_FALSE(db.insertItem(chip));

}

TEST(DatabaseTests, GetItemNotInDatabase) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    ASSERT_FALSE(db.getItem("Chips").has_value());
}

TEST(DatabaseTests, GetItemSuccess) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    ASSERT_TRUE(db.insertItem(chip));
    ASSERT_TRUE(db.getItem("Chips").has_value());
}

TEST(DatabaseTests, SetItemPriceNotInDatabase) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_FALSE(db.setItemPrice("Chips", 2));
}

TEST(DatabaseTests, SetItemPriceSuccess) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    ASSERT_TRUE(db.insertItem(chip));
    ASSERT_TRUE(db.setItemPrice("Chips", 2.5));
    ASSERT_FLOAT_EQ(2.5, db.getItem("Chips")->getPrice());
}

TEST(DatabaseTests, SetItemMarkdownNotInDatabase) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_FALSE(db.setItemMarkdown("Chips", 2));
}

TEST(DatabaseTests, SetItemMarkdownSuccess) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    ASSERT_TRUE(db.insertItem(chip));
    ASSERT_TRUE(db.setItemMarkdown("Chips", 2.5));
    ASSERT_FLOAT_EQ(2.5, db.getItem("Chips")->getMarkdown());
}

TEST(OrderTests, ScanItemUnitNotInDatabase) {
    ItemDatabase db;
    Order ord(db);

    ASSERT_FALSE(ord.ScanItem("Chips"));
    ASSERT_FLOAT_EQ(0, ord.getTotalPrice());
}

TEST(OrderTests, ScanItemUnitWrongType) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.49});
    Order ord(db);

    ASSERT_FALSE(ord.ScanItem("Apple"));
}

// Use Case #1
TEST(OrderTests, ScanItemUnitAddToCart) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_FLOAT_EQ(3, ord.getTotalPrice());
}

TEST(OrderTests, ScanItemUnitAddToCartTwice) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_FLOAT_EQ(3*2, ord.getTotalPrice());
}

TEST(OrderTests, ScanItemWeightNotInDatabase) {
    ItemDatabase db;
    Order ord(db);

    ASSERT_FALSE(ord.ScanItem("Apple", 1.0f));
    ASSERT_FLOAT_EQ(0, ord.getTotalPrice());
}

TEST(OrderTests, ScanItemWeightInvalid) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    db.insertItem({"Orange", Item::Sale_t::Weight, 2});
    Order ord(db);

    ASSERT_FALSE(ord.ScanItem("Apple", 0.0f));
    ASSERT_FALSE(ord.ScanItem("Orange", -.1f));
}

TEST(OrderTests, ScanItemWeightWrongType) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    Order ord(db);

    ASSERT_FALSE(ord.ScanItem("Chips", 1.0f));
}

// Use Case #2
TEST(OrderTests, ScanItemWeightAddToCart) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Apple", .5f));
    ASSERT_FLOAT_EQ(1.5 * .5, ord.getTotalPrice());
}

TEST(OrderTests, ScanItemWightAddToCartTwice) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Apple", .5f));
    ASSERT_TRUE(ord.ScanItem("Apple", .25f));
    ASSERT_FLOAT_EQ(1.5 * (.5 + .25), ord.getTotalPrice());;
}

// Use Case #3a
TEST(OrderTests, ScanItemUnitMarkdown) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    db.setItemMarkdown("Chips", .5);
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_FLOAT_EQ(3-.5, ord.getTotalPrice());
}

// Use Case #3b
TEST(OrderTests, ScanItemWeightMarkdown) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    db.setItemMarkdown("Apple", .25);
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Apple", .5f));
    ASSERT_FLOAT_EQ((1.5 - .25) * .5, ord.getTotalPrice());
}

// Use Case #7
TEST(OrderTests, RemoveItemUnitNotInOrder) {
    ItemDatabase db;
    Order ord(db);

    ASSERT_FALSE(ord.RemoveItem("Chips", 1U));
}

TEST(OrderTests, RemoveItemUnitWrongType) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    Order ord(db);
    ASSERT_TRUE(ord.ScanItem("Apple", .5));

    ASSERT_FALSE(ord.RemoveItem("Apple", 1U));
}

TEST(OrderTests, RemoveItemUnitInvalidQty) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    Order ord(db);
    ASSERT_TRUE(ord.ScanItem("Chips"));

    ASSERT_FALSE(ord.RemoveItem("Chips", 0U));
}

TEST(OrderTests, RemoveItemUnitSuccessfulNoMoreInOrder) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_FLOAT_EQ(3, ord.getTotalPrice());

    ASSERT_TRUE(ord.RemoveItem("Chips", 1U));
    ASSERT_FLOAT_EQ(0, ord.getTotalPrice());

    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_TRUE(ord.RemoveItem("Chips", 2U));
}

TEST(OrderTests, RemoveItemUnitSuccessfulStillOneRemainingInOrder) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_FLOAT_EQ(3*2, ord.getTotalPrice());

    ASSERT_TRUE(ord.RemoveItem("Chips", 1U));
    ASSERT_FLOAT_EQ(3, ord.getTotalPrice());
}

TEST(OrderTests, RemoveItemWeightNotInOrder) {
    ItemDatabase db;
    Order ord(db);

    ASSERT_FALSE(ord.RemoveItem("Apple", 1.5f));
}

TEST(OrderTests, RemoveItemWeightWrongType) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    Order ord(db);
    ASSERT_TRUE(ord.ScanItem("Chips"));

    ASSERT_FALSE(ord.RemoveItem("Chips", 1.0f));
}

TEST(OrderTests, RemoveItemWeightInvalidWeight) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    Order ord(db);
    ASSERT_TRUE(ord.ScanItem("Apple", 2.0f));

    ASSERT_FALSE(ord.RemoveItem("Chips", 0.0f));
    ASSERT_FALSE(ord.RemoveItem("Chips", -0.1f));
}

TEST(OrderTests, RemoveItemWeightSuccessfulNoMoreInOrder) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Apple", 2.0f));
    ASSERT_FLOAT_EQ(1.5*2.0, ord.getTotalPrice());

    ASSERT_TRUE(ord.RemoveItem("Apple", 2.0f));
    ASSERT_FLOAT_EQ(0, ord.getTotalPrice());

    ASSERT_TRUE(ord.ScanItem("Apple", 2.0f));
    ASSERT_TRUE(ord.RemoveItem("Apple", 2.01f));
    ASSERT_FLOAT_EQ(0, ord.getTotalPrice());

}

TEST(OrderTests, RemoveItemWeightSuccessfulStillRemainingInOrder) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Apple", 2.5f));
    ASSERT_FLOAT_EQ(1.5*2.5, ord.getTotalPrice());

    ASSERT_TRUE(ord.RemoveItem("Apple", 1.0f));
    ASSERT_FLOAT_EQ(1.5*(2.5-1.0), ord.getTotalPrice());
}

// Remove weight special

TEST(SpecialTests, BuyOneGetOneFreeUnitInvalidPercentPrice) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 101; // Over 100%
    unsigned int numItems = 3;
    float price = 1;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    float total = sp->calcPrice(numItems, price);
    ASSERT_FLOAT_EQ(numItems * price, total);
    delete sp;

    percentOff = -10; // Negative percent
    sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    total = sp->calcPrice(numItems, price);
    ASSERT_FLOAT_EQ(numItems * price, total);
    delete sp;

    price = -1.5;  // Negative price
    percentOff = 100;
    sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    total = sp->calcPrice(numItems, price);
    ASSERT_FLOAT_EQ((numItems - 1) * fabs(price), total);
    delete sp;
}

// Use case 4
TEST(SpecialTests, BuyOneGetOneFreeUnitNotEnough) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 100;
    unsigned int numItems = 2;
    float price = 1.5;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    float total = sp->calcPrice(numItems, price);
    ASSERT_FLOAT_EQ(numItems * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeUnitExactAmount) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 100;
    unsigned int numItems = 3;
    float price = 1.5;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    float total = sp->calcPrice(numItems, price);
    // One special received
    ASSERT_FLOAT_EQ((numItems - numReceived) * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeUnitExtraAmount) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 100;
    unsigned int numItems = 4;
    float price = 1.5;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    float total = sp->calcPrice(numItems, price);
    // One special received
    ASSERT_FLOAT_EQ((numItems - numReceived) * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeUnitExtraAmountMultipleSpecials) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 100;
    unsigned int numItems = 7;
    float price = 1.5;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    float total = sp->calcPrice(numItems, price);
    // Two specials received
    ASSERT_FLOAT_EQ((numItems - 2*numReceived) * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneXPercentUnitExactAmount) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 25;
    unsigned int numItems = 3;
    float price = 1.5;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    float total = sp->calcPrice(numItems, price);
    // One special received
    ASSERT_FLOAT_EQ(((numItems - numReceived) * price) + (price * (1 - percentOff/100)), total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneXPercentUnitAddedAmount) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 25;
    unsigned int numItems = 4;
    float price = 1.5;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    float total = sp->calcPrice(numItems, price);
    // One special received
    ASSERT_FLOAT_EQ(((numItems - numReceived) * price) + (price * (1 - percentOff/100)), total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeWeightInvalidPrice) {
    float weightNeeded = 1.5;
    float weightReceived = .5;
    float percentOff = 100;
    float weightItems = 2;
    float price = -1.5;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff);
    float total = sp->calcPrice(weightItems, price);
    ASSERT_FLOAT_EQ((weightItems - weightReceived) * fabs(price), total);
    delete sp;
}

// Use case 8
TEST(SpecialTests, BuyOneGetOneFreeWeightNotEnough) {
    float weightNeeded = 1.5;
    float weightReceived = .5;
    float percentOff = 100;
    float weightItems = 1;
    float price = 3.49;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff);
    float total = sp->calcPrice(weightItems, price);
    ASSERT_FLOAT_EQ(weightItems * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeWeightExactAmount) {
    float weightNeeded = 1.5;
    float weightReceived = .5;
    float percentOff = 100;
    float weightItems = weightNeeded + weightReceived;
    float price = 3.49;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff);
    float total = sp->calcPrice(weightItems, price);
    // One special received
    ASSERT_FLOAT_EQ((weightItems - weightReceived) * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeWeightLesserAmount) {
    float weightNeeded = 1.5;
    float weightReceived = .5;
    float percentOff = 100;
    float weightItems = weightNeeded + weightReceived/2;
    float price = 3.49;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff);
    float total = sp->calcPrice(weightItems, price);
    // Part of special received
    ASSERT_FLOAT_EQ((weightItems - weightReceived/2) * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeWeightExtraAmount) {
    float weightNeeded = 1.5;
    float weightReceived = .5;
    float percentOff = 100;
    float weightItems = 2.5;
    float price = 3.49;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff);
    float total = sp->calcPrice(weightItems, price);
    // One special received
    ASSERT_FLOAT_EQ((weightItems - weightReceived) * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeWeightExtraAmountMultipleSpecials) {
    float weightNeeded = 1.5;
    float weightReceived = .5;
    float percentOff = 100;
    float weightItems = weightNeeded * 2 + weightReceived * 1.5;
    float price = 3.49;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff);
    float total = sp->calcPrice(weightItems, price);
    // One special and part received
    ASSERT_FLOAT_EQ((weightItems - (weightReceived*1.5)) * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneXPercentWeightExactAmount) {
    float weightNeeded = 1.5;
    float weightReceived = .5;
    float percentOff = 25;
    float weightItems = 2;
    float price = 3.49;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff);
    float total = sp->calcPrice(weightItems, price);
    // One special received
    ASSERT_FLOAT_EQ((weightNeeded * price) + (weightReceived * price * (1-percentOff/100)), total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneXPercentWeightTwoSpecialsLesserAmount) {
    float weightNeeded = 1.5;
    float weightReceived = .5;
    float percentOff = 25;
    float weightItems = weightNeeded * 2 + weightReceived * 1.5;
    float price = 3.49;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff);
    float total = sp->calcPrice(weightItems, price);
    // One special received
    ASSERT_FLOAT_EQ((weightNeeded * 2 * price) + (weightReceived * 1.5 * price * (1-percentOff/100)), total);
    delete sp;
}

TEST(SpecialTests, NforXNotEnough) {
    unsigned int numNeeded = 3;
    float basePrice = 5.5;
    unsigned int numItems = 2;
    float discPrice = 5;

    Special *sp = new NforX(numNeeded, discPrice);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(numItems * basePrice, total);
    delete sp;
}

TEST(SpecialTests, NforXExact) {
    unsigned int numNeeded = 3;
    float basePrice = 5.5;
    unsigned int numItems = 3;
    float discPrice = 5;

    Special *sp = new NforX(numNeeded, discPrice);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(numItems * discPrice, total);
    delete sp;
}

TEST(SpecialTests, NforXExtra) {
    unsigned int numNeeded = 3;
    float basePrice = 5.5;
    unsigned int numItems = 4;
    float discPrice = 5;

    Special *sp = new NforX(numNeeded, discPrice);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(numNeeded * discPrice + (numItems - numNeeded) * basePrice, total);
    delete sp;
}

TEST(SpecialTests, NforXMultipleAndExtra) {
    unsigned int numNeeded = 3;
    float basePrice = 5.5;
    unsigned int numItems = numNeeded * 2 + 1;
    float discPrice = 5;

    Special *sp = new NforX(numNeeded, discPrice);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(numNeeded * 2 * discPrice + (numItems - numNeeded * 2) * basePrice, total);
    delete sp;
}