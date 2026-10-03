# Singly Linked List drills

One folder per function. Each folder has:

- `main.cpp`: tests for ONLY that function. Don't edit it.
- `LinkedList.hpp`: the solved function ABOVE the `DO NOT CHANGE` line, and
  the helper functions the main needs (setup, printing, checking) BELOW it.

## Workflow

1. `cd` into a folder, e.g. `cd 04_removeFront`
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

| Line      | Meaning |
|-----------|---------|
| `list`      | the list doesn't print the way it should |
| `size`      | `getSize()` is off (forgot `size++` / `size--`) |
| `integrity` | head / tail / size disagree. Usually `tail` (or `head`) wasn't updated in an edge case: empty list, one node, first or last position |

## Folders (suggested order)

| Folder | Function |
|--------|----------|
| 01_addFront | `addFront(val)` |
| 02_addBack | `addBack(val)` |
| 03_insertAtN | `insertAtN(val, pos)` |
| 04_removeFront | `int removeFront()` |
| 05_removeBack | `int removeBack()` |
| 06_removeNreturnN | `int removeNreturnN(pos)` |
| 07_removeAllVals | `int removeAllVals(val)` (returns how many were removed) |
| 08_getValAtN | `int getValAtN(pos)` |
| 09_getVal | `bool getVal(val)` (true if it's in the list) |
| 10_clear | `clear()` |
| 11_printFromHead | `printFromHead()` |
| 12_printFromTail | `printFromTail()` (recursion) |
| 13_isEmpty_getSize | `isEmpty()`, `getSize()` |

## Rules the tests assume (same in every folder)

- Print format: `10 -> 20 -> 30 -> NULL` (arrow with spaces, `NULL` at the end, newline).
  Empty list prints just `NULL`.
- `remove*` and `get*` functions return `-1` for an empty list or an invalid position.
  What message you print in that case doesn't matter; the tests ignore it.
- `insertAtN`: `pos <= 0` goes to the front, `pos >= size` goes to the back.
- Positions are 0-based.

## About the helpers below the line

- They are self-contained. They never call the function you're practicing, so
  a bug in yours can't break them.
- `integrity()` is a test-only helper: it checks that head / tail / size agree.
  The professor's real assignment won't have it; it's just here so edge-case
  mistakes show up as a clear message instead of a mystery crash later.
- Your solved function can reuse helpers from below (for example `insertAtN`
  uses `addFront` / `addBack`), just like you'd reuse your own functions in the
  real practical.
