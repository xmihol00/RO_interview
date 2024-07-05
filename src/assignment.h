#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>   // sort
#include <numeric>     // transform_reduce
#include <execution>   // execution::par
#include <immintrin.h> // AVX instructions
#include <bit>         // countl_zero
#include <queue>       // queue
#include <omp.h>       // OpenMP functions

#include "INode.h"

int getClosestToZero(const std::vector<int>& arr);

std::size_t countChunks(const std::vector<int>& arr);

int getLevelSum(const INode& root, std::size_t n);

std::vector<std::size_t> getReversalsToSort(const std::vector<int>& arr);