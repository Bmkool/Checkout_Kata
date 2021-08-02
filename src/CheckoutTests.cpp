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
    ASSERT_FLOAT_EQ(2.5, db.getItem("Chips").value().getPrice());
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
    ASSERT_FLOAT_EQ(2.5, db.getItem("Chips").value().getMarkdown());
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

    ASSERT_FALSE(ord.ScanItem("Apple", 1.0));
    ASSERT_FLOAT_EQ(0, ord.getTotalPrice());
}

TEST(OrderTests, ScanItemWeightInvalid) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    db.insertItem({"Orange", Item::Sale_t::Weight, 2});
    Order ord(db);

    ASSERT_FALSE(ord.ScanItem("Apple", 0));
    ASSERT_FALSE(ord.ScanItem("Orange", -.1));
}

TEST(OrderTests, ScanItemWeightWrongType) {
    ItemDatabase db;
    db.insertItem({"Chips", Item::Sale_t::Unit, 3});
    Order ord(db);

    ASSERT_FALSE(ord.ScanItem("Chips", 1.0));
}

// Use Case #2
TEST(OrderTests, ScanItemWeightAddToCart) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Apple", .5));
    ASSERT_FLOAT_EQ(1.5 * .5, ord.getTotalPrice());
}

TEST(OrderTests, ScanItemWightAddToCartTwice) {
    ItemDatabase db;
    db.insertItem({"Apple", Item::Sale_t::Weight, 1.5});
    Order ord(db);

    ASSERT_TRUE(ord.ScanItem("Apple", .5));
    ASSERT_TRUE(ord.ScanItem("Apple", .25));
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

    ASSERT_TRUE(ord.ScanItem("Apple", .5));
    ASSERT_FLOAT_EQ((1.5 - .25) * .5, ord.getTotalPrice());
}