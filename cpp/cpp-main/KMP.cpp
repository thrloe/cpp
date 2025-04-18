#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> prefix_function(const string& s) {
    int n = s.size();
    vector<int> P(n);
    for (int i = 1; i < n; i++) {
        int j = P[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = P[j - 1];
        }
        if (s[i] == s[j]) {
            j++;
        }
        P[i] = j;
    }
    return P;
}

int main() {
    string text, pattern;
    cin >> text >> pattern;

    if (pattern.empty() || text.empty()) {
        cout << "empty" << endl;
        return 0;
    }

    string combo = pattern + '#' + text;
    vector<int> P = prefix_function(combo);

    for (int i = pattern.size() + 1; i < P.size(); i++) {
        if (P[i] == pattern.size()) {
            cout << "yay" << endl;
            return 0;
        }
    }

    cout << "nay" << endl;
    return 0;
}