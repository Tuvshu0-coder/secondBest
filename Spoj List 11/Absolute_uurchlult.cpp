#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;

        if (k == 0) {
            for (int i = 1; i <= n; i++)
                cout << i << " ";
            cout << "\n";
            continue;
        }

        if (n % (2 * k) != 0) {
            cout << -1 << "\n";
            continue;
        }

        vector<int> result(n + 1);
        bool add = true;
        for (int i = 1; i <= n; i++) {
            if (add) result[i] = i + k;
            else result[i] = i - k;

            if (i % k == 0) add = !add;  // flip direction after every k numbers
        }

        for (int i = 1; i <= n; i++)
            cout << result[i] << " ";
        cout << "\n";
    }
}
