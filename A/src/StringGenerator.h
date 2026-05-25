#pragma once

#include <cstdint>
#include <string>
#include <vector>

class StringGenerator {
public:
    enum class ArrayType {
        kRandom,
        kReverseSorted,
        kAlmostSorted,
        kPrefixHeavy,
    };

    static const char* arrayTypeName(ArrayType t);

    explicit StringGenerator(std::uint64_t seed = 20260525ULL);

    void buildMasters(int masterSize = 3000,
                      int minLen     = 10,
                      int maxLen     = 200);

    std::vector<std::string> get(ArrayType type, int n) const;

    std::string randomString(int len);

    int masterSize() const { return master_size_; }

private:
    std::uint64_t seed_;
    int master_size_ = 0;
    int min_len_     = 10;
    int max_len_     = 200;

    std::vector<std::string> master_random_;
    std::vector<std::string> master_reverse_;
    std::vector<std::string> master_almost_;
    std::vector<std::string> master_prefix_;

    std::uint64_t rng_state_ = 0;
    std::uint64_t nextRand();
    int           randInt(int lo, int hi);
    char          randChar();
};
