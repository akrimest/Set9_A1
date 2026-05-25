#include "StringGenerator.h"
#include "StringSortTester.h"
#include "sorts.h"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    std::string csvPath = "data/results.csv";
    if (argc >= 2) csvPath = argv[1];

    StringGenerator gen(20260525ULL);
    gen.buildMasters(3000, 10, 200);
    std::cerr << "Master arrays built (size " << gen.masterSize() << ").\n";

    StringSortTester tester;
    tester.registerAlgorithm("std_quicksort",            stdQuicksort);
    tester.registerAlgorithm("std_mergesort",            stdMergesort);
    tester.registerAlgorithm("string_quicksort_3way",    stringQuicksort3way);
    tester.registerAlgorithm("string_mergesort_lcp",     stringMergesortLCP);
    tester.registerAlgorithm("msd_radix_sort",           msdRadixSort);
    tester.registerAlgorithm("msd_radix_quick_sort",     msdRadixQuickSort);

    std::vector<StringGenerator::ArrayType> types = {
        StringGenerator::ArrayType::kRandom,
        StringGenerator::ArrayType::kReverseSorted,
        StringGenerator::ArrayType::kAlmostSorted,
        StringGenerator::ArrayType::kPrefixHeavy,
    };

    tester.runMatrix(gen, types, 100, 3000, 100, 5, csvPath);

    std::cerr << "Done. Results written to: " << csvPath << "\n";
    return 0;
}
