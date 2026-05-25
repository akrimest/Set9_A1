#include "sorts.h"
#include "common.h"

#include <algorithm>
#include <array>
#include <utility>

namespace {

void quicksortRec(std::vector<std::string>& a, int lo, int hi) {
    while (hi - lo > 1) {
        std::string pivot = a[(lo + hi) / 2];
        int i = lo, j = hi - 1;
        while (i <= j) {
            while (strCompareCounted(a[i], pivot) < 0) ++i;
            while (strCompareCounted(a[j], pivot) > 0) --j;
            if (i <= j) { std::swap(a[i], a[j]); ++i; --j; }
        }
        if (j - lo < hi - i) {
            quicksortRec(a, lo, j + 1);
            lo = i;
        } else {
            quicksortRec(a, i, hi);
            hi = j + 1;
        }
    }
}

}

void stdQuicksort(std::vector<std::string>& a) {
    if (a.size() <= 1) return;
    quicksortRec(a, 0, static_cast<int>(a.size()));
}

namespace {

void mergeStd(std::vector<std::string>& a, std::vector<std::string>& buf,
              int lo, int mid, int hi) {
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (strCompareCounted(a[i], a[j]) <= 0)
            buf[k++] = std::move(a[i++]);
        else
            buf[k++] = std::move(a[j++]);
    }
    while (i < mid) buf[k++] = std::move(a[i++]);
    while (j < hi)  buf[k++] = std::move(a[j++]);
    for (int t = lo; t < hi; ++t) a[t] = std::move(buf[t]);
}

void mergesortRec(std::vector<std::string>& a, std::vector<std::string>& buf,
                  int lo, int hi) {
    if (hi - lo <= 1) return;
    int mid = (lo + hi) / 2;
    mergesortRec(a, buf, lo, mid);
    mergesortRec(a, buf, mid, hi);
    mergeStd(a, buf, lo, mid, hi);
}

}

void stdMergesort(std::vector<std::string>& a) {
    int n = static_cast<int>(a.size());
    if (n <= 1) return;
    std::vector<std::string> buf(n);
    mergesortRec(a, buf, 0, n);
}

namespace {

void sort3way(std::vector<std::string>& a, int lo, int hi, int d) {
    if (hi - lo <= 1) return;
    int pivot = charAt(a[(lo + hi) / 2], d);
    int lt = lo, gt = hi - 1;
    int i = lo;
    while (i <= gt) {
        int c = charAt(a[i], d);
        ++g_char_cmp;
        if (c < pivot)      { std::swap(a[lt], a[i]); ++lt; ++i; }
        else if (c > pivot) { std::swap(a[i],  a[gt]); --gt;     }
        else                {                          ++i;      }
    }
    sort3way(a, lo, lt, d);
    if (pivot >= 0) sort3way(a, lt, gt + 1, d + 1);
    sort3way(a, gt + 1, hi, d);
}

}

void stringQuicksort3way(std::vector<std::string>& a) {
    if (a.size() <= 1) return;
    sort3way(a, 0, static_cast<int>(a.size()), 0);
}

namespace {

void mergeLCP(std::vector<std::string>& a, std::vector<int>& lcp,
              std::vector<std::string>& tA, std::vector<int>& tL,
              int lo, int mid, int hi)
{
    int i = lo, j = mid, k = lo;
    int h = 0;
    int prevLcp = 0;
    bool first = true;

    while (i < mid && j < hi) {
        int cmp = lcpCompare(a[i], a[j], h);

        if (cmp <= 0) {
            tA[k] = std::move(a[i]);
            tL[k] = first ? 0 : prevLcp;
            first = false;
            ++k; ++i;
            if (i == mid) { prevLcp = h; break; }

            int lL = lcp[i];
            if (lL > h) {
                while (i < mid && lcp[i] > h) {
                    tA[k] = std::move(a[i]);
                    tL[k] = lcp[i];
                    ++k; ++i;
                }
                if (i == mid) { prevLcp = h; break; }
                if (lcp[i] < h) { prevLcp = h; h = lcp[i]; }
                else            { prevLcp = h; }
            } else if (lL < h) {
                prevLcp = h; h = lL;
            } else {
                prevLcp = h;
            }
        } else {
            tA[k] = std::move(a[j]);
            tL[k] = first ? 0 : prevLcp;
            first = false;
            ++k; ++j;
            if (j == hi) { prevLcp = h; break; }

            int lR = lcp[j];
            if (lR > h) {
                while (j < hi && lcp[j] > h) {
                    tA[k] = std::move(a[j]);
                    tL[k] = lcp[j];
                    ++k; ++j;
                }
                if (j == hi) { prevLcp = h; break; }
                if (lcp[j] < h) { prevLcp = h; h = lcp[j]; }
                else            { prevLcp = h; }
            } else if (lR < h) {
                prevLcp = h; h = lR;
            } else {
                prevLcp = h;
            }
        }
    }

    while (i < mid) {
        tA[k] = std::move(a[i]);
        tL[k] = first ? 0 : prevLcp;
        first = false;
        ++k; ++i;
        if (i < mid) prevLcp = lcp[i];
    }
    while (j < hi) {
        tA[k] = std::move(a[j]);
        tL[k] = first ? 0 : prevLcp;
        first = false;
        ++k; ++j;
        if (j < hi) prevLcp = lcp[j];
    }

    for (int t = lo; t < hi; ++t) {
        a[t]   = std::move(tA[t]);
        lcp[t] = tL[t];
    }
    lcp[lo] = 0;
}

void stringMergeRec(std::vector<std::string>& a, std::vector<int>& lcp,
                    std::vector<std::string>& tA, std::vector<int>& tL,
                    int lo, int hi) {
    if (hi - lo <= 1) {
        if (lo < hi) lcp[lo] = 0;
        return;
    }
    int mid = (lo + hi) / 2;
    stringMergeRec(a, lcp, tA, tL, lo, mid);
    stringMergeRec(a, lcp, tA, tL, mid, hi);
    mergeLCP(a, lcp, tA, tL, lo, mid, hi);
}

}

