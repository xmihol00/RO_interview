#include "INode.h"
#include "assignment.h"

#if !defined(_APPROACH_1_) && !defined(_APPROACH_2_) && !defined(_APPROACH_3_)
    #define _APPROACH_1_ // default approach
#endif

#if 0 // change to 1 to activate all approaches - just for better readability in IDEs like VSC
    #define _APPROACH_1_
    #define _APPROACH_2_
    #define _APPROACH_3_
#endif

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

#ifdef _APPROACH_1_
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

#ifdef _APPROACH_2_
    // STL approach: Find just the minimum element based on 2 sorting keys, use parallelism
    return *min_element(execution::par, arr.begin(), arr.end(), [](int a, int b) 
    { 
        return abs(a) < abs(b) ||           // first key - sort based on the distance from 0
               (abs(a) == abs(b) && a > b); // second key - sort based on the sign
    });
    // Time complexity: O(n)
    // Space complexity: O(1)
#endif

#ifdef _APPROACH_3_
    using int32_16_t = __m512i;

    // OpenMP + AVX approach: Use OpenMP for parallelism and AVX for vectorization
    int smallestDistance = INT32_MAX;
    #pragma omp parallel // start threads
    {
        // distribute the data among threads
        size_t elementsPerThread = arr.size() / omp_get_num_threads();
        size_t threadOffset = elementsPerThread * omp_get_thread_num();
        const int *data = arr.data() + threadOffset;

        // assume data are aligned to 64 bytes - must be, otherwise the program will crash
        const int32_16_t *dataVector = reinterpret_cast<const int32_16_t *>(data);
        int32_16_t threadSmallestDistances = _mm512_set1_epi32(INT32_MAX); 
        constexpr int SIMD_LEN = 16;
        
        for (size_t i = 0; i < elementsPerThread / SIMD_LEN; i++)
        {
            int32_16_t batch = _mm512_load_si512(dataVector + i);

            // calculate the absolute values of elements in a vector
            int32_16_t absBatch = _mm512_abs_epi32(batch);
            int32_16_t absSmallestBatch = _mm512_abs_epi32(threadSmallestDistances);
            
            // perform the 2-key comparison as in the 2nd approach but on a vector
            uint16_t smallerBitmap = _mm512_cmp_epi32_mask(absBatch, absSmallestBatch, _MM_CMPINT_LT);
            uint16_t equalBitmap = _mm512_cmp_epi32_mask(absBatch, absSmallestBatch, _MM_CMPINT_EQ);
            uint16_t largerBitmap = _mm512_cmp_epi32_mask(batch, threadSmallestDistances, _MM_CMPINT_GT);
            uint16_t finalBitmap = smallerBitmap | (equalBitmap & largerBitmap);

            // update the smallest distances based on the comparison results
            threadSmallestDistances = _mm512_mask_mov_epi32(threadSmallestDistances, finalBitmap, batch);
        }

        const int *lanes = reinterpret_cast<const int *>(&threadSmallestDistances);
        // reduce the vector of 16 elements to a single element
        int threadSmallestDistance = *min_element(lanes, lanes + SIMD_LEN, [](int a, int b) { return abs(a) < abs(b) || (abs(a) == abs(b) && a > b); }); 

        #pragma omp critical // reduce the results among threads, avoid race condition
        {
            smallestDistance = abs(threadSmallestDistance) < abs(smallestDistance) || 
                              (abs(threadSmallestDistance) == abs(smallestDistance) && threadSmallestDistance > smallestDistance) ? 
                                    threadSmallestDistance : 
                                    smallestDistance;
        }
    }
    return smallestDistance;
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
size_t countChunks(const vector<int>& arr)
{
    if (arr.empty())
    {
        return 0;
    }

#ifdef _APPROACH_1_
    size_t chunkCount = 0;
    bool inChunk = false;
    for (const int &value : arr)
    {
        // avoid if statements, branch prediction would be often wrong (depends on the data distribution)
        bool notZero = value != 0;
        chunkCount += !inChunk && notZero; // true when non-zero element is reached after a sequence of zeroes, otherwise false
        inChunk = notZero;
    }
    return chunkCount;
    // Time complexity: O(n)
    // Space complexity: O(1)
#endif

#ifdef _APPROACH_2_
    // Parallel approach: Use transformation, reduction and STL parallelism.
    // Identify indices where chunks start and assign them 1, assign 0 to others.
    // Perform reduction on such transformed array.
    return transform_reduce(execution::par, arr.begin() + 1, arr.end(), arr.begin(), static_cast<int>(arr[0] != 0), 
                            plus{}, [](int a, int b) { return a != 0 && b == 0; }); 
    // Time complexity: O(log(n)) - with enough parallelism
    // Space complexity: O(1)
#endif

#ifdef _APPROACH_3_
    // Single core approach, but with AVX vectorization
    // Parallelism could be added as well similarly to the 3rd approach in the 'getClosestToZero' function
    using int32_16_t = __m512i;
    
    // assume the data is aligned to 64 bytes - must be, otherwise the program will crash
    const int32_16_t *data = reinterpret_cast<const int32_16_t *>(arr.data());
    size_t chunkCount = 0;
    bool inChunk = false;
    constexpr int SIMD_LEN = 16;
    
    for (size_t i = 0; i < arr.size() / SIMD_LEN; i++)
    {
        int32_16_t batch = _mm512_load_si512(data + i);
        uint16_t cmpBitmap = _mm512_cmp_epi32_mask(batch, _mm512_setzero_si512(), _MM_CMPINT_EQ); // compare 16 elements with zero using single instruction

        // assume zeros are quite rare, then the branch prediction should be quite good and the following code won't be executed often
        if (cmpBitmap) // at least one zero in the batch
        {
            int *batchData = reinterpret_cast<int *>(&batch);
            for (size_t j = 0; j < SIMD_LEN; j++) // perform the same operation as in the 1st approach, but only on the batch of 16 elements
            {
                bool notZero = batchData[j] != 0;
                chunkCount += !inChunk && notZero;
                inChunk = notZero;
            }
        }
        else if (!inChunk) // no zero in the batch and not currently not in any chunk
        {
            chunkCount++;
            inChunk = true;
        }
    }
    return chunkCount;
    // Time complexity: remains O(n), but the algorithm is potentially faster due to vectorization
#endif
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
int getLevelSum(const INode& root, size_t n)
{
#ifdef _APPROACH_1_
    // Simple recursive approach, i.e. depth-first traversal.
    if (n == 0) // required level reached - base case
    {
        return root.value();
    }
    else
    {
        int sum = 0;
        for (const auto &child : root.children())
        {
            if (child != nullptr)
            {
                sum += getLevelSum(*child, n - 1); // recursion
            }
        }
        return sum;
    }
#endif

#ifdef _APPROACH_2_
    // Non-recursive approach, let's use a queue, therefore breadth-first traversal.
    queue<pair<const INode *, size_t>> nodeQueue;
    nodeQueue.push({&root, n});
    size_t sum = 0;

    while (!nodeQueue.empty())
    {
        // dequeue the first node
        const INode *node = nodeQueue.front().first;
        size_t level = nodeQueue.front().second;
        nodeQueue.pop();

        if (level == 0) // required level reached
        {
            sum += node->value();
        }
        else
        {
            for (const auto &child : node->children()) // enqueue all valid children
            {
                if (child != nullptr)
                {
                    nodeQueue.push({child.get(), level - 1});
                }
            }
        }
    }
    return sum;
#endif

#ifdef _APPROACH_3_
    // OpenMP approach, depth-first traversal similar to the 1st approach but with task parallelism.
    function<int(const INode&, size_t)> taskLambda = [&taskLambda](const INode& root, size_t n) -> int
    {
        if (n == 0) // required level reached - base case
        {
            return root.value();
        }
        else
        {
            int sum = 0;
            for (const auto &child : root.children())
            {
                if (child != nullptr)
                {
                    INode *childPtr = child.get();
                    #pragma omp task shared(sum) firstprivate(childPtr) // parallelize the work - spawn a task
                    #pragma omp atomic update  // avoid race condition
                    sum += taskLambda(*childPtr, n - 1); // recursion
                }
            }

            #pragma omp taskwait // wait for all spawned tasks to finish before returning the result
                                 // actually, the tasks are dispatched to the waiting threads at this point
            return sum;
        }
    };

    int sum = 0;
    #pragma omp parallel
    {
        #pragma omp master // spawn the tasks by a single thread
        sum = taskLambda(root, n);
    }
    return sum;
#endif
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
vector<size_t> getReversalsToSort(const vector<int>& arr)
{
    // Let's use some kind of divide and conquer approach.
    // We can interpret sorting as repeatedly placing an element to its correct position in a sorted array, i.e. insertion sort.
    // Therefore, the algorithm can be simplified to repeatedly emplacing yet an unsorted element using the reversals.
    // Let's look at some examples (computation indicated in columns):
    /*  
        a)        b)        c)          d)          e)            f) 
        1 3 4 2   1 2 4 3   1 2 3 5 4   1 2 4 5 3   1 2 3 5 6 4   1 2 3 4 6 5
        4 3 1 2   4 2 1 3   5 3 2 1 4   5 4 2 1 3   6 5 3 2 1 4   6 4 3 2 1 5
        2 1 3 4   3 1 2 4   4 1 2 3 5   3 1 2 4 5   4 1 2 3 5 6   5 1 2 3 4 6
        1 2 3 4   2 1 3 4   3 2 1 4 5   2 1 3 4 5   3 2 1 4 5 6   4 3 2 1 5 6
                  1 2 3 4   1 2 3 4 5   1 2 3 4 5   1 2 3 4 5 6   1 2 3 4 5 6 
    */ 
    // The emerging pattern seems to be the following:
    /*
        1. Reverse all the already sorted elements,
        2. reverse all elements including the new element,
        3. find the correct position (counting from 1) N of the new element in the sorted array,
        4. reverse N elements,
        5. reverse N-1 elements.
    */
    // Without any proof, this algorithm should work for any input array.   

    // standard binary search in a sorted array returning an index, where the searched element is located or should be inserted at
    auto binaryIdxSearch = [](const vector<int>& sorted, int element) -> size_t
    {
        size_t left = 0;
        size_t right = sorted.size();
        while (left < right)
        {
            size_t middle = (left + right) >> 1; // halve the search interval
            if (sorted[middle] < element)
            {
                left = middle + 1;
            }
            else
            {
                right = middle;
            }
        }
        return left;
    };

    vector<size_t> reversals;
    vector<int> sorted;
    sorted.push_back(arr[0]);
    for (size_t i = 1; i < arr.size(); i++)
    {
        // the simplest solution
        // of course, there are many ways how to generate less reversals
        reversals.push_back(i);
        reversals.push_back(i + 1);
        //size_t correctIdx = binaryIdxSearch(sorted, arr[i]);
        size_t correctIdx = lower_bound(sorted.begin(), sorted.end(), arr[i]) - sorted.begin();
        reversals.push_back(correctIdx + 1);
        reversals.push_back(correctIdx); // assume that 0 elements can be reversed for simplicity
        sorted.insert(sorted.begin() + correctIdx, arr[i]); 
    }
    // Time complexity (of this algorithm): O(n*log(n)) if 'insert' is in O(log(n)) or faster, otherwise O(n^2), i.e. this case with STL vector
    // Space complexity: O(n) - both 'reversals' and 'sorted' grow linearly with the input array

    // The following code is just post-processing of the reversals vector to make it more efficient.
    for (size_t &reversal : reversals)
    {
        if (reversal == 1) // mark reversals of a single element as useless
        {
            reversal = 0;
        }
    }

    // remove redundant reversals, i.e. 2 same consecutive reversals
    bool modified = true;
    while (modified) // each removal can create a new redundant reversals
    {
        modified = false;
        size_t lastValue = 0;
        size_t lastIdx = 0;
        for (size_t i = 0; i < reversals.size(); i++)
        {
            if (reversals[i] != 0) // skip the already removed reversals
            {
                if (reversals[i] == lastValue) // two consecutive same reversals
                {
                    // mark the pair of reversals as useless
                    reversals[lastIdx] = 0;
                    reversals[i] = 0;

                    modified = true;
                }
                lastValue = reversals[i];
                lastIdx = i;
            }
        }
    }

    // pack the vector (move all non-zeros to the front with 'remove' and truncate the vector with 'erase')
    reversals.erase(remove(reversals.begin(), reversals.end(), 0), reversals.end());
    
    return reversals;
}
