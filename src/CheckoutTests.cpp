#include <gtest/gtest.h>
#include "Item.hpp"

TEST(ItemTests, getItemName) {
    Item chip("Chips", Item::Sale_t::Unit, 3);
    ASSERT_EQ(chip.getName(), "Chips");
}