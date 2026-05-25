#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;
    if (k <= 0) {
        cout << '\n';
        return 0;
    }

    vector<int> codes(k);
    for (int i = 0; i < k; i++) cin >> codes[i];

    vector<string> dict;
    dict.reserve(128 + k);
    for (int i = 0; i < 128; i++) dict.emplace_back(1, (char)i);

    string result;
    string prev = dict[codes[0]];
    result += prev;

    for (int i = 1; i < k; i++) {
        string entry;
        int code = codes[i];
        if (code < (int)dict.size()) {
            entry = dict[code];
        } else {
            entry = prev;
            entry.push_back(prev[0]);
        }
        result += entry;
        string newEntry = prev;
        newEntry.push_back(entry[0]);
        dict.push_back(std::move(newEntry));
        prev = std::move(entry);
    }

    cout << result << '\n';
    return 0;
}
