#pragma once

#include <cstdint>
#include <string>
#include <vector>

inline std::int64_t g_char_cmp = 0;

inline void resetCharCmp()              { g_char_cmp = 0; }
inline std::int64_t getCharCmp()        { return g_char_cmp; }

inline int lcpCompare(const std::string& a, const std::string& b, int& k) {
    const int la = static_cast<int>(a.size());
    const int lb = static_cast<int>(b.size());
    while (k < la && k < lb) {
        ++g_char_cmp;
        if (a[k] != b[k]) {
            return (static_cast<unsigned char>(a[k]) <
                    static_cast<unsigned char>(b[k])) ? -1 : 1;
        }
        ++k;
    }
    if (la == lb) return 0;
    return (la < lb) ? -1 : 1;
}

inline int strCompareCounted(const std::string& a, const std::string& b) {
    int k = 0;
    return lcpCompare(a, b, k);
}

inline int charAt(const std::string& s, int d) {
    if (d >= static_cast<int>(s.size())) return -1;
    return static_cast<unsigned char>(s[d]);
}

constexpr int ALPHABET_SIZE = 74;

inline const std::string& taskAlphabet() {
    static const std::string kAlpha =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#%:;^&*()-.";
    return kAlpha;
}
