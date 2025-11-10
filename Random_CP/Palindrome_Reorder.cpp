#include <bits/stdc++.h>
using namespace std;

int main() {
    string s;
    cin >> s;

    vector<int> freq(26, 0);
    for (char c : s) freq[c - 'A']++;

    int oddCount = 0;
    char oddChar = 0;

    for (int i = 0; i < 26; i++) {
        if (freq[i] % 2 != 0) {
            oddCount++;
            oddChar = 'A' + i;
        }
    }

    if (oddCount > 1) {
        cout << "NO SOLUTION";
        return 0;
    }

    string half = "";
    for (int i = 0; i < 26; i++) {
        half += string(freq[i] / 2, 'A' + i);
    }

    string result = half;
    if (oddCount == 1)
        result += string(freq[oddChar - 'A'], oddChar);
    reverse(half.begin(), half.end());
    result += half;

    cout << result;
}
