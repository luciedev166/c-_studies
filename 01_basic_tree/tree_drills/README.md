# tree_drills

Same setup as your `singly_drills`: one folder per function, a locked `main.cpp` with tests, and an `.hpp` where the solved function sits **above** the `DO NOT CHANGE` line and the helpers sit below it.

**No vectors, no STL queue/stack.** Everything is pointers and linked lists, the way your class has taught it.

## How to study a folder

1. Open the `.hpp`. Read the solved function(s) above the line (the comments mark the edge cases).
2. Comment them out and rewrite them from memory.
3. Compile and run:
   ```
   g++ -std=c++17 -Wall -g -fsanitize=address -o test main.cpp && ./test
   ```
   (No ASan? Drop `-fsanitize=address`.) Each test prints `PASS`, or `FAIL` with got vs. expected. If it crashes, the last `- ...` line on screen is the test that crashed it.
4. Next folder.

## Folders (★ = most likely on Tuesday, do these first)

| Folder | You write | Needs |
|---|---|---|
| `00a_queue_enqueue_dequeue` ★ | `enqueue`, `dequeue` | |
| `00b_queue_front_isEmpty_size` | `front`, `isEmpty`, `getSize` | |
| `01_addRoot_addChild` ★ | `addRoot`, `addChild` | |
| `02_isRoot_isLeaf_isInternal_size` ★ | `isRoot`, `isLeaf`, `isInternal`, `isEmpty`, `getSize` | |
| `03_printPreorder` ★ | `printPreorder` | |
| `04_printPostorder` ★ | `printPostorder` | |
| `05_printLevelOrder` ★ | `printLevelOrder` | folder 00 (queue idea) |
| `06_printIndented` ★ | `printIndented` | |
| `07_height` ★ | `height(p)`, `height()` | |
| `08_depth` ★ | `depth(p)` | |
| `09_countLeaves_countNodes` ★ | `countNodes`, `countLeaves` (subtree + whole tree) | |
| `10_search` | `search(val)` | |
| `11_sumAndMax` | `sumAll`, `maxVal` | |
| `12_printPathFromRoot` | `printPathFromRoot(val)` | parent pointers |
| `13_removeLeaf` | `removeLeaf(p)` | your `removeNreturnN` edge cases |
| `14_clear` | `clear()` | |
| `15_addRoot_addLeft_addRight` | binary: add nodes | |
| `16_left_right_sibling` | binary: `left`, `right`, `sibling` | |
| `17_set` | binary: `set` | |
| `18_remove` | binary: `remove` (0 or 1 child) | |
| `19_attach` | binary: `attach(p, T1, T2)` | the hand-drawn slide |
| `20_inorder_preorder_postorder` | binary: the 3 traversals | |
| `21_mock_general` | a whole general tree, no solutions | do this LAST |
| `22_mock_binary` | a whole binary tree, no solutions | do this LAST |

Suggested order: 00a, 01, 02, 03, 04, 07, 08, 09, 00b, 05, 06, then 10-14, then 15-20, then the mocks.

## The two node designs

**General tree (many children, your prof's hint).** Each node's children are a singly linked list with a head and a tail, exactly like your list drills:
```
struct Node { int elem; Node *parent; Node *firstChild; Node *lastChild; Node *nextSibling; };
```
Loop over the children of `p`:
```
for (Node *c = p->firstChild; c != nullptr; c = c->nextSibling)
```
`addChild` = `addBack` on that list. `removeLeaf` = remove from that list. The same edge cases show up: empty list, first node, last node, only node.

**Binary tree (the slides).** `elem`, `parent`, `left`, `right`, plus `root` and `size` in the tree.

## The recursion pattern (almost every tree function is this)

```
f(p):
    if p is nullptr: return the "nothing here" answer     // base case
    handle p itself
    for each child c:  combine f(c)                       // recursive case
```
Pre/post/in-order only differ in WHEN "handle p itself" happens, relative to the children.

## Queue in one picture (for level order)

A queue is a linked list where you add at the **tail** and remove from the **head**. Level order: put the root in, then repeat: take the head, print it, put its children at the tail.

## Conventions the tests assume

- Queue drills (00a, 00b) use a plain `Node` with `elem` and `next`, like your list drills. (In the tree drills, `Node` is the TREE node, so the little queue helper inside `NodeQueue` uses its own node type called `QNode`. You never write that part.)
- Print format is the same as your list drills: `1 -> 2 -> 3 -> NULL`. Empty tree: `NULL`. (`printIndented` is the exception: one node per line, 2 spaces per depth.)
- Children are visited in the order they were added.
- **Errors:** return the value the folder says (`nullptr`, `-1`, or `false`) and change nothing. The message you print doesn't matter in the drills.
- **Integrity checks:** every test calls a helper `integrity()` (below the DO NOT CHANGE line) that checks all your pointers agree. When it fails, it tells you which link is wrong, usually a `lastChild` / `parent` / `root` you forgot to update.
- Binary tree test shapes are written `elem(left,right)`, `-` = no child, a leaf is just its number. Example: `1(2(4,5),3(-,6))`.

## The mock practicals (21 and 22)

These imitate your prof's format: a **locked `main.cpp`**, a `tree.hpp` where you write the functions (all bodies are empty `// TODO` stubs), and a **checker**. `main.cpp` reads commands from the input, so each test is an `.in` file and the expected output is the matching `.out`.

```
cd 21_mock_general
./run_tests.sh
```
(On Windows without bash: `g++ -std=c++17 -o test main.cpp`, `g++ -std=c++17 -o checker checker.cpp`, then `test < tests\1.in > out.txt` and `checker out.txt tests\1.out`.)

Rules of the mocks, same as a real judge:
- The checker is **exact**: spaces, blank lines and the final newline count.
- In the error cases your functions print **nothing**; `main.cpp` prints the messages. (Different from the drills, where an extra message is ignored.)
- Read the comment block in `tree.hpp` first. It lists every function, its return value and its format.

## If the real practical differs

- Match the exact **names, parameter order and return types** that his `main.cpp` calls. You can't change `main.cpp`.
- His binary-tree slides say "an error occurs". If his `main.cpp` has `try { } catch`, then he expects you to `throw` (for example `throw logic_error("...")`) instead of returning `nullptr` / `-1`. Check `main.cpp` for `catch` first.
- If his `list.hpp` already gives you a `Node` struct with different member names, use his names. The logic stays the same.