void stringMergesortLCP(std::vector<std::string>& a) {
    int n = static_cast<int>(a.size());
    if (n <= 1) return;
    std::vector<int>         lcp(n, 0);
    std::vector<std::string> tA(n);
    std::vector<int>         tL(n, 0);
    stringMergeRec(a, lcp, tA, tL, 0, n);
}

namespace {

constexpr int R = 256;

inline int charAtIdx(const std::vector<std::string>& src, int idx, int d) {
    return charAt(src[idx], d);
}

void msdRec(const std::vector<std::string>& src,
            std::vector<int>& order, std::vector<int>& aux,
            int lo, int hi, int d)
{
    if (hi - lo <= 1) return;

    int count[R + 2];
    for (int k = 0; k <= R + 1; ++k) count[k] = 0;

    for (int i = lo; i < hi; ++i) {
        int c = charAtIdx(src, order[i], d);
        ++count[c + 2];
    }
    for (int k = 1; k <= R + 1; ++k) count[k] += count[k - 1];

    int dist[R + 2];
    for (int k = 0; k <= R + 1; ++k) dist[k] = count[k];

    for (int i = lo; i < hi; ++i) {
        int c = charAtIdx(src, order[i], d);
        aux[lo + dist[c + 1]++] = order[i];
    }
    for (int i = lo; i < hi; ++i) order[i] = aux[i];

    for (int b = 1; b <= R; ++b) {
        int s = lo + count[b];
        int e = lo + count[b + 1];
        if (e - s > 1) msdRec(src, order, aux, s, e, d + 1);
    }
}

void sort3wayIdx(const std::vector<std::string>& src,
                 std::vector<int>& order, int lo, int hi, int d)
{
    if (hi - lo <= 1) return;
    int pivot = charAtIdx(src, order[(lo + hi) / 2], d);
    int lt = lo, gt = hi - 1;
    int i = lo;
    while (i <= gt) {
        int c = charAtIdx(src, order[i], d);
        ++g_char_cmp;
        if (c < pivot)      { std::swap(order[lt], order[i]); ++lt; ++i; }
        else if (c > pivot) { std::swap(order[i],  order[gt]); --gt;     }
        else                {                                  ++i;      }
    }
    sort3wayIdx(src, order, lo, lt, d);
    if (pivot >= 0) sort3wayIdx(src, order, lt, gt + 1, d + 1);
    sort3wayIdx(src, order, gt + 1, hi, d);
}

void msdQuickRec(const std::vector<std::string>& src,
                 std::vector<int>& order, std::vector<int>& aux,
                 int lo, int hi, int d)
{
    if (hi - lo <= 1) return;

    if (hi - lo < ALPHABET_SIZE) {
        sort3wayIdx(src, order, lo, hi, d);
        return;
    }

    int count[R + 2];
    for (int k = 0; k <= R + 1; ++k) count[k] = 0;

    for (int i = lo; i < hi; ++i) {
        int c = charAtIdx(src, order[i], d);
        ++count[c + 2];
    }
    for (int k = 1; k <= R + 1; ++k) count[k] += count[k - 1];

    int dist[R + 2];
    for (int k = 0; k <= R + 1; ++k) dist[k] = count[k];

    for (int i = lo; i < hi; ++i) {
        int c = charAtIdx(src, order[i], d);
        aux[lo + dist[c + 1]++] = order[i];
    }
    for (int i = lo; i < hi; ++i) order[i] = aux[i];

    for (int b = 1; b <= R; ++b) {
        int s = lo + count[b];
        int e = lo + count[b + 1];
        if (e - s > 1) msdQuickRec(src, order, aux, s, e, d + 1);
    }
}

void applyOrder(std::vector<std::string>& a, const std::vector<int>& order) {
    std::vector<std::string> tmp(a.size());
    for (size_t i = 0; i < a.size(); ++i) tmp[i] = std::move(a[order[i]]);
    a = std::move(tmp);
}

}

void msdRadixSort(std::vector<std::string>& a) {
    int n = static_cast<int>(a.size());
    if (n <= 1) return;
    std::vector<int> order(n), aux(n);
    for (int i = 0; i < n; ++i) order[i] = i;
    msdRec(a, order, aux, 0, n, 0);
    applyOrder(a, order);
}

void msdRadixQuickSort(std::vector<std::string>& a) {
    int n = static_cast<int>(a.size());
    if (n <= 1) return;
    std::vector<int> order(n), aux(n);
    for (int i = 0; i < n; ++i) order[i] = i;
    msdQuickRec(a, order, aux, 0, n, 0);
    applyOrder(a, order);
}
