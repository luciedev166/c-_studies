# Doubly Linked List drills

Same template as the singly drills: one folder per function, `main.cpp` with the tests,
`LinkedList.hpp` with the solved function above the `DO NOT CHANGE` line and the helpers below it.

| Folder | Style | Functions |
|--------|-------|-----------|
| `1_sentinel_nodes/`    | sentinels (header / trailer) are `Node` OBJECTS: `Node header, trailer;` | 12 drills |
| `2_sentinel_pointers/` | sentinels (header / trailer) are POINTERS: `Node *header, *trailer;`      | 12 drills |
| `3_no_sentinel/`       | no sentinels, plain `head` / `tail`                                      | 6 basic drills |

Folders 1 and 2 are the same 12 drills with the same tests; only how the sentinels are declared
changes. They are built on `addBetween` and `removeNode`. Folder 3 only has the basics:
addHead, addTail, removeHead, removeTail, printFromHead, printFromTail.

Each style has its own README.md with the workflow, the folder list and the rules the tests assume.
Quick start:

    cd 1_sentinel_nodes/01_addHead
    g++ -std=c++17 -Wall -g -fsanitize=address -o test main.cpp && ./test
