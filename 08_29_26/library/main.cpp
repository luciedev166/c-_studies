// TO BE ANSWERED
// /*
//  * ============================================================
//  *  SPEC — build these from scratch. Nothing below is provided.
//  *  (The odd one out — the hard part here isn't shifting math,
//  *  it's a class whose methods reach into a SECOND, nested fixed
//  *  array and mutate it as a side effect.)
//  * ============================================================
//  *
//  * You need TWO types:
//  *
//  *   Book — holds:
//  *              string title
//  *              string borrower     // "" means the book is available
//  *              int    dueDay       // only meaningful while borrowed
//  *              string waitlist[3]  // fixed capacity, FIFO
//  *              int    waitlistSize // starts at 0
//  *            main.cpp constructs a NEW book with brace-init giving only
//  *            the title, e.g. {"Dune"} — everything else must default
//  *            (borrower to "", dueDay to 0, waitlistSize to 0).
//  *
//  *   Library — a class with:
//  *              private: Book catalog[6];  // fixed capacity, NOT a vector
//  *              private: int  size;        // starts at 0
//  *
//  *         catalog is kept sorted ALPHABETICALLY by title at all times.
//  *
//  *         Public methods to implement:
//  *           bool   addBook(Book b);
//  *           bool   checkout(string title, string borrower, int dueDay);
//  *           bool   joinWaitlist(string title, string borrower);
//  *           bool   returnBook(string title); // THE core method
//  *           void   display() const;
//  *           int    getSize() const;
//  *           bool   isFull() const;
//  *           bool   isEmpty() const;
//  *           void   clear();
//  *           double getAverageDueDay() const; // over BORROWED books only
//  *
//  *         Bonus / optional:
//  *           bool   contains(string title) const;
//  *           int    indexOf(string title) const;
//  *           bool   removeBook(string title);
//  *
//  * addBook(b) RULES:
//  *   - Reject (return false, unchanged) if a book with that exact title
//  *     already exists — no duplicate titles allowed.
//  *   - Reject if the catalog is already full.
//  *   - Otherwise insert it at its correct alphabetically-sorted position,
//  *     shifting only what needs to move (same style as Leaderboard's
//  *     add() — one pass, direct assignment, no full sort).
//  *
//  * checkout(title, borrower, dueDay) RULES:
//  *   - Only succeeds if the book exists AND is currently available
//  *     (borrower == ""). Sets its borrower and dueDay, returns true.
//  *   - If the book doesn't exist, or is already checked out to someone
//  *     else, return false and change nothing. (Trying to check out a
//  *     book that's already out is NOT the same as joining its waitlist —
//  *     that's a separate, deliberate call. checkout() should not
//  *     silently redirect into joinWaitlist().)
//  *
//  * joinWaitlist(title, borrower) RULES:
//  *   - Only valid if the book exists, is currently checked out (not
//  *     available), the waitlist isn't full (max 3), the borrower isn't
//  *     already on the waitlist, and the borrower isn't the person who
//  *     currently has the book. Any violation -> return false, unchanged.
//  *   - Otherwise append the name to the END of that book's waitlist
//  *     (simple FIFO append — no shifting needed here).
//  *
//  * returnBook(title) RULES — the whole point of this exercise:
//  *   - If the book doesn't exist or isn't currently borrowed, return
//  *     false, unchanged.
//  *   - Clear borrower back to "" and dueDay back to 0.
//  *   - THEN: if that book's waitlist is non-empty, automatically hand
//  *     the book to whoever is first in line — read waitlist[0] into
//  *     the book's borrower field, assign them a fresh dueDay of 14,
//  *     and shift the REST of the waitlist left by one slot to close
//  *     the gap (same direction as Leaderboard's removeByName, just on
//  *     a nested array instead of the top-level one). Decrement
//  *     waitlistSize.
//  *   - Return true either way (the return itself always succeeds once
//  *     you get past the initial existence/borrowed checks) — whether or
//  *     not anyone was waiting.
//  *
//  * display() RULES: for each book in catalog order, print:
//  *   "<title> - available"                                  if free, or
//  *   "<title> - borrowed by <name> (due day <n>)"            if out, and
//  *   append " | waitlist: <a>, <b>, ..." to that line if the waitlist
//  *   is non-empty.
//  *
//  * getAverageDueDay() must skip available books entirely, and must
//  * return 0.0 (not crash / not divide by zero) if nothing is currently
//  * borrowed.
//  *
//  * Header should be named Library.hpp (or split Book.hpp — your call).
//  * ============================================================
//  */

