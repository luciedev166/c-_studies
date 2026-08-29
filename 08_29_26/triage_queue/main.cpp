// TO BE ANSWERED
// /*
//  * ============================================================
//  *  SPEC — build these from scratch. Nothing below is provided.
//  *  (Sequel to the Leaderboard exercise — sorted-array shifting,
//  *  but now with TWO sort keys and a method that repositions an
//  *  element from the MIDDLE of the array, not just at insert time.)
//  * ============================================================
//  *
//  * You need TWO types:
//  *
//  *   Patient — holds:
//  *              string name
//  *              int    priority   // 1 = most urgent ... 5 = least urgent
//  *              int    arrival    // assigned INTERNALLY by the class,
//  *                                // ignore/overwrite whatever the caller
//  *                                // put here. Lower = arrived earlier.
//  *            main.cpp constructs it with brace-init like {"Bob", 1} —
//  *            only name and priority are given; arrival defaults to 0
//  *            and gets overwritten by add().
//  *
//  *   TriageQueue — a class with:
//  *              private: Patient board[6];     // fixed capacity
//  *              private: int     size;         // starts at 0
//  *              private: int     arrivalCounter; // starts at 0, only
//  *                                                // ever increases
//  *
//  *         Public methods to implement:
//  *           bool    add(Patient p);        // THE core method
//  *           bool    escalate(string name); // THE other core method
//  *           void    display() const;       // "name(pN) " in board order
//  *           int     getSize() const;
//  *           bool    isFull() const;
//  *           bool    isEmpty() const;
//  *           void    clear();
//  *           Patient getNextToBeSeen() const; // board[0]
//  *           Patient getLastInLine() const;   // last occupied slot
//  *           double  getAveragePriority() const; // must NOT truncate
//  *
//  *         Bonus / optional:
//  *           bool    contains(string name) const;
//  *           int     indexOf(string name) const;
//  *           bool    dischargeByName(string name); // shift LEFT to close gap
//  *
//  * SORT ORDER: board is sorted ASCENDING by priority (1s first). Ties
//  * in priority are broken by ASCENDING arrival (whoever got here first
//  * stays ahead). Because arrival is assigned by an ever-increasing
//  * internal counter, no two patients ever have the same arrival value —
//  * there is no "true tie" to special-case the way Leaderboard had to.
//  *
//  * add(Patient p) RULES:
//  *   - Stamp p.arrival = arrivalCounter, then arrivalCounter++.
//  *   - Do NOT call any full sort. One entry moves — shift only what
//  *     needs to move, in one pass, using direct assignment.
//  *   - If board isn't full: p always gets in, inserted at its correct
//  *     sorted position (by the two-key rule above).
//  *   - If board IS full: compare p against the current LAST patient
//  *     (the worst-off one). p only gets in if p's priority is STRICTLY
//  *     LESS (more urgent) than that last patient's priority — a new
//  *     arrival can never win a priority tie, since it always has the
//  *     newest (largest) arrival number, and ties are broken toward
//  *     whoever arrived earlier. If p doesn't qualify, add() returns
//  *     false and the board is UNCHANGED.
//  *   - If p does qualify, the current last patient is dropped (never
//  *     explicitly erased — just overwritten by the shift) and p is
//  *     inserted at its correct sorted position among what remains.
//  *
//  * escalate(name) RULES — this is the new mechanic, no Leaderboard
//  * equivalent:
//  *   - Find the patient by name. If not found, return false.
//  *   - If they're already priority 1, they can't get more urgent —
//  *     return false, nothing changes.
//  *   - Otherwise decrease their priority by 1 (their arrival value
//  *     stays exactly as it was — escalating does NOT reset your place
//  *     in a tie). This may now put them out of sorted order relative
//  *     to patients AHEAD of them in the array.
//  *   - Reposition ONLY this one patient LEFTWARD as far as the two-key
//  *     sort order requires (shifting the patients you pass over one
//  *     slot to the RIGHT), stopping as soon as you hit someone who is
//  *     still correctly ahead of them. Do not touch anything behind
//  *     their original position. Return true.
//  *
//  * Header should be named TriageQueue.hpp (or split Patient.hpp).
//  * ============================================================
//  */

