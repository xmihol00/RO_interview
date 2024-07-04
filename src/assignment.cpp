#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>

#include "INode.h"

using namespace std; // let's avoid typing 'std::' everywhere

/*
 * General rules:
 * - You can use modern C++ features (anything that is supported by modern compilers)
 * - Try to validate input parameters if it makes sense, throw exceptions if needed
 * - You are allowed to use STL (so adding includes is allowed)
 */

/**
 * Implement a method that returns number that is closest to zero in a given vector.
 * If there are two equally close to zero elements like 2 and -2,
 * then consider the positive element to be "closer" to zero.
 */
int getClosestToZero(const vector<int>& arr) 
{
    if (arr.empty())
    {
        throw invalid_argument("Empty vector, 'getClosestToZero' expects at least one element in the input vector");
    }

#ifdef _NAIVE_APPROACH_
    // Naive approach: Sort the array (sorting does not have to be stable) and try to return non-negative element from the values with the smallest distance to zero
    vector<int> sortedArr(arr); // make a copy to avoid side effects, that makes the space complexity O(n)
    auto compareLambda = [](int a, int b) { return abs(a) < abs(b); };
    sort(sortedArr.begin(), sortedArr.end(), compareLambda); // O(n*log(n))
    int smallestDistance = sortedArr[0];
    for (size_t i = 1; smallestDistance < 0 && abs(smallestDistance) == abs(sortedArr[i]) && i < sortedArr.size(); smallestDistance = sortedArr[i++]);
        // for loop without body!
    return smallestDistance; 
    // Time complexity: STL should use some kind of quicksort, although that's a randomized algorithm, the amortized time complexity will be O(n*log(n)).
    //                  Additionally, the for loop will have a time complexity of O(n), so the overall time complexity is O(n*log(n))*O(n) = O(n^2*log(n)).
    // Space complexity: O(n).
#endif
}

/**
 * Please implement this method to return count of chunks in given array.
 *
 * Chunk is defined as continuous sequence of one or more non-zero numbers separated by one or more zeroes.
 * Input can contain leading and trailing zeroes.
 *
 * Example: [5, 4, 0, 0, -1, 0, 2, 0, 0] contains 3 chunks
 */
size_t countChunks(const vector<int>& arr) {
}

/**
 * Open INode.h to see the INode interface.
 *
 * Implement following function that shall traverse the tree,
 * and return the sum of the values (INode.value()) of all nodes
 * at the level 'n' in the tree.
 * 
 * Node root is assumed to be at the level 0. All its children are level 1, etc.
 */
int getLevelSum(const INode& root, size_t n) {
}

/**
 * Imagine a sort algorithm, that sorts array of integers by repeatedly reversing
 * the order of the first several elements of it.
 *
 * For example, to sort [12,13,11,14], you need to reverse the order of the first two (2)
 * elements. You will get [13,12,11,14].
 * Then you shall reverse the order of the first three (3) elements,
 * and you will get [11,12,13,14]
 *
 * For this assignment you shall implement function
 * that returns list of integers corresponding to the required reversals.
 * E.g. for given vector [12,13,11,14]
 * the function should return [2, 3].
 */
vector<size_t> getReversalsToSort(const vector<int>& arr) {
}
