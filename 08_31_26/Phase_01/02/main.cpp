// TO DO
/*
 * ============================================================
 *  PHASE 1 — PROBLEM 2 (easy-medium)
 *  SPEC — build Product and Catalog from scratch.
 * ============================================================
 *
 *   struct Product — holds:
 *              string name
 *              double price
 *            (main.cpp brace-inits it like {"Pen", 1.5} — struct or
 *            class, your choice, as long as name/price are accessible
 *            in that order.)
 *
 *   class Catalog
 *     private:
 *       Product* items;   // heap array, malloc/calloc/realloc only
 *       int      capacity;
 *       int      size;
 *
 *     public:
 *       Catalog(int initialCapacity);
 *       ~Catalog();
 *
 *       bool add(Product p);
 *         // Always appends (no sorting). If size == capacity, grow by
 *         // GROWTH = 4 first. Always returns true.
 *
 *       void display() const;
 *         // one line per product: "name: $price" with price to
 *         // EXACTLY 2 decimal places (round, don't truncate) —
 *         // e.g. "Notebook: $12.76". Use <iomanip>'s fixed/setprecision.
 *
 *       int    getSize() const;
 *       int    getCapacity() const;
 *       bool   isEmpty() const;
 *       bool   contains(string name) const;  // case-SENSITIVE exact match
 *
 *       double getTotalValue() const;    // sum of all prices, un-rounded
 *       double getAveragePrice() const;  // must NOT truncate. Assume non-empty.
 *
 * Header should be named Catalog.hpp (or split Product.hpp — your call).
 * ============================================================
 */

#include "Catalog.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    cout << boolalpha;

    // ================= SECTION A: growth + rounding on display =================
    Catalog shop(2); // room for 2

    cout << "isEmpty (new): " << shop.isEmpty() << endl; // Expected: isEmpty (new): true
    shop.display(); // Expected: (blank line)

    shop.add({"Pen", 1.5});
    shop.add({"Notebook", 12.756});
    shop.display();
    // Expected:
    // Pen: $1.50
    // Notebook: $12.76
    // ^ 12.756 rounds UP to 12.76 — if you got 12.75, you truncated
    //   instead of rounding.
    cout << "capacity: " << shop.getCapacity() << endl; // Expected: capacity: 2

    // size == capacity now — next add() must grow first.
    shop.add({"Eraser", 0.999});
    shop.display();
    // Expected:
    // Pen: $1.50
    // Notebook: $12.76
    // Eraser: $1.00
    // ^ 0.999 rounds UP to a full dollar — a classic "rounds past the
    //   decimal boundary" trap.
    cout << "capacity: " << shop.getCapacity() << endl; // Expected: capacity: 6

    // cout << "contains(Notebook): " << shop.contains("Notebook") << endl; // Expected: true
    // cout << "contains(notebook): " << shop.contains("notebook") << endl; // Expected: false (case-sensitive)
    // cout << "contains(Marker): " << shop.contains("Marker") << endl;     // Expected: false

    // // ================= SECTION B: totals =================
    // shop.add({"Stapler", 8.2});
    // shop.add({"Glue", 3.333});
    // shop.display();
    // // Expected:
    // // Pen: $1.50
    // // Notebook: $12.76
    // // Eraser: $1.00
    // // Stapler: $8.20
    // // Glue: $3.33
    // cout << "size: " << shop.getSize() << endl;         // Expected: size: 5
    // cout << "capacity: " << shop.getCapacity() << endl; // Expected: capacity: 6
    // // ^ no grow needed for these last two — 6 has room for 5.

    // cout << fixed << setprecision(2);
    // cout << "total value: $" << shop.getTotalValue() << endl;    // Expected: total value: $26.79
    // cout << "average price: $" << shop.getAveragePrice() << endl; // Expected: average price: $5.36
    // // Raw sum is 26.788 (rounds to 26.79) and average is 26.788/5 = 5.3576
    // // (rounds to 5.36). If your getTotalValue/getAveragePrice do any
    // // internal rounding themselves instead of returning the raw double,
    // // double-rounding could shift these by a cent — return the raw
    // // value and let cout's setprecision do the rounding here.

    return 0;
}