#include <bits/stdc++.h>
using namespace std;

static int lcpCompare(const string& a, const string& b, int& k) {
    const int la = (int)a.size(), lb = (int)b.size();
    while (k < la && k < lb) {
        if (a[k] != b[k]) {
            return ((unsigned char)a[k] < (unsigned char)b[k]) ? -1 : 1;
        }
        ++k;
    }
    if (la == lb) return 0;
    return (la < lb) ? -1 : 1;
}

static void mergeLCP(vector<string>& a, vector<int>& lcp,
                     vector<string>& tA, vector<int>& tL,
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

static void rec(vector<string>& a, vector<int>& lcp,
                vector<string>& tA, vector<int>& tL,
                int lo, int hi)
{
    if (hi - lo <= 1) { if (lo < hi) lcp[lo] = 0; return; }
    int mid = (lo + hi) / 2;
    rec(a, lcp, tA, tL, lo, mid);
    rec(a, lcp, tA, tL, mid, hi);
    mergeLCP(a, lcp, tA, tL, lo, mid, hi);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    if (n > 1) {
        vector<int> lcp(n, 0);
        vector<string> tA(n);
        vector<int> tL(n, 0);
        rec(a, lcp, tA, tL, 0, n);
    }

    for (int i = 0; i < n; ++i) cout << a[i] << '\n';
    return 0;
}
