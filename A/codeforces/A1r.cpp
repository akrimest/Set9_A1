#include <bits/stdc++.h>
using namespace std;

static constexpr int R = 256;

static inline int charAtIdx(const vector<string>& src, int idx, int d) {
    const string& s = src[idx];
    if (d >= (int)s.size()) return -1;
    return (unsigned char)s[d];
}

static void msdRec(const vector<string>& src,
                   vector<int>& order, vector<int>& aux,
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
        msdRec(a, order, aux, 0, n, 0);
        vector<string> tmp(n);
        for (int i = 0; i < n; ++i) tmp[i] = std::move(a[order[i]]);
        a.swap(tmp);
    }

    for (int i = 0; i < n; ++i) cout << a[i] << '\n';
    return 0;
}
