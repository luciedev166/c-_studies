#include "MyMatrix.hpp"
#include <iostream>

using namespace std;

int main() {
    cout << boolalpha; // print bools as true/false instead of 1/0

    // ================= PART 1: matrix A (3x4, non-square) =================
    // 1  2  3  4
    // 5  6  7  8
    // 9 10 11 12
    MyMatrix A(3, 4);
    int val = 1;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 4; c++)
            A.set(r, c, val++);

    // --- Baseline: Print all elements row by row ---
    A.displayRowByRow();
    // Expected:
    // 1 2 3 4
    // 5 6 7 8
    // 9 10 11 12

    // // --- Test 1: Print all elements column by column ---
    // A.displayColByCol();
    // // Expected:
    // // 1 5 9
    // // 2 6 10
    // // 3 7 11
    // // 4 8 12

    // // --- Test 2: Print matrix in reverse row order (bottom to top) ---
    // A.displayReverseRows();
    // // Expected:
    // // 9 10 11 12
    // // 5 6 7 8
    // // 1 2 3 4

    // // --- Test 3: Print each row in reverse (right to left) ---
    // A.displayRowsReversed();
    // // Expected:
    // // 4 3 2 1
    // // 8 7 6 5
    // // 12 11 10 9

    // // --- Test 4: Sum of all elements ---
    // cout << "sum(): " << A.sum() << endl; // Expected: sum(): 78

    // // --- Test 5: Find maximum and minimum element ---
    // cout << "maxValue(): " << A.maxValue() << endl; // Expected: maxValue(): 12
    // cout << "minValue(): " << A.minValue() << endl; // Expected: minValue(): 1

    // // --- Test 6: Count even and odd numbers ---
    // cout << "countEven(): " << A.countEven() << endl; // Expected: countEven(): 6
    // cout << "countOdd(): "  << A.countOdd()  << endl; // Expected: countOdd(): 6

    // // --- Test 7: Row sums and column sums ---
    // A.displayRowSums(); // Expected: 10 26 42
    // A.displayColSums(); // Expected: 15 18 21 24

    // // --- Test 17 (partial): A is NOT square ---
    // cout << "isSquare(): " << A.isSquare() << endl; // Expected: isSquare(): false

    // // --- Test 10: Transpose a matrix (must NOT mutate A) ---
    // MyMatrix At = A.transpose();
    // At.displayRowByRow();
    // // Expected:
    // // 1 5 9
    // // 2 6 10
    // // 3 7 11
    // // 4 8 12
    // A.displayRowByRow(); // prove A is untouched
    // // Expected:
    // // 1 2 3 4
    // // 5 6 7 8
    // // 9 10 11 12

    // // --- Test 11: Swap two rows (mutates A) ---
    // A.swapRows(0, 2);
    // A.displayRowByRow();
    // // Expected:
    // // 9 10 11 12
    // // 5 6 7 8
    // // 1 2 3 4

    // // --- Test 12: Swap two columns (mutates A further, on TOP of the row swap) ---
    // A.swapCols(0, 3);
    // A.displayRowByRow();
    // // Expected:
    // // 12 10 11 9
    // // 8 6 7 5
    // // 4 2 3 1

    // // ================= PART 2: matrix B (4x4, square) =================
    // // 1  2  3  4
    // // 5  6  7  8
    // // 9 10 11 12
    // // 13 14 15 17   <- note: 17, not 16, so main/anti diagonal sums differ
    // MyMatrix B(4, 4);
    // val = 1;
    // for (int r = 0; r < 4; r++)
    //     for (int c = 0; c < 4; c++)
    //         B.set(r, c, val++);
    // B.set(3, 3, 17);

    // cout << "isSquare(): " << B.isSquare() << endl; // Expected: isSquare(): true

    // // --- Test 8: Print main diagonal ---
    // B.displayMainDiagonal(); // Expected: 1 6 11 17

    // // --- Test 9: Print anti-diagonal ---
    // B.displayAntiDiagonal(); // Expected: 4 7 10 13

    // // --- Test 15: Sum of main diagonal elements ---
    // cout << "sumMainDiagonal(): " << B.sumMainDiagonal() << endl; // Expected: sumMainDiagonal(): 35

    // // --- Test 16: Sum of anti-diagonal elements ---
    // cout << "sumAntiDiagonal(): " << B.sumAntiDiagonal() << endl; // Expected: sumAntiDiagonal(): 34

    // // --- Test 13: Print upper triangular matrix ---
    // B.displayUpperTriangular();
    // // Expected:
    // // 1 2 3 4
    // // 0 6 7 8
    // // 0 0 11 12
    // // 0 0 0 17

    // // --- Test 14: Print lower triangular matrix ---
    // B.displayLowerTriangular();
    // // Expected:
    // // 1 0 0 0
    // // 5 6 0 0
    // // 9 10 11 0
    // // 13 14 15 17

    // // ================= PART 3: SNEAKY / EDGE CASES =================
    // // These mimic the "gotcha" tests a strict checker throws at you.

    // // --- Test 18: Copy matrix into a new matrix (must be a DEEP, independent copy) ---
    // MyMatrix C = B.copy();
    // B.set(0, 0, 999);       // mutate the ORIGINAL after copying
    // cout << "-- B after mutating (0,0) --" << endl;
    // B.displayRowByRow();
    // // Expected:
    // // 999 2 3 4
    // // 5 6 7 8
    // // 9 10 11 12
    // // 13 14 15 17
    // cout << "-- C should be UNCHANGED --" << endl;
    // C.displayRowByRow();
    // // Expected:
    // // 1 2 3 4
    // // 5 6 7 8
    // // 9 10 11 12
    // // 13 14 15 17

    // // Sneaky: matrix D, ALL NEGATIVE values (breaks implementations that
    // // start "best so far" at 0 for max/min, or mishandle negative evens)
    // MyMatrix D(2, 3);
    // D.set(0, 0, -5); D.set(0, 1, -1); D.set(0, 2, -10);
    // D.set(1, 0, -3); D.set(1, 1, -8); D.set(1, 2, -2);
    // D.displayRowByRow();
    // // Expected:
    // // -5 -1 -10
    // // -3 -8 -2
    // cout << "sum(): " << D.sum() << endl;             // Expected: sum(): -29
    // cout << "maxValue(): " << D.maxValue() << endl;   // Expected: maxValue(): -1
    // cout << "minValue(): " << D.minValue() << endl;   // Expected: minValue(): -10
    // cout << "countEven(): " << D.countEven() << endl; // Expected: countEven(): 3
    // cout << "countOdd(): "  << D.countOdd()  << endl; // Expected: countOdd(): 3

    // // Sneaky: 1x1 matrix (boundary for diagonal/anti-diagonal index math —
    // // an off-by-one here often crashes or reads garbage)
    // MyMatrix E(1, 1);
    // E.set(0, 0, 7);
    // cout << "isSquare(): " << E.isSquare() << endl; // Expected: isSquare(): true
    // E.displayMainDiagonal();  // Expected: 7
    // E.displayAntiDiagonal();  // Expected: 7
    // cout << "sumMainDiagonal(): " << E.sumMainDiagonal() << endl; // Expected: sumMainDiagonal(): 7
    // cout << "sumAntiDiagonal(): " << E.sumAntiDiagonal() << endl; // Expected: sumAntiDiagonal(): 7

    // // Sneaky: swapping a row/column with ITSELF must be a no-op, not
    // // corrupt data (classic bug if your swap uses a single temp variable
    // // carelessly)
    // MyMatrix F(2, 2);
    // F.set(0, 0, 1); F.set(0, 1, 2);
    // F.set(1, 0, 3); F.set(1, 1, 4);
    // F.swapRows(0, 0);
    // F.swapCols(1, 1);
    // F.displayRowByRow();
    // // Expected:
    // // 1 2
    // // 3 4

    // return 0;
}