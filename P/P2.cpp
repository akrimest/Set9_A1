#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);

    unordered_map<string, int> dict;
    dict.reserve(4096);
    for (int i = 0; i < 128; i++) {
        dict.emplace(string(1, (char)i), i);
    }

    int nextCode = 128;
    vector<int> output;
    string w;

    for (char c : s) {
        string wc = w;
        wc.push_back(c);
        if (dict.find(wc) != dict.end()) {
            w = std::move(wc);
        } else {
            output.push_back(dict[w]);
            dict.emplace(std::move(wc), nextCode++);
            w.assign(1, c);
        }
    }
    if (!w.empty()) output.push_back(dict[w]);

    cout << output.size() << '\n';
    for (size_t i = 0; i < output.size(); i++) {
        if (i) cout << ' ';
        cout << output[i];
    }
    cout << '\n';

    return 0;
}
