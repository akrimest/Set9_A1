#pragma once

#include "StringGenerator.h"

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

class StringSortTester {
public:
    using SortFn = std::function<void(std::vector<std::string>&)>;

    struct Algorithm {
        std::string name;
        SortFn      fn;
    };

    struct RunResult {
        std::string  algorithm;
        std::string  array_type;
        int          n              = 0;
        long long    time_ns_median = 0;
        long long    time_ns_min    = 0;
        long long    time_ns_max    = 0;
        std::int64_t char_cmp       = 0;
        bool         correct        = true;
    };

    void registerAlgorithm(const std::string& name, SortFn fn);

    RunResult runOne(const Algorithm&               algo,
                     const std::vector<std::string>& arr,
                     StringGenerator::ArrayType     type,
                     int                            repeats = 5) const;

    void runMatrix(const StringGenerator&                              gen,
                   const std::vector<StringGenerator::ArrayType>&      types,
                   int nMin, int nMax, int nStep,
                   int repeats,
                   const std::string&                                   csvPath) const;

    const std::vector<Algorithm>& algorithms() const { return algorithms_; }

private:
    std::vector<Algorithm> algorithms_;
};
