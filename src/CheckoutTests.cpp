#include <gtest/gtest.h>
#include "Item.hpp"

TEST(ItemTests, getItemName)
{
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ASSERT_EQ("Chips", chip.getName());
}

TEST(ItemTests, getSetPrice)
{
    Item chip("Chips", Item::Sale_t::Unit, -3);
    ASSERT_FLOAT_EQ(3, chip.getPrice());

    // Invalid price doesnt change item price
    ASSERT_FALSE(chip.setPrice(-1));
    ASSERT_FLOAT_EQ(3, chip.getPrice());

    // Valid price change
    ASSERT_TRUE(chip.setPrice(2.5));
    ASSERT_FLOAT_EQ(2.5, chip.getPrice());
}