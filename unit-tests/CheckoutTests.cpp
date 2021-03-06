#include <gtest/gtest.h>
#include <optional>
#include <cmath>

#include "../src/Item.hpp"
#include "../src/ItemDatabase.hpp"
#include "../src/Order.hpp"
#include "../src/Special.hpp"

/*************************** Item Tests **************************************/

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

TEST(ItemTests, SetGetSpecial) {
    // Negative price gets set to absolute value
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ASSERT_EQ(nullptr, chip.getSpecial());

    auto sp = std::make_shared<BuyOneGetOneUnit>(5, 2, 100);
    chip.setSpecial(sp);
    ASSERT_EQ(sp.get(), chip.getSpecial());

    chip.setSpecial(nullptr);
    ASSERT_EQ(nullptr, chip.getSpecial());
}

/*************************** Database Tests **********************************/

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

TEST(DatabaseTests, SetItemSpecialUnitBOGONotInDatabase) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_FALSE(db.setItemSpecial("Chips", 3U, 2U, 20));
}

TEST(DatabaseTests, SetItemSpecialUnitBOGONotUnit) {
    Item apple("Apple", Item::Sale_t::Weight, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_TRUE(db.insertItem(apple));
    ASSERT_FALSE(db.setItemSpecial("Apple", 3U, 2U, 20));
}

TEST(DatabaseTests, SetItemSpecialUnitBOGOSuccess) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_TRUE(db.insertItem(chip));
    ASSERT_TRUE(db.setItemSpecial("Chips", 3U, 2U, 20));

    // Special exists
    ASSERT_NE(nullptr, db.getItem("Chips")->getSpecial());
}

TEST(DatabaseTests, SetItemSpecialWeightBOGONotInDatabase) {
    Item apple("Apple", Item::Sale_t::Weight, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_FALSE(db.setItemSpecial("Apple", 3.0f, 2.0f, 20));
}

TEST(DatabaseTests, SetItemSpecialWeightBOGONotUnit) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_TRUE(db.insertItem(chip));
    ASSERT_FALSE(db.setItemSpecial("Chips", 3.0f, 2.0f, 20));
}

TEST(DatabaseTests, SetItemSpecialWeightBOGOSuccess) {
    Item apple("Apple", Item::Sale_t::Weight, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_TRUE(db.insertItem(apple));
    ASSERT_TRUE(db.setItemSpecial("Apple", 3.0f, 2.0f, 20));

    // Special exists
    ASSERT_NE(nullptr, db.getItem("Apple")->getSpecial());
}

TEST(DatabaseTests, SetItemSpecialNforXNotInDatabase) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_FALSE(db.setItemSpecial("Chips", 3U, 2.5f));
}

TEST(DatabaseTests, SetItemSpecialNforXNotUnit) {
    Item apple("Apple", Item::Sale_t::Weight, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_TRUE(db.insertItem(apple));
    ASSERT_FALSE(db.setItemSpecial("Apple", 3U, 2.5f));
}

TEST(DatabaseTests, SetItemSpecialNforXSuccess) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_TRUE(db.insertItem(chip));
    ASSERT_TRUE(db.setItemSpecial("Chips", 3U, 5.0f));

    // Special exists
    ASSERT_NE(nullptr, db.getItem("Chips")->getSpecial());
}

/***************************** Order Tests ***********************************/

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

TEST(OrderTests, ScanItemUnitAddToCartSpecial) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    db.setItemSpecial("Chips", 2U, 1U, 100);
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Chips"));
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

TEST(OrderTests, ScanItemWightAddToCartSpecial) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    db.setItemSpecial("Apple", .5f, .25f, 100);
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Apple", .75f));
    ASSERT_FLOAT_EQ(1.5 * .5, ord.getTotalPrice());
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

