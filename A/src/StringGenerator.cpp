#include "StringGenerator.h"
#include "common.h"

#include <algorithm>

const char* StringGenerator::arrayTypeName(ArrayType t) {
    switch (t) {
        case ArrayType::kRandom:        return "random";
        case ArrayType::kReverseSorted: return "reverse_sorted";
        case ArrayType::kAlmostSorted:  return "almost_sorted";
        case ArrayType::kPrefixHeavy:   return "prefix_heavy";
    }
    return "unknown";
}

StringGenerator::StringGenerator(std::uint64_t seed)
    : seed_(seed), rng_state_(seed) {}

std::uint64_t StringGenerator::nextRand() {
    std::uint64_t z = (rng_state_ += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

int StringGenerator::randInt(int lo, int hi) {
    std::uint64_t span = static_cast<std::uint64_t>(hi - lo + 1);
    return lo + static_cast<int>(nextRand() % span);
}

char StringGenerator::randChar() {
    const std::string& alpha = taskAlphabet();
    return alpha[nextRand() % alpha.size()];
}

std::string StringGenerator::randomString(int len) {
    std::string s(len, ' ');
    for (int i = 0; i < len; ++i) s[i] = randChar();
    return s;
}

void StringGenerator::buildMasters(int masterSize, int minLen, int maxLen) {
    master_size_ = masterSize;
    min_len_     = minLen;
    max_len_     = maxLen;
    rng_state_   = seed_;

    master_random_.resize(masterSize);
    for (int i = 0; i < masterSize; ++i) {
        int len = randInt(minLen, maxLen);
        master_random_[i] = randomString(len);
    }

    master_reverse_ = master_random_;
    std::sort(master_reverse_.begin(), master_reverse_.end());
    std::reverse(master_reverse_.begin(), master_reverse_.end());

    master_almost_ = master_random_;
    std::sort(master_almost_.begin(), master_almost_.end());
    int swaps = std::max(1, masterSize / 100);
    for (int s = 0; s < swaps; ++s) {
        int i = randInt(0, masterSize - 1);
        int j = randInt(0, masterSize - 1);
        std::swap(master_almost_[i], master_almost_[j]);
    }

    master_prefix_.resize(masterSize);
    int numPrefixes = std::max(1, masterSize / 50);
    std::vector<std::string> prefixes(numPrefixes);
    int prefLen = std::min(50, minLen + (maxLen - minLen) / 4);
    for (int p = 0; p < numPrefixes; ++p) {
        prefixes[p] = randomString(prefLen);
    }
    for (int i = 0; i < masterSize; ++i) {
        const std::string& pref = prefixes[nextRand() % prefixes.size()];
        int total = randInt(minLen, maxLen);
        int suff  = std::max(0, total - static_cast<int>(pref.size()));
        master_prefix_[i] = pref + randomString(suff);
    }
}

std::vector<std::string> StringGenerator::get(ArrayType type, int n) const {
    const std::vector<std::string>* src = nullptr;
    switch (type) {
        case ArrayType::kRandom:        src = &master_random_;  break;
        case ArrayType::kReverseSorted: src = &master_reverse_; break;
        case ArrayType::kAlmostSorted:  src = &master_almost_;  break;
        case ArrayType::kPrefixHeavy:   src = &master_prefix_;  break;
    }
    if (!src || n <= 0) return {};
    int take = std::min(n, static_cast<int>(src->size()));
    return std::vector<std::string>(src->begin(), src->begin() + take);
}
