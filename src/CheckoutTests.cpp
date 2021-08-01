#include <gtest/gtest.h>

#include "Item.hpp"
#include "ItemDatabase.hpp"

TEST(ItemTests, GetItemName) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ASSERT_EQ("Chips", chip.getName());
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