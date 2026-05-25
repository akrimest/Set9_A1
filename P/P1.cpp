#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

struct Node {
    long long freq;
    char ch;
    Node* left;
    Node* right;
    Node(long long f, char c) : freq(f), ch(c), left(nullptr), right(nullptr) {}
    Node(long long f, Node* l, Node* r) : freq(f), ch(0), left(l), right(r) {}
};

struct Cmp {
    bool operator()(Node* a, Node* b) const { return a->freq > b->freq; }
};

void traverse(Node* node, string& code, map<char, string>& codes) {
    if (!node->left && !node->right) {
        codes[node->ch] = code.empty() ? "0" : code;
        return;
    }
    if (node->left) {
        code.push_back('0');
        traverse(node->left, code, codes);
        code.pop_back();
    }
    if (node->right) {
        code.push_back('1');
        traverse(node->right, code, codes);
        code.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);

    map<char, long long> freq;
    for (char c : s) freq[c]++;

    if (freq.empty()) {
        cout << "0 0\n\n";
        return 0;
    }

    priority_queue<Node*, vector<Node*>, Cmp> pq;
    for (auto& p : freq) pq.push(new Node(p.second, p.first));

    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node(a->freq + b->freq, a, b));
    }

    Node* root = pq.top();
    map<char, string> codes;
    string buf;
    traverse(root, buf, codes);

    long long total = 0;
    for (auto& p : codes) total += freq[p.first] * (long long)p.second.size();

    cout << codes.size() << ' ' << total << '\n';
    for (auto& p : codes) cout << p.first << ": " << p.second << '\n';

    string encoded;
    encoded.reserve(total);
    for (char c : s) encoded += codes[c];
    cout << encoded << '\n';

    return 0;
}
