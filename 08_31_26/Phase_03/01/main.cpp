/*
 * ============================================================
 *  PHASE 3 — PROBLEM 1 (deep)
 *  SPEC — build Player and Leaderboard from scratch.
 * ============================================================
 *
 * Same shape as the very first Leaderboard spec you were given — sorted
 * descending by score, fixed capacity 5, displaces the weakest when
 * full. The twist: Player.name is now a heap-owned char*, not a
 * std::string. That single change means the container is now
 * responsible for manually freeing memory at exactly the right moments
 * — nothing does that for you automatically anymore.
 *
 *   struct Player — holds:
 *              char* name;   // heap-allocated (malloc'd), owned
 *              int   score;
 *            Plain struct, no constructor/destructor of its own — this
 *            is deliberate. A malloc'd array of Players never runs a
 *            C++ constructor or destructor on its elements anyway, so
 *            giving Player one would be misleading. Ownership and
 *            cleanup are entirely YOUR container's job, done by hand.
 *
 *   class Leaderboard
 *     private:
 *       Player board[5];   // fixed array member, same as the original spec
 *       int    size;       // starts at 0
 *
 *     public:
 *       Leaderboard();
 *       ~Leaderboard();
 *         // Loop over the `size` occupied slots and free(board[i].name)
 *         // for each one. This is the ONLY place that memory gets
 *         // cleaned up when the board is never emptied out manually.
 *
 *       bool add(Player p);
 *         // IDENTICAL sorting/displacement rules to the original spec:
 *         // board stays sorted descending, one entry moves per call, a
 *         // new entry only displaces something when the board is full
 *         // AND its score is STRICTLY GREATER than an existing entry's.
 *         //
 *         // THE NEW PART: when the board is full and a displacement IS
 *         // about to happen, the current weakest entry (board[4], the
 *         // one about to be silently overwritten by the shift cascade)
 *         // owns a malloc'd name that is about to become unreachable.
 *         // free(board[4].name) BEFORE you run the shift, or that
 *         // block leaks forever — nothing else will ever free it.
 *         //
 *         // If the board is full and p does NOT displace anything,
 *         // return false and leave the board untouched — same as
 *         // before. But now p.name was never absorbed into the board,
 *         // so ownership of it was never transferred. YOUR add()
 *         // should NOT free it in this case — the caller (main.cpp)
 *         // still owns it and is responsible for freeing it, exactly
 *         // like it would if add() had simply never been called.
 *
 *       void display() const;   // prints "name(score) " for each, board order
 *       int  getSize() const;
 *       bool isFull() const;
 *
 *       Player getStrongest() const;
 *         // Assume never called on empty. Returns board[0] — BUT the
 *         // returned Player's `name` must be a FRESH malloc'd copy of
 *         // board[0].name, not the same pointer. Handing back the
 *         // internal pointer directly would let the caller free() it
 *         // and corrupt (or double-free) the board's own copy. The
 *         // caller owns whatever comes back from this method and is
 *         // expected to free() it when done.
 *
 * Header should be named Leaderboard.hpp (or split Player.hpp — your call).
 * ============================================================
 */

#include "Leaderboard.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

// ---- PROVIDED — do not modify. Mallocs a copy of a C-string. ----
char* dupName(const char* src) {
    char* copy = (char*)malloc(strlen(src) + 1);
    strcpy(copy, src);
    return copy;
}

int main() {
    cout << boolalpha;

    // Same test data as your very first Leaderboard spec — if you
    // already have that board-state sequence memorized, use it. The
    // only new thing to track this time is WHEN a name gets freed.

    Leaderboard board;

    board.add({dupName("Bob"), 80});
    board.add({dupName("Alice"), 90});
    board.add({dupName("Dave"), 60});
    board.add({dupName("Carol"), 70});
    board.add({dupName("Eve"), 50});
    board.display(); // Expected: Alice(90) Bob(80) Carol(70) Dave(60) Eve(50)
    cout << "isFull: " << board.isFull() << endl; // Expected: isFull: true

    // Full board, 65 beats Dave(60) and Eve(50) but not Carol(70) —
    // lands in the middle, Eve falls off. Eve's malloc'd name is now
    // unreachable from anywhere in main — if your add() didn't free it
    // the instant before the shift overwrote her slot, that block leaks
    // for the rest of the program with literally no way to recover it.
    board.add({dupName("Frank"), 65});
    board.display(); // Expected: Alice(90) Bob(80) Carol(70) Frank(65) Dave(60)

    // Full board, 40 doesn't beat anyone — rejected. This name was
    // NEVER absorbed into the board, so add() must NOT free it — main
    // still owns it and must clean it up itself, same as any malloc
    // whose add() call never happened.
    char* rejectedName = dupName("Grace");
    bool graceAdded = board.add({rejectedName, 40});
    cout << "Grace added: " << graceAdded << endl; // Expected: Grace added: false
    board.display(); // Expected (unchanged): Alice(90) Bob(80) Carol(70) Frank(65) Dave(60)
    free(rejectedName); // main's job, since the board never took ownership

    // Full board, tie with Alice(90) — not strictly greater, doesn't
    // displace her. First strictly-greater match is Bob(80), so Zack
    // lands right after Alice. Dave(60), currently weakest, falls off
    // and must be freed — same lesson as Eve, different slot.
    board.add({dupName("Zack"), 90});
    board.display(); // Expected: Alice(90) Zack(90) Bob(80) Carol(70) Frank(65)

    // ================= getStrongest() must hand back a COPY =================
    Player top = board.getStrongest();
    cout << top.name << "(" << top.score << ")" << endl; // Expected: Alice(90)

    free(top.name); // caller owns this copy — freeing it must be safe
    board.display();
    // Expected (STILL intact): Alice(90) Zack(90) Bob(80) Carol(70) Frank(65)
    // ^ if this line now shows corrupted/garbage text where "Alice"
    //   should be, getStrongest() handed back the board's OWN pointer
    //   instead of a fresh copy — the free() above just deleted the
    //   board's actual data out from under it.

    // No explicit cleanup call needed here — when `board` goes out of
    // scope at the end of main, its destructor should free every
    // remaining name (Alice, Zack, Bob, Carol, Frank) exactly once each.

    return 0;
}