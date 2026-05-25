#include <bits/stdc++.h>
using namespace std;

static inline int charAt(const string& s, int d) {
    if (d >= (int)s.size()) return -1;
    return (unsigned char)s[d];
}

static void sort3way(vector<string>& a, int lo, int hi, int d) {
    if (hi - lo <= 1) return;
    int pivot = charAt(a[(lo + hi) / 2], d);
    int lt = lo, gt = hi - 1;
    int i = lo;
    while (i <= gt) {
        int c = charAt(a[i], d);
        if (c < pivot)      { swap(a[lt], a[i]); ++lt; ++i; }
        else if (c > pivot) { swap(a[i],  a[gt]); --gt;     }
        else                {                     ++i;      }
    }
    sort3way(a, lo, lt, d);
    if (pivot >= 0) sort3way(a, lt, gt + 1, d + 1);
    sort3way(a, gt + 1, hi, d);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    if (n > 1) sort3way(a, 0, n, 0);

    for (int i = 0; i < n; ++i) cout << a[i] << '\n';
    return 0;
}