// #include "TriageQueue.hpp"
// #include <iostream>
// using namespace std;

// int main()
// {
//     cout << boolalpha;

//     TriageQueue tq;

//     // Deliberately mixed priorities and a genuine tie to exercise both
//     // sort keys.
//     tq.add({"Alice", 3});
//     tq.add({"Bob", 1});
//     tq.add({"Cara", 3}); // ties Alice on priority, arrived later -> goes after her
//     tq.add({"Dan", 2});
//     tq.add({"Ed", 5});
//     tq.add({"Fay", 4});
//     tq.display(); // Expected: Bob(p1) Dan(p2) Alice(p3) Cara(p3) Fay(p4) Ed(p5)

//     cout << "isFull: " << tq.isFull() << endl; // Expected: isFull: true

//     // Sneaky: full board, new patient ties the current worst (p5). A tie
//     // never wins for a newcomer — rejected.
//     bool gusAdded = tq.add({"Gus", 5});
//     cout << "Gus added: " << gusAdded << endl; // Expected: Gus added: false
//     tq.display(); // Expected: Bob(p1) Dan(p2) Alice(p3) Cara(p3) Fay(p4) Ed(p5)  (unchanged)

//     // Sneaky: full board, new patient (p4) strictly beats the worst (p5) —
//     // qualifies. But p4 ties Fay on priority, so Harry lands AFTER Fay,
//     // and Ed(p5) is the one who quietly falls off.
//     bool harryAdded = tq.add({"Harry", 4});
//     cout << "Harry added: " << harryAdded << endl; // Expected: Harry added: true
//     tq.display(); // Expected: Bob(p1) Dan(p2) Alice(p3) Cara(p3) Fay(p4) Harry(p4)

//     // Escalate Harry: p4 -> p3. He should move LEFT past Fay (who's now
//     // strictly worse than him) but STOP at Cara — same new priority (p3),
//     // and Cara arrived first, so Cara correctly stays ahead of him.
//     bool escalated = tq.escalate("Harry");
//     cout << "Harry escalated: " << escalated << endl; // Expected: true
//     tq.display(); // Expected: Bob(p1) Dan(p2) Alice(p3) Cara(p3) Harry(p3) Fay(p4)

//     // Sneaky: Bob is already p1 — can't escalate further.
//     cout << "Bob escalated: " << tq.escalate("Bob") << endl; // Expected: false
//     // Sneaky: Ed isn't even on the board anymore (fell off earlier).
//     cout << "Ed escalated: " << tq.escalate("Ed") << endl; // Expected: false
//     tq.display(); // Expected: Bob(p1) Dan(p2) Alice(p3) Cara(p3) Harry(p3) Fay(p4)  (unchanged)

//     cout << "Next to be seen: " << tq.getNextToBeSeen().name << endl; // Expected: Bob
//     cout << "Last in line: " << tq.getLastInLine().name << endl;     // Expected: Fay

//     cout << "Average priority: " << tq.getAveragePriority() << endl;
//     // Expected: Average priority: 2.66667 (or similar — NOT 2, that's
//     // integer division)

//     // ================= BONUS (optional) =================

//     cout << "contains Cara: " << tq.contains("Cara") << endl; // Expected: true
//     cout << "indexOf Fay: " << tq.indexOf("Fay") << endl;     // Expected: 5
//     cout << "indexOf Zed: " << tq.indexOf("Zed") << endl;     // Expected: -1

//     bool discharged = tq.dischargeByName("Alice");
//     cout << "Alice discharged: " << discharged << endl; // Expected: true
//     tq.display();                                        // Expected: Bob(p1) Dan(p2) Cara(p3) Harry(p3) Fay(p4)
//     cout << "getSize: " << tq.getSize() << endl;         // Expected: getSize: 5

//     cout << "Alice discharged again: " << tq.dischargeByName("Alice") << endl;
//     // Expected: false

//     // ================= state reset =================

//     tq.clear();
//     cout << "isEmpty after clear: " << tq.isEmpty() << endl; // Expected: true
//     tq.display();                                             // Expected: (blank line)

//     return 0;
// }