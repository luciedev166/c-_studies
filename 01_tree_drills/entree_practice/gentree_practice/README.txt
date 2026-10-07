gentree_skeleton.hpp  - empty declarations, code inside the TODOs
test_gentree.cpp      - checks exceptions, depth, height, array shifting, removing the root

Run:  g++ -std=c++17 -Wall -g -fsanitize=address -o test test_gentree.cpp && ./test
Prints "ALL OK" when everything passes. A failed check stops the program with the line number.
Assumes: root depth 0, leaf height 0 (change the test's numbers if your spec says otherwise).