TEST(OrderTests, AddItemSpecialRemoveItemInvalidateSpecialUnit) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    db.setItemSpecial("Chips", 2U, 1U, 100);
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_TRUE(ord.ScanItem("Chips"));
    ASSERT_FLOAT_EQ(3*2, ord.getTotalPrice()); // BOGO special reached

    ASSERT_TRUE(ord.RemoveItem("Chips", 1U)); // No more BOGO
    ASSERT_FLOAT_EQ(3*2, ord.getTotalPrice());
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

TEST(OrderTests, AddItemSpecialRemoveItemInvalidateSpecialWeight) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    db.setItemSpecial("Apple", .5f, .25f, 100);
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Apple", .75f));
    ASSERT_FLOAT_EQ(1.5 * .5, ord.getTotalPrice());

    ASSERT_TRUE(ord.RemoveItem("Apple", .3f));
    ASSERT_FLOAT_EQ(1.5 * (.75 - .3), ord.getTotalPrice());
}

/***************************** Special Tests *********************************/

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

// Use case #4
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

// Use case #8
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

// Use case #5
TEST(SpecialTests, NforXNotEnough) {
    unsigned int numNeeded = 3;
    float basePrice = 5.5;
    unsigned int numItems = 2;
    float discPrice = 10;

    Special *sp = new NforX(numNeeded, discPrice);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(numItems * basePrice, total);
    delete sp;
}

TEST(SpecialTests, NforXExact) {
    unsigned int numNeeded = 3;
    float basePrice = 5.5;
    unsigned int numItems = 3;
    float discPrice = 10;

    Special *sp = new NforX(numNeeded, discPrice);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(discPrice, total);
    delete sp;
}

TEST(SpecialTests, NforXExtra) {
    unsigned int numNeeded = 3;
    float basePrice = 5.5;
    unsigned int numItems = 4;
    float discPrice = 10;

    Special *sp = new NforX(numNeeded, discPrice);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(discPrice + (numItems - numNeeded) * basePrice, total);
    delete sp;
}

TEST(SpecialTests, NforXMultipleAndExtra) {
    unsigned int numNeeded = 3;
    float basePrice = 5.5;
    unsigned int numItems = numNeeded * 2 + 1;
    float discPrice = 10;

    Special *sp = new NforX(numNeeded, discPrice);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(2 * discPrice + (numItems - numNeeded * 2) * basePrice, total);
    delete sp;
}

// Use case #6
TEST(SpecialTests, BuyOneGetOneFreeUnitLimitUnderSpecial) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 100;
    unsigned int numItems = 2;
    float price = 1.5;
    unsigned int limit = 1;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff, limit);
    float total = sp->calcPrice(numItems, price); // Illogical, but since limit is one no items meet special qualifier so all base price
    ASSERT_FLOAT_EQ(numItems * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeUnitLimitSpecialNotReached) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 100;
    unsigned int numItems = 1;
    float price = 1.5;
    unsigned int limit = 6;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff, limit);
    float total = sp->calcPrice(numItems, price);
    ASSERT_FLOAT_EQ(numItems * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeUnitLimitEqual) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 100;
    unsigned int numItems = 6;
    float price = 1.5;
    unsigned int limit = 6;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff, limit);
    float total = sp->calcPrice(numItems, price);
    ASSERT_FLOAT_EQ((numItems-(numReceived*2)) * price, total); // 2 free items reach limit
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeUnitLimitExceeded) {
    unsigned int numNeeded = 2;
    unsigned int numReceived = 1;
    float percentOff = 100;
    unsigned int numItems = 6;
    float price = 1.5;
    unsigned int limit = 3;

    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff, limit);
    float total = sp->calcPrice(numItems, price);
    ASSERT_FLOAT_EQ((numItems-numReceived) * price, total); // Onle 1 discount allowed as limit reached
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeWeightLimitUnderSpecial) {
    float weightNeeded = 2.5;
    float weightReceived = 1.5;
    float percentOff = 100;
    float weightItems = 4;
    float price = 8.75;
    float limit = 1.5;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff, limit);
    float total = sp->calcPrice(weightItems, price); // Illogical, but since limit is 1.5 no weight meet special qualifier so all base price
    ASSERT_FLOAT_EQ(weightItems * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeWeightLimitSpecialNotReached) {
    float weightNeeded = 2.5;
    float weightReceived = 1.5;
    float percentOff = 100;
    float weightItems = 2.0;
    float price = 8.75;
    float limit = 8;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff, limit);
    float total = sp->calcPrice(weightItems, price);
    ASSERT_FLOAT_EQ(weightItems * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeWeightLimitEqual) {
    float weightNeeded = 2.5;
    float weightReceived = 1.5;
    float percentOff = 100;
    float weightItems = 8;
    float price = 8.75;
    float limit = 8;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff, limit);
    float total = sp->calcPrice(weightItems, price);
    ASSERT_FLOAT_EQ((weightItems-(weightReceived*2)) * price, total); // 2 free pounds reach limit
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeWeightLimitExceeded) {
    float weightNeeded = 2.5;
    float weightReceived = 1.5;
    float percentOff = 100;
    float weightItems = 8;
    float price = 8.75;
    float limit = 4;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff, limit);
    float total = sp->calcPrice(weightItems, price);
    ASSERT_FLOAT_EQ((weightItems-weightReceived) * price, total); // Onle 1 discount allowed as limit reached
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeWeightLimitBetweenNeededAndReceived) {
    float weightNeeded = 2.5;
    float weightReceived = 1.5;
    float percentOff = 100;
    float weightItems = 8;
    float price = 8.75;
    float limit = 3;

    Special *sp = new BuyOneGetOneWeight(weightNeeded, weightReceived, percentOff, limit);
    float total = sp->calcPrice(weightItems, price);
    ASSERT_FLOAT_EQ((weightItems-(limit-weightNeeded)) * price, total); // Only partial discount allowed as limit reached
    delete sp;
}

