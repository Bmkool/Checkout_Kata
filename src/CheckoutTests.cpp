#include <gtest/gtest.h>
#include <optional>

#include "Item.hpp"
#include "ItemDatabase.hpp"
#include "Order.hpp"

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

TEST(DatabaseTests, SetItemPriceNotInDatabaseAndPriceInvalid) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    // Not in database
    ASSERT_FALSE(db.setItemPrice("Chips", 2));

    // Invalid price
    ASSERT_TRUE(db.insertItem(chip));
    ASSERT_FALSE(db.setItemPrice("Chips", -2));
}

TEST(DatabaseTests, SetItemPriceSuccess) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ItemDatabase db;

    ASSERT_TRUE(db.insertItem(chip));
    ASSERT_TRUE(db.setItemPrice("Chips", 2.5));
    ASSERT_FLOAT_EQ(2.5, db.getItem("Chips").value().getPrice());
}


TEST(OrderTests, ScanItemNotInDatabase) {
    ItemDatabase db;
    Order ord(db);

    ASSERT_FALSE(ord.ScanItem("Chips"));
}