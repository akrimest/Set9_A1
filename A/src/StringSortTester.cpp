#include "StringSortTester.h"
#include "common.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

void StringSortTester::registerAlgorithm(const std::string& name, SortFn fn) {
    algorithms_.push_back({name, std::move(fn)});
}

static bool isSorted(const std::vector<std::string>& a) {
    for (size_t i = 1; i < a.size(); ++i) {
        if (a[i] < a[i - 1]) return false;
    }
    return true;
}

StringSortTester::RunResult StringSortTester::runOne(
    const Algorithm&                algo,
    const std::vector<std::string>& arr,
    StringGenerator::ArrayType      type,
    int                             repeats) const
{
    RunResult r;
    r.algorithm  = algo.name;
    r.array_type = StringGenerator::arrayTypeName(type);
    r.n          = static_cast<int>(arr.size());

    if (repeats < 1) repeats = 1;
    std::vector<long long> times;
    times.reserve(repeats);

    std::int64_t lastCmp = 0;

    for (int it = 0; it < repeats; ++it) {
        std::vector<std::string> copy = arr;

        resetCharCmp();
        auto t0 = std::chrono::steady_clock::now();
        algo.fn(copy);
        auto t1 = std::chrono::steady_clock::now();

        long long ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
        times.push_back(ns);
        lastCmp = getCharCmp();

        if (it == 0 && !isSorted(copy)) r.correct = false;
    }

    std::sort(times.begin(), times.end());
    r.time_ns_min    = times.front();
    r.time_ns_max    = times.back();
    r.time_ns_median = times[times.size() / 2];
    r.char_cmp       = lastCmp;
    return r;
}

void StringSortTester::runMatrix(
    const StringGenerator&                          gen,
    const std::vector<StringGenerator::ArrayType>&  types,
    int nMin, int nMax, int nStep,
    int repeats,
    const std::string&                              csvPath) const
{
    std::ofstream out(csvPath);
    if (!out) {
        std::cerr << "ERROR: cannot open " << csvPath << " for writing\n";
        return;
    }
    out << "algorithm,array_type,n,time_ns_median,time_ns_min,time_ns_max,char_comparisons,correct\n";
    out.flush();

    long long doneRuns  = 0;
    const long long stepsCount = static_cast<long long>((nMax - nMin) / nStep + 1);
    const long long totalRuns  = static_cast<long long>(types.size()) *
                                 stepsCount *
                                 static_cast<long long>(algorithms_.size());

    for (auto type : types) {
        for (int n = nMin; n <= nMax; n += nStep) {
            auto arr = gen.get(type, n);
            for (const auto& algo : algorithms_) {
                auto r = runOne(algo, arr, type, repeats);
                out << r.algorithm   << ','
                    << r.array_type  << ','
                    << r.n           << ','
                    << r.time_ns_median << ','
                    << r.time_ns_min    << ','
                    << r.time_ns_max    << ','
                    << r.char_cmp       << ','
                    << (r.correct ? "1" : "0") << '\n';
                out.flush();
                ++doneRuns;
                if (doneRuns % 10 == 0 || doneRuns == totalRuns) {
                    std::cerr << "[" << doneRuns << "/" << totalRuns << "] "
                              << r.algorithm << " | " << r.array_type
                              << " | n=" << r.n
                              << " | t=" << r.time_ns_median << "ns"
                              << " | cmp=" << r.char_cmp
                              << (r.correct ? "" : " !!! INCORRECT")
                              << "\n";
                }
            }
        }
    }
}
