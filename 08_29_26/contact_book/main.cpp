// TO BE ANSWERED 
// /*
//  * ============================================================
//  *  SPEC — build these from scratch. Nothing below is provided.
//  *  (Different flavor entirely from Leaderboard/Triage — this one
//  *  isn't "sorted" at all. The challenge is collision resolution
//  *  and correct deletion, not shifting.)
//  * ============================================================
//  *
//  * You need ONE type plus the class:
//  *
//  *   Contact — holds:
//  *              string name
//  *              string phone
//  *            main.cpp constructs it with brace-init like {"Al","111"}.
//  *
//  *   ContactBook — a class with:
//  *              private: Contact table[7];   // fixed capacity, prime size
//  *              private: int     count;      // # of OCCUPIED slots, starts 0
//  *              private: <your choice of state tracking> — you need each
//  *                       slot to be in one of THREE states: EMPTY (never
//  *                       used), OCCUPIED (holds a live contact), or
//  *                       TOMBSTONE (held a contact that was since removed).
//  *                       Represent this however you like — an enum array,
//  *                       two parallel bool arrays, whatever's cleanest for
//  *                       you. The three-state distinction is the whole
//  *                       point, so don't collapse tombstone into empty.
//  *
//  *         Public methods to implement:
//  *           bool   insert(Contact c);          // THE core method
//  *           bool   find(string name, string &phoneOut) const;
//  *           bool   remove(string name);
//  *           void   display() const;    // "name:phone " for OCCUPIED
//  *                                       // slots, in TABLE order (index
//  *                                       // 0..6) — NOT insertion order
//  *           int    getSize() const;    // returns count
//  *           bool   isFull() const;
//  *           bool   isEmpty() const;
//  *           void   clear();
//  *           double loadFactor() const; // (double)count / capacity
//  *
//  *         Bonus / optional:
//  *           int    probeCount(string name) const;
//  *             // returns how many slots had to be examined to locate
//  *             // `name` (1 if found on the first try), or -1 if not
//  *             // present. Examine at most `capacity` slots total —
//  *             // if a fully-occupied table has no match anywhere, you
//  *             // must still terminate instead of looping forever.
//  *
//  * HASH FUNCTION (use exactly this, so your output is checkable):
//  *   hash(name) = (sum of the ASCII/char codes of every character
//  *                 in name) % 7
//  *
//  * insert(c) RULES — this is the actual thing being tested:
//  *   - Start probing at index = hash(c.name). If that slot is taken,
//  *     probe LINEARLY forward (index+1, wrapping with % 7 at the end)
//  *     until you find a spot.
//  *   - While probing, if you encounter a slot that's OCCUPIED and its
//  *     name already matches c.name, UPDATE that slot's phone number in
//  *     place and return true — do not insert a duplicate. This check
//  *     has to happen even if the matching slot is past a tombstone.
//  *   - Otherwise, insert into the FIRST slot you encounter along the
//  *     probe sequence that is either EMPTY or a TOMBSTONE (whichever
//  *     comes first) — mark it OCCUPIED, increment count, return true.
//  *   - If you probe all the way around the table (capacity attempts)
//  *     without finding a match, an EMPTY slot, or a TOMBSTONE, the
//  *     table is genuinely full — return false, unchanged.
//  *
//  * find(name, phoneOut) RULES:
//  *   - Probe from hash(name) exactly like insert does.
//  *   - Skip straight over TOMBSTONE slots — they don't mean "not here",
//  *     they mean "something used to be here, keep looking."
//  *   - Stop and return false the moment you hit a truly EMPTY slot —
//  *     that's the only valid "definitely not present" signal. Stopping
//  *     at a tombstone instead is the classic bug here: it causes false
//  *     negatives for anything inserted AFTER a deletion created that
//  *     tombstone.
//  *
//  * remove(name) RULES:
//  *   - Probe like find(). If located, mark that slot as a TOMBSTONE
//  *     (never EMPTY — turning it back to EMPTY would break find()'s
//  *     probe chain for anything that was inserted past it), decrement
//  *     count, return true. If not found, return false.
//  *
//  * Header should be named ContactBook.hpp (or split Contact.hpp).
//  * ============================================================
//  */

// #include "ContactBook.hpp"
// #include <iostream>
// using namespace std;

// int main()
// {
//     cout << boolalpha;

//     ContactBook book;

