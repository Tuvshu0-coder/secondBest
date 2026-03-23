#include <bits/stdc++.h>
using namespace std;

void solve(){
    int N;
    cin >> N;

    for (int x = N; x >= 0; x--) {
        if (x % 3 == 0 && (N - x) % 5 == 0) {

            cout << string(x, '5') + string(N - x, '3') << "\n";
            return;
        }
    }
    cout << "-1\n"; 
}

int main() {
    int T; 
    cin >> T;
    while (T--)
        solve();
    return 0;
}