TEST(SpecialTests, NforXLimitUnderSpecial) {
    unsigned int numNeeded = 3;
    float basePrice = 10.4;
    unsigned int numItems = 3;
    float discPrice = 20;
    unsigned int limit = 2;

    Special *sp = new NforX(numNeeded, discPrice, limit);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(numItems * basePrice, total); // Illogical, no special
    delete sp;
}

TEST(SpecialTests, NforXLimitSpecialNotReached) {
    unsigned int numNeeded = 3;
    float basePrice = 10.4;
    unsigned int numItems = 2;
    float discPrice = 20;
    unsigned int limit = 6;

    Special *sp = new NforX(numNeeded, discPrice, limit);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(basePrice * numItems, total); // Two specials
    delete sp;
}

TEST(SpecialTests, NforXLimitEqual) {
    unsigned int numNeeded = 3;
    float basePrice = 10.4;
    unsigned int numItems = 6;
    float discPrice = 20;
    unsigned int limit = 6;

    Special *sp = new NforX(numNeeded, discPrice, limit);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ(discPrice * 2, total); // Two specials
    delete sp;
}

TEST(SpecialTests, NforXLimitExceeded) {
    unsigned int numNeeded = 3;
    float basePrice = 10.4;
    unsigned int numItems = 7;
    float discPrice = 20;
    unsigned int limit = 3;

    Special *sp = new NforX(numNeeded, discPrice, limit);
    float total = sp->calcPrice(numItems, basePrice);
    ASSERT_FLOAT_EQ((numItems - limit) * basePrice + discPrice, total); // One special
    delete sp;
}

/***************************** Integration Tess ******************************/

