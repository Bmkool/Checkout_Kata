#ifndef __ORDER_HPP__
#define __ORDER_HPP__

#include "ItemDatabase.hpp"

#include <string>
#include <unordered_map>
#include <variant>

class Order
{
public:
    // Constructor. All items that can be added to the order must be in the ItemDatabase
    Order(const ItemDatabase& db);

    // Return total price of the order
    float getTotalPrice() const;

    // Scans item by unit into cart and updates order total. Item must exist in database and
    // be sold by unit. Returns status of operation.
    bool ScanItem(const std::string& name);

    // Scans item by weight into cart and updates order total. Item must exist in database and
    // be sold by weight. Weight must be > 0. Returns status of operation and updates total price when successful.
    bool ScanItem(const std::string& name, float weight);

private:
    // Get the total price of the item based on amount
    float getItemTotalPrice(const Item& item, const std::variant<int, float>& amt) const;

private:
    // Database of available items
    const ItemDatabase& mDatabase;
    // Price of order
    float mTotalPrice;
    // Items that have been scanned into the cart and the corresponding total quantity or weight per item
    std::unordered_map<std::string, std::variant<int, float>> mCart;
};

#endif

/* Initial Outline
 *  Enum Sale_t { Unit, Weight }
 *
 * struct Special
 *  - buy (number of items needed for deal)
 *  - limit
 *  BOGO : Special
 *  - int get (number of items receiving disount)
 *  - percent_off
 *  NforX : Special
 *  - float price
 *
 *  Class Item(Name, Sale_t, Base Price, Markdown = 0, Special=nullptr)
 *  Members:
 *  - Name
 *  - Sale_t
 *  - Price
 *  - Markdown
 *  - Special*
 *
 * Class Database
 *  - vector<Item> database
 *  API:
 *  - InsertItemIntoDatabase(Item)
 *  - SetItemPrice(Name, NewPrice)
 *  - SetItemMarkdown(Name, Discount)
 *  - SetItemSpecial(Name, Special)
 *
 *  Class Order(const Database&):
 *  Members:
 *  - Database
 *  - Total Price
 *  - unordered_map of scanned items <Name, std::variant (int for Qty, float for Weight)>
 *  API:
 *  - ScanItem(Name)
 *  - ScanItem(Name, Weight)
 *  - RemoveItem(Name) // Removes all qty/weight from order
 *  - RemoveItem(Name, Qty)
 *  - RemoveItem(Name, Weight)
 *  - getTotal()
 *
*/