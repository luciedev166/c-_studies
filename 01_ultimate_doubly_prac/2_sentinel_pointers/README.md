# Doubly Linked List drills: sentinels are POINTERS

The two guards are pointers to dummy nodes created with `new` in the constructor:

    Node *header;
    Node *trailer;

You use them directly as pointers: `header`, `trailer`, and reach their fields with an arrow:
`header->next`, `trailer->prev`. The destructor must `delete` them after `clear()`.

The real nodes always sit BETWEEN the two guards, so no node ever has a `nullptr` neighbor.
That is the whole point: `addHead`, `addTail`, `removeHead`, `removeTail` and friends need no
special cases for "empty list" or "only one node". Two primitives do all the linking:

- `addBetween(val, pred, succ)`: link a new node between two neighbors.
- `removeNode(node)`: unlink one real node and return its value.

One folder per function. Each folder has:

- `main.cpp`: tests for ONLY that function. Don't edit it.
- `LinkedList.hpp`: the solved function ABOVE the `DO NOT CHANGE` line, and
  the helper functions the main needs (setup, printing, checking) BELOW it.
  If your function is built on `addBetween` / `removeNode`, that primitive sits above the
  line too, so you can rewrite it as well. (Only your function calls it, so it can't break the helpers.)

## Workflow

1. `cd` into a folder, e.g. `cd 03_removeHead`
2. Compile and run:

       g++ -std=c++17 -Wall -g -fsanitize=address -o test main.cpp && ./test

   No AddressSanitizer on your machine (common on Windows)? Use:

       g++ -std=c++17 -Wall -g -o test main.cpp && ./test

3. Read the solved function. Then comment it out, write your own in the same
   spot (above the DO NOT CHANGE line), and re-run until everything says PASS.

Every test prints one line, PASS or FAIL. A FAIL shows what you returned and
what was expected. If the program crashes, the last `- ...` line on screen is the
test that crashed it. (Segfault = you probably dereferenced nullptr. Look at the
test name to see which edge case it was.)

## Reading a FAIL

| Line                       | Meaning |
|----------------------------|---------|
| `list`                     | the list doesn't print the way it should |
| `size`                     | `getSize()` is off (forgot `size++` / `size--`) |
| `integrity`                | next / prev pointers / size disagree with each other |
| `list (backward via prev)` | going backwards through `prev` gives the wrong values: a prev pointer is wrong |

## Folders (suggested order)

| Folder | Function |
|--------|----------|
| 01_addHead | `addHead(val)` |
| 02_addTail | `addTail(val)` |
| 03_removeHead | `int removeHead()` |
| 04_removeTail | `int removeTail()` |
| 05_get | `int get(pos)` (value at index pos) |
| 06_remove | `int remove(num)` (removes the FIRST node whose value is num; returns num, or -1 if not found) |
| 07_addAt | `addAt(val, pos)` |
| 08_removePosn | `int removePosn(pos)` (returns the removed value) |
| 09_removeAllVals | `int removeAllVals(val)` (returns how many were removed) |
| 10_getVal | `int getVal(val)` (index of the FIRST match, or -1) |
| 11_printFromHead | `printFromHead()` |
| 12_printFromTail | `printFromTail()` (walks back with prev, no recursion) |

## Rules the tests assume (same in every folder)

- Print format: `10 -> 20 -> 30 -> NULL` (arrow with spaces, `NULL` at the end, newline).
  Empty list prints just `NULL`. `printFromTail` prints the same way, last node first.
- `remove*` and `get*` functions return `-1` for an empty list, an invalid position, or a
  value that isn't there. What message you print in that case doesn't matter; the tests ignore it.
- Positions are 0-based.
- `addAt`: `pos <= 0` goes to the front, `pos >= size` goes to the back.
- `remove(num)` removes by VALUE (first match only). `removePosn(pos)` removes by POSITION.

## About the helpers below the line

- They are self-contained. They never call the function you're practicing, so
  a bug in yours can't break them.
- `integrity()` and `backwardText()` are test-only helpers. `integrity()` checks that the
  next pointers, the prev pointers and `size` all agree. `backwardText()` reads the list
  through the prev pointers, so a wrong `prev` shows up as a FAIL instead of a mystery crash later.
  The real assignment won't have them.
- Your solved function can reuse helpers from below the line, just like you'd reuse your own
  functions in the real practical.
