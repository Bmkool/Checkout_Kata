#include <gtest/gtest.h>
#include <optional>

#include "Item.hpp"
#include "ItemDatabase.hpp"
#include "Order.hpp"
#include "Special.hpp"

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

TEST(SpecialTests, BuyOneGetOneFreeUnitInvalidPercent) {
    int numNeeded = 2;
    int numReceived = 1;
    float percentOff = 101; // Over 100%
    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    int numItems = 3;
    int price = 1;

    float total = sp->calcPrice(numItems, price);
    ASSERT_FLOAT_EQ(numItems * price, total);
    delete sp;

    percentOff = -10; // Negative percent
    sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    total = sp->calcPrice(numItems, price);
    ASSERT_FLOAT_EQ(numItems * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeUnitNotEnough) {
    int numNeeded = 2;
    int numReceived = 1;
    float percentOff = 100;
    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    int numItems = 2;
    int price = 1;

    float total = sp->calcPrice(numItems, price);
    ASSERT_FLOAT_EQ(numItems * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeUnitExactAmount) {
    int numNeeded = 2;
    int numReceived = 1;
    float percentOff = 100;
    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    int numItems = 3;
    int price = 1;

    float total = sp->calcPrice(numItems, price);
    // One special received
    ASSERT_FLOAT_EQ((numItems - 1) * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeUnitExtraAmount) {
    int numNeeded = 2;
    int numReceived = 1;
    float percentOff = 100;
    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    int numItems = 4;
    int price = 1;

    float total = sp->calcPrice(numItems, price);
    // One special received
    ASSERT_FLOAT_EQ((numItems - 1) * price, total);
    delete sp;
}

TEST(SpecialTests, BuyOneGetOneFreeUnitExtraAmountMultipleSpecials) {
    int numNeeded = 2;
    int numReceived = 1;
    float percentOff = 100;
    Special *sp = new BuyOneGetOneUnit(numNeeded, numReceived, percentOff);
    int numItems = 7;
    int price = 1;

    float total = sp->calcPrice(numItems, price);
    // Two specials received
    ASSERT_FLOAT_EQ((numItems - 2) * price, total);
    delete sp;
}