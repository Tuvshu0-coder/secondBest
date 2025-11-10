#include <bits/stdc++.h>
using namespace std;

bool alreadyHappy(const string &s) {
    int n = s.size();
    for (int i = 0; i < n; i++) {
        bool leftSame  = (i > 0 && s[i] == s[i - 1]);
        bool rightSame = (i < n - 1 && s[i] == s[i + 1]);
        if (!leftSame && !rightSame) return false;
    }
    return true;
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        int n;
        string s;
        cin >> n >> s;

        bool hasEmpty = s.find('_') != string::npos;
        map<char,int> freq;
        for (char c : s) if (c != '_') freq[c]++;

        bool possible = true;
        for (auto &[c, count] : freq)
            if (count == 1) { possible = false; break; }

        if (hasEmpty) {
            cout << (possible ? "YES\n" : "NO\n");
        } else {
            cout << (alreadyHappy(s) ? "YES\n" : "NO\n");
        }
    }
    return 0;
}
