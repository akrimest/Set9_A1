#include <bits/stdc++.h>
using namespace std;

static constexpr int R = 256;
static constexpr int ALPHABET_SIZE = 74;

static inline int charAtIdx(const vector<string>& src, int idx, int d) {
    const string& s = src[idx];
    if (d >= (int)s.size()) return -1;
    return (unsigned char)s[d];
}

static void sort3wayIdx(const vector<string>& src,
                        vector<int>& order, int lo, int hi, int d)
{
    if (hi - lo <= 1) return;
    int pivot = charAtIdx(src, order[(lo + hi) / 2], d);
    int lt = lo, gt = hi - 1;
    int i = lo;
    while (i <= gt) {
        int c = charAtIdx(src, order[i], d);
        if (c < pivot)      { swap(order[lt], order[i]); ++lt; ++i; }
        else if (c > pivot) { swap(order[i],  order[gt]); --gt;     }
        else                {                             ++i;      }
    }
    sort3wayIdx(src, order, lo, lt, d);
    if (pivot >= 0) sort3wayIdx(src, order, lt, gt + 1, d + 1);
    sort3wayIdx(src, order, gt + 1, hi, d);
}

static void msdQuickRec(const vector<string>& src,
                        vector<int>& order, vector<int>& aux,
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    if (n > 1) {
        vector<int> order(n), aux(n);
        for (int i = 0; i < n; ++i) order[i] = i;
        msdQuickRec(a, order, aux, 0, n, 0);
        vector<string> tmp(n);
        for (int i = 0; i < n; ++i) tmp[i] = std::move(a[order[i]]);
        a.swap(tmp);
    }

    for (int i = 0; i < n; ++i) cout << a[i] << '\n';
    return 0;
}
