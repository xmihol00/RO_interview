#pragma once
#include <vector>
#include <cstdint>

#include "INode.h"

int getClosestToZero(const std::vector<int>& arr);

std::size_t countChunks(const std::vector<int>& arr);

int getLevelSum(const INode& root, std::size_t n);

std::vector<std::size_t> getReversalsToSort(const std::vector<int>& arr);