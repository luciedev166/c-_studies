/*
 * ============================================================
 *  SPEC — build these from scratch. Nothing below is provided.
 *  (Sequel to the Leaderboard exercise — same "no vector,
 *  no full sort, shift only what moves" spirit, but the shift
 *  now goes in TWO possible directions instead of one.)
 * ============================================================
 *
 * You need TWO types:
 *
 *   Entry — holds:
 *              string key
 *              int    value
 *            (struct OR class, your choice. main.cpp constructs
 *            it with brace-init like {"A", 1}.)
 *
 *   LRUCache — a class with:
 *              private: Entry cache[5];   // fixed capacity, NOT a vector
 *              private: int   size;       // starts at 0
 *
 *         cache[0] is always the MOST recently used entry.
 *         cache[size-1] is always the LEAST recently used entry.
 *
 *         Public methods to implement:
 *           bool   put(string key, int value);   // insert or update
 *           bool   get(string key, int &valueOut); // lookup + promote
 *           void   display() const;    // prints "key:value " front->back
 *           int    getSize() const;
 *           bool   isFull() const;
 *           bool   isEmpty() const;
 *           void   clear();
 *           bool   contains(string key) const;
 *
 *         Bonus / optional:
 *           Entry  getMostRecentlyUsed() const;  // cache[0]
 *           Entry  getLeastRecentlyUsed() const; // cache[size-1]
 *           bool   remove(string key);           // shift LEFT to close gap
 *
 * put(key, value) RULES:
 *   - If key already exists anywhere in the cache: update its value
 *     IN PLACE, then promote it to the front (index 0). Promotion means
 *     shifting every entry that was AHEAD of it one slot to the RIGHT
 *     to open up index 0 — do not touch anything behind its old position.
 *   - If key does not exist and the cache isn't full: insert a brand
 *     new entry at the front, shifting the whole current contents one
 *     slot to the right first.
 *   - If key does not exist and the cache IS full: the entry at the
 *     back (index size-1, the least recently used) is evicted first —
 *     you should never explicitly clear it, it's simply overwritten as
 *     part of the same rightward shift used for a normal insert.
 *   - put() always returns true. Unlike Leaderboard's add(), there is
 *     no rejection case here — something is always evicted to make room.
 *
 * get(key, valueOut) RULES:
 *   - If found: write the value into valueOut (out-param), promote that
 *     entry to the front exactly like put() does for an existing key,
 *     and return true.
 *   - If NOT found: valueOut is left untouched, nothing in the cache
 *     moves, and the function returns false. A miss must not shift
 *     anything — that's the trap: it's tempting to run your promotion
 *     loop unconditionally.
 *
 * Header should be named LRUCache.hpp (or split Entry.hpp — your call).
 * ============================================================
 */

#include "LRUCache.hpp"
#include <iostream>
using namespace std;

int main()
{
    cout << boolalpha;

    LRUCache cache;

    cache.put("A", 1);
    cache.put("B", 2);
    cache.put("C", 3);
    // insertion order was A, B, C but each new put pushes to the FRONT,
    // so the cache should read newest-first:
    cache.display(); // Expected: C:3 B:2 A:1

    int val = -1;
    bool found = cache.get("A", val);
    cout << "get A -> found: " << found << " value: " << val << endl;
    // Expected: get A -> found: true value: 1
    cache.display(); // Expected: A:1 C:3 B:2   (A promoted from the back)

    cache.put("D", 4);
    cache.put("E", 5);
    cache.display();                              // Expected: E:5 D:4 A:1 C:3 B:2
    cout << "isFull: " << cache.isFull() << endl; // Expected: isFull: true

    // Sneaky: put() on a key that's ALREADY in the full cache. This must
    // NOT evict anyone — it's an update+promote, not an insert.
    cache.put("C", 33);
    cache.display(); // Expected: C:33 E:5 D:4 A:1 B:2
    // B is still there. If your code evicted B here, you evicted before
    // checking whether the key already existed.

    // Now a genuinely NEW key while full — THIS is what should evict.
    cache.put("F", 6);
    cache.display(); // Expected: F:6 C:33 E:5 D:4 A:1
    // B(2) is gone — it was the least recently used at the moment this
    // insert happened, quietly overwritten by the shift.

    // Sneaky: a miss must change nothing at all.
    int missVal = -1;
    bool missFound = cache.get("Z", missVal);
    cout << "get Z -> found: " << missFound << " value: " << missVal << endl;
    // Expected: get Z -> found: false value: -1   (valueOut untouched)
    cache.display(); // Expected: F:6 C:33 E:5 D:4 A:1   (unchanged)

    // Promote something from near the back — proves the shift only moves
    // the slice between index 0 and the found index, nothing further back.
    found = cache.get("A", val);
    cache.display(); // Expected: A:1 F:6 C:33 E:5 D:4

    cout << "getSize: " << cache.getSize() << endl; // Expected: getSize: 5
    cout << "contains C: " << cache.contains("C") << endl; // Expected: true
    cout << "contains Z: " << cache.contains("Z") << endl; // Expected: false

    // ================= BONUS (optional) =================

    Entry mru = cache.getMostRecentlyUsed();
    Entry lru = cache.getLeastRecentlyUsed();
    cout << "MRU: " << mru.key << "  LRU: " << lru.key << endl;
    // Expected: MRU: A  LRU: D

    bool removed = cache.remove("C");
    cout << "remove C -> " << removed << endl; // Expected: true
    cache.display();                           // Expected: A:1 F:6 E:5 D:4
    cout << "getSize: " << cache.getSize() << endl; // Expected: getSize: 4

    bool removedAgain = cache.remove("Z");
    cout << "remove Z -> " << removedAgain << endl; // Expected: false

    // ================= state reset =================

    cache.clear();
    cout << "isEmpty after clear: " << cache.isEmpty() << endl; // Expected: true
    cache.display();                                            // Expected: (blank line)

    cache.put("NewGuy", 10);
    cache.put("NewGuy2", 20);
    cache.display(); // Expected: NewGuy2:20 NewGuy:10

    return 0;
}