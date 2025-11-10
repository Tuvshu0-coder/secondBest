#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> problems(n);
    for (int &x : problems) cin >> x;

    int count = 0;
    int page = 1;

    for (int i = 0; i < n; i++) {
        int total = problems[i];
        for (int j = 1; j <= total; j++) {
            if (j == page) count++;
            if (j % k == 0 && j != total) page++;  // move to next page after k problems
        }
        page++; // new page for next chapter
    }

    cout << count;
    return 0;
}
