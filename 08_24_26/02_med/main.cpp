#include "MyArray.hpp"
#include <iostream>
using namespace std;

int main() {

    cout << boolalpha;

    // ============================================================
    // MEDIUM ARRAY PROBLEMS
    // ============================================================


    // ------------------------------------------------------------
    // 1. REMOVE DUPLICATES
    // ------------------------------------------------------------
    // TASK:
    // Remove all duplicate values from the array.
    //
    // Keep only the FIRST occurrence of each value.
    //
    // Example:
    // Input:  4 2 4 7 2 9
    // Output: 4 2 7 9
    // ------------------------------------------------------------

    MyArray a1;

    a1.insert(0, 4);
    a1.insert(1, 2);
    a1.insert(2, 4);
    a1.insert(3, 7);
    a1.insert(4, 2);
    a1.insert(5, 9);

    cout << "\nOriginal: ";
    a1.display();

    a1.removeDuplicates();

    cout << "After removing duplicates: ";
    a1.display();


    // ------------------------------------------------------------
    // 2. FIND ALL DUPLICATE VALUES
    // ------------------------------------------------------------
    // TASK:
    // Find and print every value that appears more than once.
    //
    // Each duplicate value should only be printed ONCE.
    //
    // Example:
    // Input:  5 3 5 8 3 9 8 8
    // Output: 5 3 8
    //
    // NOTE:
    // 8 appears three times, but should only be reported once.
    // ------------------------------------------------------------

    MyArray a2;

    a2.insert(0, 5);
    a2.insert(1, 3);
    a2.insert(2, 5);
    a2.insert(3, 8);
    a2.insert(4, 3);
    a2.insert(5, 9);
    a2.insert(6, 8);
    a2.insert(7, 8);

    cout << "\nArray: ";
    a2.display();

    cout << "Duplicate values: ";
    a2.printDuplicates();


    // ------------------------------------------------------------
    // 3. ROTATE ARRAY LEFT BY K POSITIONS
    // ------------------------------------------------------------
    // TASK:
    // Rotate every element to the LEFT by k positions.
    
    // Example:
    // Input:  1 2 3 4 5
    // k = 2
    
    // Output: 3 4 5 1 2
    
    // Think:
    // Elements that fall off the LEFT side move to the RIGHT.
    // ------------------------------------------------------------

    MyArray a3;

    a3.insert(0, 1);
    a3.insert(1, 2);
    a3.insert(2, 3);
    a3.insert(3, 4);
    a3.insert(4, 5);

    cout << "\nOriginal: ";
    a3.display();

    a3.rotateLeft(2);

    cout << "After rotating left by 2: ";
    a3.display();


    // ------------------------------------------------------------
    // 4. ROTATE ARRAY RIGHT BY K POSITIONS
    // ------------------------------------------------------------
    // TASK:
    // Rotate every element to the RIGHT by k positions.
    //
    // Example:
    // Input:  1 2 3 4 5
    // k = 2
    //
    // Output: 4 5 1 2 3
    //
    // Think:
    // Elements that fall off the RIGHT side move to the LEFT.
    // ------------------------------------------------------------

    MyArray a4;

    a4.insert(0, 1);
    a4.insert(1, 2);
    a4.insert(2, 3);
    a4.insert(3, 4);
    a4.insert(4, 5);

    cout << "\nOriginal: ";
    a4.display();

    a4.rotateRight(2);

    cout << "After rotating right by 2: ";
    a4.display();


    // ------------------------------------------------------------
    // 5. MOVE ALL ZEROS TO THE END
    // ------------------------------------------------------------
    // TASK:
    // Move every zero to the END of the array.
    //
    // IMPORTANT:
    // Preserve the original order of all NON-ZERO elements.
    //
    // Example:
    // Input:  0 1 0 3 12 0 5
    // Output: 1 3 12 5 0 0 0
    // ------------------------------------------------------------

    MyArray a5;

    a5.insert(0, 0);
    a5.insert(1, 1);
    a5.insert(2, 0);
    a5.insert(3, 3);
    a5.insert(4, 12);
    a5.insert(5, 0);
    a5.insert(6, 5);

    cout << "\nOriginal: ";
    a5.display();

    a5.moveZerosToEnd();

    cout << "After moving zeros: ";
    a5.display();


    // // ------------------------------------------------------------
    // // 6. MOVE ALL NEGATIVE NUMBERS TO THE END
    // // ------------------------------------------------------------
    // // TASK:
    // // Move every negative number to the END of the array.
    // //
    // // IMPORTANT:
    // // Preserve the order of the remaining elements.
    // //
    // // Example:
    // // Input:  4 -2 7 -5 3 -1
    // // Output: 4 7 3 -2 -5 -1
    // // ------------------------------------------------------------

    // MyArray a6;

    // a6.insert(0, 4);
    // a6.insert(1, -2);
    // a6.insert(2, 7);
    // a6.insert(3, -5);
    // a6.insert(4, 3);
    // a6.insert(5, -1);

    // cout << "\nOriginal: ";
    // a6.display();

    // a6.moveNegativesToEnd();

    // cout << "After moving negatives: ";
    // a6.display();


    // // ------------------------------------------------------------
    // // 7. MOVE ELEMENTS MATCHING A CONDITION TO THE END
    // // ------------------------------------------------------------
    // // TASK:
    // // Move every element satisfying a given condition to the END.
    // //
    // // For this test, the condition is:
    // // "The number is EVEN."
    // //
    // // IMPORTANT:
    // // Preserve the relative order of the other elements.
    // //
    // // Example:
    // // Input:  4 9 2 7 6 3
    // // Output: 9 7 3 4 2 6
    // //
    // // The even numbers (4, 2, 6) move to the end.
    // // ------------------------------------------------------------

    // MyArray a7;

    // a7.insert(0, 4);
    // a7.insert(1, 9);
    // a7.insert(2, 2);
    // a7.insert(3, 7);
    // a7.insert(4, 6);
    // a7.insert(5, 3);

    // cout << "\nOriginal: ";
    // a7.display();

    // a7.moveEvenToEnd();

    // cout << "After moving even numbers: ";
    // a7.display();


    // // ------------------------------------------------------------
    // // 8. SEPARATE EVEN AND ODD NUMBERS
    // // ------------------------------------------------------------
    // // TASK:
    // // Rearrange the array so that:
    // //
    // //     1. ALL EVEN numbers come first.
    // //     2. ALL ODD numbers come afterward.
    // //
    // // Example:
    // // Input:  7 2 9 4 6 3
    // // Output: 2 4 6 7 9 3
    // // ------------------------------------------------------------

    // MyArray a8;

    // a8.insert(0, 7);
    // a8.insert(1, 2);
    // a8.insert(2, 9);
    // a8.insert(3, 4);
    // a8.insert(4, 6);
    // a8.insert(5, 3);

    // cout << "\nOriginal: ";
    // a8.display();

    // a8.separateEvenOdd();

    // cout << "Even first, odd after: ";
    // a8.display();


    // // ------------------------------------------------------------
    // // 9. FIND ALL PAIRS THAT SUM TO A TARGET
    // // ------------------------------------------------------------
    // // TASK:
    // // Find and print every PAIR of elements whose sum equals
    // // the given target.
    // //
    // // Target = 9
    // //
    // // Example:
    // // Input:  2 7 4 5 3 8 1
    // //
    // // Valid pairs:
    // // 2 + 7 = 9
    // // 4 + 5 = 9
    // // 8 + 1 = 9
    // // ------------------------------------------------------------

    // MyArray a9;

    // a9.insert(0, 2);
    // a9.insert(1, 7);
    // a9.insert(2, 4);
    // a9.insert(3, 5);
    // a9.insert(4, 3);
    // a9.insert(5, 8);
    // a9.insert(6, 1);

    // cout << "\nArray: ";
    // a9.display();

    // cout << "Pairs that sum to 9:\n";
    // a9.printPairsWithSum(9);


    // // ------------------------------------------------------------
    // // 10. FIND ALL PEAKS
    // // ------------------------------------------------------------
    // // TASK:
    // // Find every element that is GREATER than BOTH of its
    // // immediate neighbours.
    // //
    // // Example:
    // // Input:  1 5 2 7 3 9 4
    // //
    // // Peaks:
    // // 5  (greater than 1 and 2)
    // // 7  (greater than 2 and 3)
    // // 9  (greater than 3 and 4)
    // //
    // // NOTE:
    // // The first and last elements do NOT have two neighbours,
    // // so they are not considered peaks.
    // // ------------------------------------------------------------

    // MyArray a10;

    // a10.insert(0, 1);
    // a10.insert(1, 5);
    // a10.insert(2, 2);
    // a10.insert(3, 7);
    // a10.insert(4, 3);
    // a10.insert(5, 9);
    // a10.insert(6, 4);

    // cout << "\nArray: ";
    // a10.display();

    // cout << "Peaks: ";
    // a10.printPeaks();


    // // ------------------------------------------------------------
    // // 11. FIND ALL VALLEYS
    // // ------------------------------------------------------------
    // // TASK:
    // // Find every element that is LESS than BOTH of its
    // // immediate neighbours.
    // //
    // // Example:
    // // Input:  9 3 7 2 8 1 6
    // //
    // // Valleys:
    // // 3  (less than 9 and 7)
    // // 2  (less than 7 and 8)
    // // 1  (less than 8 and 6)
    // //
    // // Again, the first and last elements are not considered.
    // // ------------------------------------------------------------

    // MyArray a11;

    // a11.insert(0, 9);
    // a11.insert(1, 3);
    // a11.insert(2, 7);
    // a11.insert(3, 2);
    // a11.insert(4, 8);
    // a11.insert(5, 1);
    // a11.insert(6, 6);

    // cout << "\nArray: ";
    // a11.display();

    // cout << "Valleys: ";
    // a11.printValleys();


    // // ------------------------------------------------------------
    // // 12. COUNT ELEMENTS GREATER THAN THE ARRAY AVERAGE
    // // ------------------------------------------------------------
    // // TASK:
    // // Calculate the average of all elements.
    // // Then count how many elements are GREATER than that average.
    // //
    // // Example:
    // // Input: 2 4 6 8 10
    // //
    // // Average = 6
    // //
    // // Elements greater than 6:
    // // 8, 10
    // //
    // // Answer: 2
    // // ------------------------------------------------------------

    // MyArray a12;

    // a12.insert(0, 2);
    // a12.insert(1, 4);
    // a12.insert(2, 6);
    // a12.insert(3, 8);
    // a12.insert(4, 10);

    // cout << "\nArray: ";
    // a12.display();

    // cout << "Count greater than average: "
    //      << a12.countGreaterThanAverage() << endl;


    // // ------------------------------------------------------------
    // // 13. FREQUENCY COUNT OF EVERY DISTINCT ELEMENT
    // // ------------------------------------------------------------
    // // TASK:
    // // Count how many times EVERY distinct value appears.
    // //
    // // Example:
    // // Input: 4 2 4 7 2 4 9
    // //
    // // Output:
    // // 4 -> 3
    // // 2 -> 2
    // // 7 -> 1
    // // 9 -> 1
    // //
    // // Each distinct value should be reported once.
    // // ------------------------------------------------------------

    // MyArray a13;

    // a13.insert(0, 4);
    // a13.insert(1, 2);
    // a13.insert(2, 4);
    // a13.insert(3, 7);
    // a13.insert(4, 2);
    // a13.insert(5, 4);
    // a13.insert(6, 9);

    // cout << "\nArray: ";
    // a13.display();

    // cout << "Frequencies:\n";
    // a13.printFrequency();


    // // ------------------------------------------------------------
    // // 14. PRINT ELEMENTS THAT APPEAR EXACTLY ONCE
    // // ------------------------------------------------------------
    // // TASK:
    // // Print only the elements whose frequency is EXACTLY 1.
    // //
    // // Example:
    // // Input: 4 2 4 7 9 2 5
    // //
    // // Frequencies:
    // // 4 -> 2
    // // 2 -> 2
    // // 7 -> 1
    // // 9 -> 1
    // // 5 -> 1
    // //
    // // Output:
    // // 7 9 5
    // // ------------------------------------------------------------

    // MyArray a14;

    // a14.insert(0, 4);
    // a14.insert(1, 2);
    // a14.insert(2, 4);
    // a14.insert(3, 7);
    // a14.insert(4, 9);
    // a14.insert(5, 2);
    // a14.insert(6, 5);

    // cout << "\nArray: ";
    // a14.display();

    // cout << "Appearing exactly once: ";
    // a14.printExactlyOnce();


    // // ------------------------------------------------------------
    // // 15. PRINT ELEMENTS THAT APPEAR EXACTLY N TIMES
    // // ------------------------------------------------------------
    // // TASK:
    // // Given a number N, print every element that appears
    // // EXACTLY N times.
    // //
    // // Here N = 3.
    // //
    // // Example:
    // // Input: 4 2 4 7 4 2 9 7
    // //
    // // Frequencies:
    // // 4 -> 3
    // // 2 -> 2
    // // 7 -> 2
    // // 9 -> 1
    // //
    // // Output:
    // // 4
    // // ------------------------------------------------------------

    // MyArray a15;

    // a15.insert(0, 4);
    // a15.insert(1, 2);
    // a15.insert(2, 4);
    // a15.insert(3, 7);
    // a15.insert(4, 4);
    // a15.insert(5, 2);
    // a15.insert(6, 9);
    // a15.insert(7, 7);

    // cout << "\nArray: ";
    // a15.display();

    // cout << "Appearing exactly 3 times: ";
    // a15.printExactlyNTimes(3);


    // // ============================================================
    // // KTH LARGEST
    // // ============================================================


    // ------------------------------------------------------------
    // 16. FIND THE KTH LARGEST ELEMENT
    // ------------------------------------------------------------
    // TASK:
    // Return the kth largest element in the array.
    //
    // Example:
    // Input:
    // 12 5 8 20 3 15 10
    //
    // Sorted from largest to smallest:
    // 20 15 12 10 8 5 3
    //
    // Therefore:
    // 1st largest = 20
    // 2nd largest = 15
    // 3rd largest = 12
    // 5th largest = 8
    // ------------------------------------------------------------

    MyArray a16;

    a16.insert(0, 12);
    a16.insert(1, 5);
    a16.insert(2, 8);
    a16.insert(3, 20);
    a16.insert(4, 3);
    a16.insert(5, 15);
    a16.insert(6, 10);

    cout << "\nArray: ";
    a16.display();

    cout << "1st largest: " << a16.kthLargest(1) << endl;
    cout << "2nd largest: " << a16.kthLargest(2) << endl;
    cout << "3rd largest: " << a16.kthLargest(3) << endl;
    cout << "5th largest: " << a16.kthLargest(5) << endl;


    // ------------------------------------------------------------
    // 17. FIND 1ST, 2ND, AND 3RD LARGEST
    // ------------------------------------------------------------
    // TASK:
    // Find the:
    //
    //     1. Largest element
    //     2. Second largest element
    //     3. Third largest element
    //
    // Example:
    // Input: 6 14 2 19 11 7
    //
    // Largest       = 19
    // Second largest = 14
    // Third largest  = 11
    // ------------------------------------------------------------

    MyArray a17;

    a17.insert(0, 6);
    a17.insert(1, 14);
    a17.insert(2, 2);
    a17.insert(3, 19);
    a17.insert(4, 11);
    a17.insert(5, 7);

    cout << "\nArray: ";
    a17.display();

    cout << "1st largest: " << a17.firstLargest() << endl;
    cout << "2nd largest: " << a17.secondLargest() << endl;
    cout << "3rd largest: " << a17.thirdLargest() << endl;


    // // ============================================================
    // // EDGE CASES
    // // ============================================================


    // // ------------------------------------------------------------
    // // 18. ROTATION WHERE K IS GREATER THAN ARRAY SIZE
    // // ------------------------------------------------------------
    // // TASK:
    // // Rotate the array left by K positions even when K is
    // // GREATER than the array's size.
    // //
    // // Example:
    // // Array: 1 2 3 4 5
    // // K = 7
    // //
    // // Since 7 % 5 = 2, this is equivalent to rotating by 2.
    // //
    // // Expected:
    // // 3 4 5 1 2
    // // ------------------------------------------------------------

    // MyArray edge1;

    // edge1.insert(0, 1);
    // edge1.insert(1, 2);
    // edge1.insert(2, 3);
    // edge1.insert(3, 4);
    // edge1.insert(4, 5);

    // cout << "\nOriginal: ";
    // edge1.display();

    // edge1.rotateLeft(7);

    // cout << "After rotating left by 7: ";
    // edge1.display();


    // // ------------------------------------------------------------
    // // 19. ROTATE BY ZERO
    // // ------------------------------------------------------------
    // // TASK:
    // // Rotating an array by 0 positions should NOT change it.
    // //
    // // Input:
    // // 10 20 30
    // //
    // // Expected:
    // // 10 20 30
    // // ------------------------------------------------------------

    // MyArray edge2;

    // edge2.insert(0, 10);
    // edge2.insert(1, 20);
    // edge2.insert(2, 30);

    // edge2.rotateRight(0);

    // cout << "\nRotate by 0: ";
    // edge2.display();


    // // ------------------------------------------------------------
    // // 20. ALL ZEROS
    // // ------------------------------------------------------------
    // // TASK:
    // // Move zeros to the end when EVERY element is already zero.
    // //
    // // Input:
    // // 0 0 0 0
    // //
    // // Expected:
    // // 0 0 0 0
    // //
    // // This checks that your method doesn't accidentally change
    // // the array size or lose elements.
    // // ------------------------------------------------------------

    // MyArray edge3;

    // edge3.insert(0, 0);
    // edge3.insert(1, 0);
    // edge3.insert(2, 0);
    // edge3.insert(3, 0);

    // edge3.moveZerosToEnd();

    // cout << "\nAll zeros: ";
    // edge3.display();


    // // ------------------------------------------------------------
    // // 21. NO DUPLICATES
    // // ------------------------------------------------------------
    // // TASK:
    // // Remove duplicates when there ARE NO duplicates.
    // //
    // // Input:
    // // 1 2 3 4
    // //
    // // Expected:
    // // 1 2 3 4
    // // ------------------------------------------------------------

    // MyArray edge4;

    // edge4.insert(0, 1);
    // edge4.insert(1, 2);
    // edge4.insert(2, 3);
    // edge4.insert(3, 4);

    // edge4.removeDuplicates();

    // cout << "\nNo duplicates: ";
    // edge4.display();


    // // ------------------------------------------------------------
    // // 22. ALL VALUES ARE DUPLICATES
    // // ------------------------------------------------------------
    // // TASK:
    // // Remove duplicates when EVERY element has the same value.
    // //
    // // Input:
    // // 7 7 7 7
    // //
    // // Expected:
    // // 7
    // //
    // // Only one copy should remain.
    // // ------------------------------------------------------------

    // MyArray edge5;

    // edge5.insert(0, 7);
    // edge5.insert(1, 7);
    // edge5.insert(2, 7);
    // edge5.insert(3, 7);

    // edge5.removeDuplicates();

    // cout << "\nAll duplicates: ";
    // edge5.display();


    // // ------------------------------------------------------------
    // // 23. KTH LARGEST WITH NEGATIVE NUMBERS
    // // ------------------------------------------------------------
    // // TASK:
    // // Find the kth largest value when ALL values are negative.
    // //
    // // Input:
    // // -10 -3 -20 -1 -7
    // //
    // // Sorted largest -> smallest:
    // // -1 -3 -7 -10 -20
    // //
    // // Therefore:
    // // 1st largest = -1
    // // 2nd largest = -3
    // // 4th largest = -10
    // //
    // // IMPORTANT:
    // // Do NOT assume that 0 is the largest starting value.
    // // ------------------------------------------------------------

    // MyArray edge6;

    // edge6.insert(0, -10);
    // edge6.insert(1, -3);
    // edge6.insert(2, -20);
    // edge6.insert(3, -1);
    // edge6.insert(4, -7);

    // cout << "\nNegative array: ";
    // edge6.display();

    // cout << "1st largest: " << edge6.kthLargest(1) << endl;
    // cout << "2nd largest: " << edge6.kthLargest(2) << endl;
    // cout << "4th largest: " << edge6.kthLargest(4) << endl;


    // // ------------------------------------------------------------
    // // 24. KTH LARGEST WITH DUPLICATES
    // // ------------------------------------------------------------
    // // TASK:
    // // Find the kth largest element when duplicate values exist.
    // //
    // // Input:
    // // 10 5 10 8 5 3
    // //
    // // Sorted from largest to smallest:
    // // 10 10 8 5 5 3
    // //
    // // Therefore:
    // // 1st largest = 10
    // // 2nd largest = 10
    // // 3rd largest = 8
    // //
    // // Here, DUPLICATES COUNT as separate positions.
    // // ------------------------------------------------------------

    // MyArray edge7;

    // edge7.insert(0, 10);
    // edge7.insert(1, 5);
    // edge7.insert(2, 10);
    // edge7.insert(3, 8);
    // edge7.insert(4, 5);
    // edge7.insert(5, 3);

    // cout << "\nArray with duplicates: ";
    // edge7.display();

    // cout << "1st largest: " << edge7.kthLargest(1) << endl;
    // cout << "2nd largest: " << edge7.kthLargest(2) << endl;
    // cout << "3rd largest: " << edge7.kthLargest(3) << endl;


    // return 0;
}