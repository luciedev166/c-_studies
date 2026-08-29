/*
 * ============================================================
 *  SPEC — build these from scratch. Nothing below is provided.
 * ============================================================
 *
 * You need TWO types:
 *
 *   Entry  — holds:
 *              string name
 *              int    score
 *            (struct OR class, your choice — main.cpp constructs
 *            it with brace-init like {"Alice", 90}, which works
 *            either way as long as name/score are accessible and
 *            in that order.)
 *
 *   Leaderboard — a class with:
 *              private: Entry board[5];   // fixed capacity, NOT a vector
 *              private: int    size;      // starts at 0
 *  
 *         Public methods to implement:
 *           bool   add(Entry e);          // THE core method — see rules below
 *           void   display() const;       // prints "name(score) " for each, in board order
 *           int    getSize() const;
 *           bool   isFull() const;
 *           bool   isEmpty() const;
 *           void   clear();
 *           double getAverageScore() const;  // must NOT truncate to int
 *           Entry  getStrongest() const;     // board[0] — assume never called on empty
 *           Entry  getWeakest() const;       // last occupied slot — assume never called on empty
 *
 *         Bonus / optional (only if you have time left):
 *           bool   contains(string name) const;
 *           int    indexOf(string name) const;   // -1 if not found
 *           bool   removeByName(string name);    // shift LEFT to close the gap
 *
 * add(Entry e) RULES — this is the actual thing being tested:
 *   - Board must stay sorted DESCENDING by score at all times.
 *   - Do NOT call any full sort inside add(). One entry moves — only
 *     shift what actually needs to move, in one pass.
 *   - Use DIRECT ASSIGNMENT to shift (board[j] = board[j-1]), not a
 *     three-line temp swap chain. Nothing is trading places here —
 *     everything cascades one direction.
 *   - A new entry only displaces something when board is full AND its
 *     score is STRICTLY GREATER than an existing entry's score. Equal
 *     score does NOT count as "greater" — ties never displace, and a
 *     tie always lands AFTER existing equal entries.
 *   - If the board is full and the new entry isn't strictly greater
 *     than anything on it, add() returns false and the board is
 *     UNCHANGED.
 *   - If the board isn't full, the new entry always gets in — either
 *     shifted into its sorted position, or appended at the end if
 *     it's not greater than anything currently there.
 *
 * Header should be named Leaderboard.hpp (or split Entry.hpp — your call).
 * ============================================================
 */

#include "Leaderboard.hpp"
#include <iostream>
using namespace std;