// #include "Library.hpp"
// #include <iostream>
// using namespace std;

// int main()
// {
//     cout << boolalpha;

//     Library lib;

//     // Deliberately out of alphabetical order.
//     lib.addBook({"Dune"});
//     lib.addBook({"Neuromancer"});
//     lib.addBook({"Foundation"});
//     lib.display();
//     // Expected:
//     // Dune - available
//     // Foundation - available
//     // Neuromancer - available

//     bool dupAdded = lib.addBook({"Foundation"});
//     cout << "duplicate Foundation added: " << dupAdded << endl; // Expected: false
//     lib.display(); // Expected: unchanged, still 3 books, no duplicate

//     bool amyCheckout = lib.checkout("Neuromancer", "Amy", 14);
//     cout << "Amy checkout Neuromancer: " << amyCheckout << endl; // Expected: true

//     // Sneaky: Ben tries to check out a book that's already out.
//     bool benCheckout = lib.checkout("Neuromancer", "Ben", 14);
//     cout << "Ben checkout Neuromancer: " << benCheckout << endl; // Expected: false

//     lib.joinWaitlist("Neuromancer", "Ben");
//     lib.joinWaitlist("Neuromancer", "Cleo");
//     // Sneaky: Amy already has the book — she can't also wait for it.
//     bool amyJoins = lib.joinWaitlist("Neuromancer", "Amy");
//     cout << "Amy joins own waitlist: " << amyJoins << endl; // Expected: false
//     lib.joinWaitlist("Neuromancer", "Dez");
//     // Sneaky: waitlist capacity is 3 and it's now full.
//     bool eliJoins = lib.joinWaitlist("Neuromancer", "Eli");
//     cout << "Eli joins full waitlist: " << eliJoins << endl; // Expected: false

//     lib.display();
//     // Expected:
//     // Dune - available
//     // Foundation - available
//     // Neuromancer - borrowed by Amy (due day 14) | waitlist: Ben, Cleo, Dez

//     // THE core moment: returning Neuromancer should auto-promote Ben
//     // (first in line), give him a fresh 14-day due date, and shift
//     // Cleo/Dez left to close the gap in the waitlist array.
//     bool returned = lib.returnBook("Neuromancer");
//     cout << "Neuromancer returned: " << returned << endl; // Expected: true
//     lib.display();
//     // Expected:
//     // Dune - available
//     // Foundation - available
//     // Neuromancer - borrowed by Ben (due day 14) | waitlist: Cleo, Dez

//     lib.checkout("Dune", "Frank", 15);
//     lib.display();
//     // Expected:
//     // Dune - borrowed by Frank (due day 15)
//     // Foundation - available
//     // Neuromancer - borrowed by Ben (due day 14) | waitlist: Cleo, Dez

//     cout << "Average due day: " << lib.getAverageDueDay() << endl;
//     // Expected: Average due day: 14.5 (Foundation is available, so it's
//     // excluded — this is NOT (14+15+0)/3)

//     // ================= BONUS (optional) =================

//     // Foundation is available and has no waitlist -> removable.
//     bool removedFoundation = lib.removeBook("Foundation");
//     cout << "Foundation removed: " << removedFoundation << endl; // Expected: true

//     // Sneaky: Dune is currently checked out -> must NOT be removable,
//     // even though it's a perfectly valid title.
//     bool removedDune = lib.removeBook("Dune");
//     cout << "Dune removed: " << removedDune << endl; // Expected: false

//     lib.display();
//     // Expected:
//     // Dune - borrowed by Frank (due day 15)
//     // Neuromancer - borrowed by Ben (due day 14) | waitlist: Cleo, Dez

//     cout << "contains Dune: " << lib.contains("Dune") << endl; // Expected: true
//     cout << "indexOf Neuromancer: " << lib.indexOf("Neuromancer") << endl; // Expected: 1

//     cout << "getSize: " << lib.getSize() << endl; // Expected: getSize: 2

//     // ================= state reset =================

//     lib.clear();
//     cout << "isEmpty after clear: " << lib.isEmpty() << endl; // Expected: true
//     lib.display();                                             // Expected: (blank line)

//     return 0;
// }