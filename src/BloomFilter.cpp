#include "BloomFilter.h"

#include <algorithm>
#include <functional>

BloomFilter::BloomFilter(size_t m, size_t k) : m(m), k(k), bitArray(m, false) {}

void BloomFilter::clear() {
  std::fill(bitArray.begin(), bitArray.end(), false);
}

void BloomFilter::insert(const std::string &url) {
  std::vector<size_t> indices = getHashIndices(url);
  for (size_t index : indices) {
    bitArray[index] = true;
  }
}

bool BloomFilter::possiblyContains(const std::string &url) const {
  std::vector<size_t> indices = getHashIndices(url);
  for (size_t index : indices) {
    if (!bitArray[index]) {
      return false;
    }
  }
  return true;
}

std::vector<size_t> BloomFilter::getHashIndices(const std::string &url) const {
  std::vector<size_t> indices;
  indices.reserve(k);

  size_t h1 = std::hash<std::string>{}(url);
  size_t h2 = h1 ^ (h1 >> 16);

  for (size_t i = 0; i < k; ++i) {
    size_t combinedHash = h1 + i * h2;
    indices.push_back(combinedHash % m);
  }

  return indices;
}