int main()
{
    cout << boolalpha; // print bool as true/false instead of 1/0

    // ================= SECTION A: mixed scores =================
    // Deliberately inserted OUT of sorted order — the class must sort
    // itself as each one comes in, not rely on input already sorted.

    Leaderboard board;

    board.add({"Bob", 80});
    board.add({"Alice", 90});
    board.add({"Dave", 60});
    board.display(); // Expected: Bob(80) Alice(90) Dave(60)  <-- WRONG on purpose, see below
    // ^ if your display() prints that, something's wrong — after 3 correct
    //   inserts it should already be sorted. Real expected:
    //   Alice(90) Bob(80) Dave(60)

    cout << "Average so far: " << board.getAverageScore() << endl;
    // Expected: Average so far: 76.6667 (or similar — NOT 76, that means
    // you didn't cast before dividing)

    board.add({"Carol", 70});
    board.display(); // Expected: Alice(90) Bob(80) Carol(70) Dave(60)

    board.add({"Eve", 50});
    board.display();                              // Expected: Alice(90) Bob(80) Carol(70) Dave(60) Eve(50)
    cout << "isFull: " << board.isFull() << endl; // Expected: isFull: true

    // Sneaky: board full, new entry beats the weakest but not everyone —
    // should land in the MIDDLE, and the current weakest quietly falls off.
    board.add({"Frank", 65});
    board.display(); // Expected: Alice(90) Bob(80) Carol(70) Frank(65) Dave(60)
    // Eve(50) is gone. You should never have explicitly deleted her —
    // she was overwritten as a side effect of the shift.

    // Sneaky: board full, new entry WORSE than everyone currently on it.
    bool wasAdded = board.add({"Grace", 40});
    cout << "Grace added: " << wasAdded << endl; // Expected: Grace added: false
    board.display();                             // Expected: Alice(90) Bob(80) Carol(70) Frank(65) Dave(60)  (unchanged)

    // Sneaky: TIE with an existing score, board full. Must NOT displace
    // Alice (equal isn't "strictly greater") — must land right after her.
    board.add({"Zack", 90});
    board.display(); // Expected: Alice(90) Zack(90) Bob(80) Carol(70) Frank(65)
    // Dave(60) fell off this time — he was the weakest at the moment
    // the shift cascaded.

    // ================= SECTION B: all-ties edge case =================
    // Fresh object — a nastier test of the strict-greater rule.

    Leaderboard ties;
    ties.add({"P1", 50});
    ties.add({"P2", 50});
    ties.add({"P3", 50});
    ties.add({"P4", 50});
    ties.add({"P5", 50});
    ties.display(); // Expected: P1(50) P2(50) P3(50) P4(50) P5(50)
    // None of these ever beat each other, so they just appended in the
    // order they arrived — no shifting happened at all yet.

    cout << "isFull: " << ties.isFull() << endl; // Expected: isFull: true

    // Board full, EVERY existing entry ties the newcomer. Nothing is
    // "strictly greater" so nothing gets displaced — even though P5
    // technically arrived last. This one trips people up.
    bool p6Added = ties.add({"P6", 50});
    cout << "P6 added: " << p6Added << endl; // Expected: P6 added: false
    ties.display();                          // Expected: P1(50) P2(50) P3(50) P4(50) P5(50)  (unchanged)

    // Now break the tie — first entry that's ACTUALLY better than
    // something displaces from the front, cascades, P5 falls off.
    ties.add({"P7", 51});
    ties.display(); // Expected: P7(51) P1(50) P2(50) P3(50) P4(50)

    // ================= SECTION C: state resets + edge fills =================

    Leaderboard fresh;
    cout << "isEmpty (new board): " << fresh.isEmpty() << endl; // Expected: true

    fresh.add({"Solo", 100});
    fresh.display();                                               // Expected: Solo(100)
    cout << "getSize: " << fresh.getSize() << endl;                // Expected: getSize: 1
    cout << "getStrongest: " << fresh.getStrongest().name << endl; // Expected: Solo
    cout << "getWeakest: " << fresh.getWeakest().name << endl;     // Expected: Solo (only one entry)

    fresh.clear();
    cout << "isEmpty after clear: " << fresh.isEmpty() << endl; // Expected: true
    cout << "getSize after clear: " << fresh.getSize() << endl; // Expected: getSize after clear: 0
    fresh.display();                                            // Expected: (blank line)

    // Prove state doesn't "leak" after clear — reinsert and re-test full fork.
    fresh.add({"NewGuy", 10});
    fresh.add({"NewGuy2", 20});
    fresh.display(); // Expected: NewGuy2(20) NewGuy(10)

    // ================= SECTION D (BONUS — optional) =================
    // Only attempt this if Sections A-C are solid and you have time left.

    Leaderboard bonus;
    bonus.add({"Amy", 88});
    bonus.add({"Ben", 72});
    bonus.add({"Cleo", 95});
    bonus.display(); // Expected: Cleo(95) Amy(88) Ben(72)

    cout << "contains(Ben): " << bonus.contains("Ben") << endl; // Expected: true
    cout << "contains(Zoe): " << bonus.contains("Zoe") << endl; // Expected: false
    cout << "indexOf(Amy): " << bonus.indexOf("Amy") << endl;   // Expected: 1
    cout << "indexOf(Zoe): " << bonus.indexOf("Zoe") << endl;   // Expected: -1

    // removeByName should shift LEFT to close the gap — the opposite
    // direction from add()'s shift. Good pairing exercise.
    bool removed = bonus.removeByName("Amy");
    cout << "Amy removed: " << removed << endl;     // Expected: true
    bonus.display();                                // Expected: Cleo(95) Ben(72)
    cout << "getSize: " << bonus.getSize() << endl; // Expected: getSize: 2

    bool removedAgain = bonus.removeByName("Amy");         // already gone
    cout << "Amy removed again: " << removedAgain << endl; // Expected: false

    return 0;
}