/**
 * ============================================================
 *  PHASE 3 — PROBLEM 1 (medium-hard)
 *  SPEC — build Product and Inventory from scratch.
 * ============================================================
 *
 *   struct Product — holds:
 *                  string name
 *                  int    stock
 *                  double price
 *            (brace-init like {"Pen", 10, 25.50}.)
 *
 *
 *   class Inventory
 *
 *     IMPORTANT MEMORY RULE:
 *
 *       Inventory STARTS by using a FIXED member array.
 *
 *       Product fixedProducts[2];
 *
 *       This array is NOT heap memory.
 *       Therefore, you MUST NOT call realloc() on it.
 *
 *       Once the fixed array becomes full:
 *
 *       1. Allocate a NEW heap array using malloc().
 *       2. Copy all existing products from fixedProducts
 *          into the heap array.
 *       3. Switch the Inventory to using the heap array.
 *       4. From this point onward, future growth may use realloc().
 *
 *       The Inventory therefore has two stages:
 *
 *       STAGE 1:
 *           fixed member array is being used.
 *
 *       STAGE 2:
 *           dynamically allocated heap array is being used.
 *
 *     private:
 *
 *       Product fixedProducts[2];   // starts here
 *       Product* products;          // heap array after switching
 *
 *       int capacity;
 *       int size;
 *
 *       bool usingHeap;
 *         // starts false.
 *         // becomes true once the fixed array is converted
 *         // into heap storage.
 *
 *
 *     public:
 *
 *       Inventory();
 *         // Starts with:
 *         //
 *         // capacity = 2
 *         // size = 0
 *         // usingHeap = false
 *         //
 *         // products does not own the fixed array.
 *
 *       ~Inventory();
 *         // Only free memory if heap memory was actually allocated.
 *         //
 *         // NEVER free() fixedProducts.
 *
 *
 *       void addProduct(Product p);
 *
 *         // If size < capacity:
 *         //     simply append the product.
 *
 *         // If size == capacity AND usingHeap == false:
 *         //
 *         //     You have filled fixedProducts.
 *         //
 *         //     You CANNOT use realloc() here because
 *         //     fixedProducts is a fixed member array.
 *         //
 *         //     Instead:
 *         //
 *         //     1. malloc() a new heap array with
 *         //        capacity * 2 Product slots.
 *         //
 *         //     2. Manually copy all existing products
 *         //        from fixedProducts into the new heap array.
 *         //
 *         //     3. Store the new heap pointer in products.
 *         //
 *         //     4. Update capacity.
 *         //
 *         //     5. Set usingHeap = true.
 *         //
 *         //     6. Add the new product.
 *
 *         // If size == capacity AND usingHeap == true:
 *         //
 *         //     Grow the existing heap array using realloc().
 *         //
 *         //     New capacity = capacity * 2.
 *
 *
 *       Product getProduct(int index) const;
 *         // Assume valid index.
 *
 *         // Must correctly read from:
 *         //
 *         // fixedProducts when usingHeap == false
 *         //
 *         // products when usingHeap == true
 *
 *
 *       int getSize() const;
 *
 *       int getCapacity() const;
 *
 *       bool isUsingHeap() const;
 *
 *
 *       double getTotalValue() const;
 *         // Sum of:
 *         //
 *         // product.stock * product.price
 *         //
 *         // for every product.
 *
 *
 *       void display() const;
 *         // One line per product:
 *         //
 *         // "{name}: stock={stock}, price={price}"
 *
 *
 *   Header should be named Inventory.hpp.
 * ============================================================
 */

#include "Inventory.hpp"
#include <iostream>

using namespace std;

int main() {

    Inventory store;

    // ================= SECTION A: fixed member array =================
    //
    // The Inventory begins with a fixed member array of capacity 2.
    //
    // At this point:
    //
    // usingHeap should still be false.

    store.addProduct({"Pen", 10, 25.50});
    store.addProduct({"Notebook", 5, 60.00});

    store.display();

    // Expected:
    // Pen: stock=10, price=25.5
    // Notebook: stock=5, price=60

    cout << "size: " << store.getSize() << endl;
    // Expected: size: 2

    cout << "capacity: " << store.getCapacity() << endl;
    // Expected: capacity: 2

    cout << boolalpha;

    cout << "usingHeap: " << store.isUsingHeap() << endl;
    // Expected: usingHeap: false


    // ================= SECTION B: fixed array -> heap transition =================
    //
    // The fixed array is now completely full.
    //
    // Adding this product MUST trigger the transition
    // from fixed member storage to heap storage.
    //
    // IMPORTANT:
    //
    // You CANNOT do:
    //
    // realloc(fixedProducts, ...)
    //
    // because fixedProducts was never allocated using
    // malloc(), calloc(), or realloc().
    //
    // You must:
    //
    // malloc a new heap array,
    // copy Pen and Notebook into it,
    // then switch to using that heap array.

    store.addProduct({"Pencil", 20, 15.00});

    store.display();

    // Expected:
    // Pen: stock=10, price=25.5
    // Notebook: stock=5, price=60
    // Pencil: stock=20, price=15

    cout << "size: " << store.getSize() << endl;
    // Expected: size: 3

    cout << "capacity: " << store.getCapacity() << endl;
    // Expected: capacity: 4

    cout << "usingHeap: " << store.isUsingHeap() << endl;
    // Expected: usingHeap: true


    // ================= SECTION C: normal heap usage =================
    //
    // The Inventory is already using heap memory.
    //
    // size(3) < capacity(4),
    // so this should simply append without growing.

    store.addProduct({"Eraser", 8, 12.50});

    cout << "size: " << store.getSize() << endl;
    // Expected: size: 4

    cout << "capacity: " << store.getCapacity() << endl;
    // Expected: capacity: 4


    // ================= SECTION D: heap realloc =================
    //
    // The heap array is now full.
    //
    // This time, growth is different from Section B.
    //
    // products is already heap memory, so realloc()
    // CAN now be used.
    //
    // Capacity should grow from 4 to 8.

    store.addProduct({"Marker", 6, 35.00});

    cout << "size: " << store.getSize() << endl;
    // Expected: size: 5

    cout << "capacity: " << store.getCapacity() << endl;
    // Expected: capacity: 8

    cout << "usingHeap: " << store.isUsingHeap() << endl;
    // Expected: usingHeap: true


    // ================= SECTION E: reading after the transition =================
    //
    // These products were originally stored inside fixedProducts.
    //
    // After the transition, they should have been copied
    // correctly into the heap array.

    Product first = store.getProduct(0);
    Product second = store.getProduct(1);
    Product last = store.getProduct(4);

    cout << first.name << endl;
    // Expected: Pen

    cout << second.name << endl;
    // Expected: Notebook

    cout << last.name << endl;
    // Expected: Marker


    // ================= SECTION F: aggregate calculation =================
    //
    // Calculate:
    //
    // Pen:      10 * 25.50 = 255
    // Notebook:  5 * 60.00 = 300
    // Pencil:   20 * 15.00 = 300
    // Eraser:    8 * 12.50 = 100
    // Marker:    6 * 35.00 = 210
    //
    // Total = 1165

    cout << "total value: " << store.getTotalValue() << endl;
    // Expected: total value: 1165


    // ================= SECTION G: final display =================

    store.display();

    // Expected:
    // Pen: stock=10, price=25.5
    // Notebook: stock=5, price=60
    // Pencil: stock=20, price=15
    // Eraser: stock=8, price=12.5
    // Marker: stock=6, price=35


    return 0;
}