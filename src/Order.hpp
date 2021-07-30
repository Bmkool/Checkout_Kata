#ifndef __ORDER_HPP__
#define __ORDER_HPP__


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
 *  - SetPrice(Name, NewPrice)
 *  - SetMarkdown(Name, Discount)
 *  - SetSpecial(Name, Special)
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
#endif