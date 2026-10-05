# add_ons (Priority A)

Same setup as `tree_drills`: one folder per function group, a locked `main.cpp` with the auto-checks, and a `GeneralTree.hpp`
where the solved function(s) sit **above** the `DO NOT CHANGE` line (your area) and the helpers sit **below** it.
Everything below the line is the same helper set as drills 13/14 (it never calls the function you are practicing).

Run a folder:
```
g++ -std=c++17 -Wall -g -fsanitize=address -o test main.cpp && ./test
```

| Folder | You write | Conventions the tests assume |
|---|---|---|
| `23_numChildren_totalChildren_nthChild` | `getNumChildren(p)`, `getTotalChildren(p)`, `getNthChild(p, n)` | numChildren = direct children; totalChildren = all descendants (p not counted); `n` is **1-based**; nullptr -> -1 (counts) / nullptr (nth) |
| `24_addChildAtPosition` | `addChildAtPosition(p, val, pos)` | `pos` 1-based, valid 1..children+1, else nullptr |
| `25_addChildBeforeAfter` | `addChildBefore(ref, val)`, `addChildAfter(ref, val)` | new node is a sibling of `ref`; root / nullptr -> nullptr |
| `26_removeChildWithVal` | `removeChildWithVal(p, val)` -> bool | first direct child with `val`; only removed if it is a leaf |
| `27_removeAllChildrenWithVal` | `removeAllChildrenWithVal(p, val)` -> count | all direct leaf children with `val`; nullptr -> -1 |
| `28_printLeaves_removeAllLeaves` | `printLeaves()`, `removeAllLeaves()` -> count | one layer only; a lone root counts as a leaf |
| `29_countWithVal_countGreaterThan_countWithKChildren` | the three counters (subtree + whole-tree versions) | subtree includes p; `>` is strict; k = direct children |
| `30_printByLevel` | `printByLevel()`, `printAtDepth(k)` -> count | one line per level, single spaces, no trailing space; root is depth 0 |

If your prof's version of a function differs (0-based index, different error value, ...), change the matching line in the
tests/rules comment at the top of that folder's `main.cpp`, or just tell me and I'll adjust the folder.