//     // hash("Al")=5  hash("Bo")=2  hash("Cy")=6  hash("Di")=5 (collides
//     // with Al)  hash("Ed")=1  hash("Fi")=0  hash("Gu")=6 (collides w/ Cy)
//     // Chosen on purpose to force collisions and wraparound.

//     book.insert({"Al", "111"});
//     book.insert({"Bo", "222"});
//     book.insert({"Cy", "333"});
//     book.display(); // Expected: Bo:222 Al:111 Cy:333
//     // Table order, not insertion order — Bo lives at index 2, Al at 5,
//     // Cy at 6, so that's the order display() walks past.

//     book.insert({"Di", "444"});
//     // Di hashes to 5 (Al's slot) -> probes 6 (Cy's slot) -> lands at 0.
//     book.display(); // Expected: Di:444 Bo:222 Al:111 Cy:333

//     book.insert({"Ed", "555"});
//     book.display(); // Expected: Di:444 Ed:555 Bo:222 Al:111 Cy:333

//     book.insert({"Fi", "666"});
//     book.display(); // Expected: Di:444 Ed:555 Bo:222 Fi:666 Al:111 Cy:333

//     book.insert({"Gu", "777"});
//     // Gu hashes to 6 (Cy) -> probes 0(Di),1(Ed),2(Bo),3(Fi) -> lands at 4.
//     book.display(); // Expected: Di:444 Ed:555 Bo:222 Fi:666 Gu:777 Al:111 Cy:333

//     cout << "count: " << book.getSize() << endl;   // Expected: count: 7
//     cout << "isFull: " << book.isFull() << endl;   // Expected: isFull: true

//     // Sneaky: insert on an EXISTING name must update in place, not move
//     // or duplicate anything.
//     book.insert({"Bo", "999"});
//     book.display(); // Expected: Di:444 Ed:555 Bo:999 Fi:666 Gu:777 Al:111 Cy:333
//     cout << "count after update: " << book.getSize() << endl; // Expected: 7

//     // Sneaky: table is genuinely full (no tombstones exist yet) — a
//     // brand new name must be rejected outright.
//     bool ivyAdded = book.insert({"Ivy", "000"});
//     cout << "Ivy added: " << ivyAdded << endl; // Expected: false
//     book.display(); // Expected: Di:444 Ed:555 Bo:999 Fi:666 Gu:777 Al:111 Cy:333  (unchanged)

//     // Undo the Bo update for the next stage of the test and remove Al —
//     // this creates the table's first tombstone, at Al's old slot.
//     bool removed = book.remove("Al");
//     cout << "Al removed: " << removed << endl; // Expected: true
//     book.display(); // Expected: Di:444 Ed:555 Bo:999 Fi:666 Gu:777 Cy:333

//     // Sneaky: Di sits PAST the tombstone Al left behind (Di's own hash
//     // is 5, same as Al's — Di only ended up at slot 0 because Al and Cy
//     // were occupying its natural probe path). find() must probe through
//     // the tombstone to still reach Di correctly.
//     string phone;
//     bool foundDi = book.find("Di", phone);
//     cout << "find Di -> found: " << foundDi << " phone: " << phone << endl;
//     // Expected: find Di -> found: true phone: 444

//     // Insert a brand new name that happens to hash into the tombstone's
//     // probe path — it should reuse that freed slot instead of failing.
//     bool huAdded = book.insert({"Hu", "888"});
//     cout << "Hu added: " << huAdded << endl; // Expected: true
//     book.display(); // Expected: Di:444 Ed:555 Bo:999 Fi:666 Gu:777 Hu:888 Cy:333

//     // Al is really gone now — its tombstone got overwritten by Hu.
//     bool foundAl = book.find("Al", phone);
//     cout << "find Al -> found: " << foundAl << endl; // Expected: false

//     cout << "loadFactor: " << book.loadFactor() << endl; // Expected: 1

//     // ================= BONUS (optional) =================

//     cout << "probeCount Bo: " << book.probeCount("Bo") << endl;  // Expected: 1
//     cout << "probeCount Gu: " << book.probeCount("Gu") << endl;  // Expected: 6
//     // Gu's own hash is 6, but Cy/Di/Ed/Bo/Fi were all in the way before
//     // it found its real home at slot 4 — 6 slots examined to confirm it.
//     cout << "probeCount Zzz: " << book.probeCount("Zzz") << endl; // Expected: -1

//     // ================= state reset =================

//     book.clear();
//     cout << "isEmpty after clear: " << book.isEmpty() << endl; // Expected: true
//     book.display();                                             // Expected: (blank line)

//     return 0;
// }