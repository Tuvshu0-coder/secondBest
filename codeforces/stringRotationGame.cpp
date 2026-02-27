#include <iostream>
#include <string>
using namespace std;

int main() {
    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        string s;
        cin >> s;

        int transitions = 0;
        bool hasEqualEdge = false;

        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            if (s[i] != s[j]) {
                transitions++;
            } else {
                hasEqualEdge = true;
            }
        }

        int result = hasEqualEdge ? transitions + 1 : transitions;
        cout << result << endl;
    }

    return 0;
}