// Integration test with shopping cart consisting of multiple items, specials, markdown, etc
TEST(IntegrationTest, FullShoppingCartLargeOrder) {
    // Setup Database and specials
    ItemDatabase db;
    for (const auto& item : { Item{"Eggs", Item::Sale_t::Unit, 1.29},
                              Item{"Bread", Item::Sale_t::Unit, 2.99},
                              Item{"Cereal", Item::Sale_t::Unit, 2.50},
                              Item{"Soda", Item::Sale_t::Unit, 4.99},
                              Item{"Oranges", Item::Sale_t::Weight, 1.99},
                              Item{"Steak", Item::Sale_t::Weight, 12.99},
                              Item{"Turkey", Item::Sale_t::Weight, 9.49}, })
    {
        db.insertItem(item);
    }


    ASSERT_TRUE(db.setItemMarkdown("Bread", .49));              // Bread 49 cents off
    ASSERT_TRUE(db.setItemSpecial("Cereal", 1U, 1U, 100));      // Cereal BOGO free
    ASSERT_TRUE(db.setItemSpecial("Soda", 3U, 12.00f, 3U));      // Soda 3 for $12 limit three sodas (1 special)
    ASSERT_TRUE(db.setItemSpecial("Steak", 1.0f, 0.5f, 25));    // Steak Buy 1 lb get .5 lb 25% off
    ASSERT_TRUE(db.setItemMarkdown("Turkey", 1.49));
    ASSERT_TRUE(db.setItemSpecial("Turkey", 1.0f, 1.0f, 50, 4.0f));    // Turkey markdown 1.49, BOGO 50% 1lb limit of 4 lbs

    // Start order
    float exp_subtotal = 0; // Expected total of cart
    Order o(db);

    auto check_total = [&] (float diff) { exp_subtotal += diff; EXPECT_EQ(exp_subtotal, o.getTotalPrice());}; // Update and check order total

    // Eggs
    EXPECT_TRUE(o.ScanItem("Eggs"));
    check_total(1.29);  // Add egg
    EXPECT_TRUE(o.ScanItem("Eggs"));
    check_total(1.29);  // Add egg

    // Bread
    EXPECT_TRUE(o.ScanItem("Bread"));
    check_total(2.99 - .49);    // Add markdown price of bread

    // Remove one of eggs dont want anymore
    EXPECT_TRUE(o.RemoveItem("Eggs", 1U));
    check_total(-1.29);

    // Cereal 1
    EXPECT_TRUE(o.ScanItem("Cereal"));
    check_total(2.50);    // Add single cereal

    // Soda
    EXPECT_TRUE(o.ScanItem("Soda"));
    check_total(4.99);    // Add Soda single
    EXPECT_TRUE(o.ScanItem("Soda"));
    check_total(4.99);
    EXPECT_TRUE(o.ScanItem("Soda"));
    check_total(12 - (4.99*2));    // Add Soda special

    EXPECT_TRUE(o.ScanItem("Soda"));
    check_total(4.99);
    EXPECT_TRUE(o.ScanItem("Soda"));
    check_total(4.99);
    EXPECT_TRUE(o.ScanItem("Soda"));
    check_total(4.99);     // Limit hit no special

    // Cereal 2
    EXPECT_TRUE(o.ScanItem("Cereal")); // Finish BOGO deal
    check_total(0);

    // Oranges
    EXPECT_TRUE(o.ScanItem("Oranges", 1.25));
    check_total(1.99 * 1.25);

    // Steak
    EXPECT_TRUE(o.ScanItem("Steak", 2.75));
    check_total(12.99 * 2.0 + 12.99 * (.5 + .25) * (1-.25)); // 2 lbs normal, .75 of deal (25 % off)

    // Turkey
    EXPECT_TRUE(o.ScanItem("Turkey", 5.5));
    float md_price = 9.49 - 1.49;
    float tmp_turkeyprice = (md_price * 2.0) + md_price * .5 * 2.0 + md_price * 1.5; // 3.5lbs total md_price, 2 lbs of 50% off
    check_total(tmp_turkeyprice);

    // Remove Turkey accidentally weighed too much
    EXPECT_TRUE(o.RemoveItem("Turkey", 3.0f));
    float new_turkeyprice = (md_price * 1.5) +  md_price * .5 * 1.0; // 1.5 lbs of base price, 1.0 lb special price
    check_total(new_turkeyprice - tmp_turkeyprice);

    // Fun sanity printout of final order total
    printf("Order Total = $%.2f\n", o.getTotalPrice());
}