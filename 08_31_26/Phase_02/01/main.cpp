/*
 * ============================================================
 *  PHASE 2 — PROBLEM 1 (medium)
 *  SPEC — build Item and Cart from scratch.
 * ============================================================
 *
 *   struct Item — holds:
 *              string name
 *              double price   // price per unit
 *              int    qty
 *            (brace-init like {"Pen", 1.50, 3}.)
 *
 *   class Cart
 *     private:
 *       Item*  items;          // heap array, malloc/calloc/realloc only
 *       int    capacity;
 *       int    size;
 *       bool   discountApplied;   // starts false
 *       double discountRate;      // percent, e.g. 10 means 10%
 *
 *     public:
 *       Cart(int initialCapacity); // discountApplied = false
 *       ~Cart();
 *
 *       bool addItem(Item it);
 *         // Search existing entries by name (exact match) FIRST:
 *         //   - if a match exists, add it.qty onto that entry's qty
 *         //     (merge — do NOT create a second entry for the same name,
 *         //     and this does NOT touch capacity since nothing new is
 *         //     appended).
 *         //   - if no match, append as a new entry. Grow by GROWTH = 3
 *         //     first if size == capacity.
 *         // Always returns true either way.
 *
 *       double getSubtotal() const;       // sum of (price * qty) over all entries
 *       int    getCapacity() const;
 *       int    getItemCount() const;      // number of DISTINCT entries (== size)
 *       int    getTotalQuantity() const;  // sum of qty across all entries
 *
 *       bool applyDiscount(double percent);
 *         // ONE-TIME only. If discountApplied is already true, change
 *         // nothing and return false. Otherwise set discountApplied =
 *         // true, discountRate = percent, return true.
 *
 *       double getTotal() const;
 *         // Recompute from scratch every call — do NOT cache a stored
 *         // total. If discountApplied, return subtotal * (1 - rate/100),
 *         // else return subtotal plain. Must not truncate.
 *
 *       void display() const;
 *         // one line per entry: "name x{qty} - ${price} each", price to
 *         // exactly 2 decimals.
 *
 *       string toString() const;
 *         // EXACT format (money to 2 decimals):
 *         // "Cart: {itemCount} items ({totalQty} units), subtotal ${x},
 *         //  total ${y}, discount applied: {true|false}"
 *         // e.g.: "Cart: 2 items (5 units), subtotal $10.00, total $10.00, discount applied: false"
 *
 * Header should be named Cart.hpp (or split Item.hpp — your call).
 * ============================================================
 */

#include "Cart.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    cout << boolalpha;

    Cart cart(2);

    // ================= SECTION A: append vs merge =================
    cart.addItem({"Pen", 1.50, 3});
    cart.addItem({"Notebook", 4.00, 2});
    cart.display();
    // Expected:
    // Pen x3 - $1.50 each
    // Notebook x2 - $4.00 each
    cout << "capacity: " << cart.getCapacity() << endl; // Expected: capacity: 2

    // Same name again — this must MERGE into the existing "Pen" entry,
    // not create a second row. itemCount stays 2, capacity untouched.
    cart.addItem({"Pen", 1.50, 2});
    cart.display();
    // Expected:
    // Pen x5 - $1.50 each
    // Notebook x2 - $4.00 each
    cout << "itemCount: " << cart.getItemCount() << endl;         // Expected: itemCount: 2
    cout << "totalQuantity: " << cart.getTotalQuantity() << endl; // Expected: totalQuantity: 7
    cout << "capacity: " << cart.getCapacity() << endl;           // Expected: capacity: 2
    // ^ still 2 — a merge should never trigger a grow.

    // A genuinely new name — NOW size == capacity, so this one must grow.
    cart.addItem({"Eraser", 0.75, 4});
    cart.display();
    // Expected:
    // Pen x5 - $1.50 each
    // Notebook x2 - $4.00 each
    // Eraser x4 - $0.75 each
    cout << "capacity: " << cart.getCapacity() << endl; // Expected: capacity: 5

    // ================= SECTION B: discount is a ONE-TIME gate =================
    cout << fixed << setprecision(2);
    cout << "subtotal: $" << cart.getSubtotal() << endl; // Expected: subtotal: $18.50
    // Pen 5*1.50=7.50, Notebook 2*4.00=8.00, Eraser 4*0.75=3.00 -> 18.50

    bool applied1 = cart.applyDiscount(10);
    cout << "applyDiscount(10): " << applied1 << endl; // Expected: applyDiscount(10): true
    cout << "total: $" << cart.getTotal() << endl;      // Expected: total: $16.65
    // 18.50 * 0.9 = 16.65

    // Second call must be REJECTED — the rate from the first call sticks.
    bool applied2 = cart.applyDiscount(20);
    cout << "applyDiscount(20): " << applied2 << endl; // Expected: applyDiscount(20): false
    cout << "total: $" << cart.getTotal() << endl;      // Expected: total: $16.65 (unchanged — still 10%, not 20%)

    // ================= SECTION C: total is recomputed, never cached =================
    // Adding a new item AFTER the discount was applied must still reflect
    // in getTotal() — proves you're not just returning a stored number
    // from when applyDiscount() was first called.
    cart.addItem({"Ruler", 2.00, 1});
    cout << "subtotal: $" << cart.getSubtotal() << endl; // Expected: subtotal: $20.50
    cout << "total: $" << cart.getTotal() << endl;        // Expected: total: $18.45
    // 20.50 * 0.9 = 18.45 — the original 10% rate, applied fresh.

    cout << cart.toString() << endl;
    // Expected: Cart: 4 items (12 units), subtotal $20.50, total $18.45, discount applied: true

    return 0;
}