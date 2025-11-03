#include <bits/stdc++.h>
using namespace std;

int main() {
    
    freopen("blocks.in", "r", stdin);
    freopen("blocks.out", "w", stdout);

    int N;
    cin >> N;

    vector<int> total(26, 0);
    for (int i = 0; i < N; i++) {
        string a, b;
        cin >> a >> b;

        vector<int> countA(26, 0);
        for (int j = 0; j < a.size(); j++) {
            char ch = a[j];
            countA[ch - 'a']++;
        }

        vector<int> countB(26, 0);
        for (int j = 0; j < b.size(); j++) {
            char ch = b[j];
            countB[ch - 'a']++;
        }

        for (int j = 0; j < 26; j++) {
            total[j] += max(countA[j], countB[j]);
        }
    }

    for (int i = 0; i < 26; i++) {
        cout << total[i] << '\n';
    }

    return 0;
}
