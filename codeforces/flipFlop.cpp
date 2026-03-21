#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void solve(){
    int n;
		long long c, k;
		cin >> n >> c >> k;
		vector<long long> a(n);
		for (int i = 0; i < n; ++i) {
			cin >> a[i];
		}

		sort(a.begin(), a.end());
		vector<char> used(n, false);

		while (true) {
			long long best = -1;
			int bestIdx = -1;
			for (int i = 0; i < n; ++i) {
				if (!used[i] && a[i] <= c && a[i] > best) {
					best = a[i];
					bestIdx = i;
				}
			}

			if (bestIdx == -1) {
				break;
			}

			long long extra = min(k, c - best);
			c += best + extra;
			k -= extra;
			used[bestIdx] = true;
		}

		cout << c << "\n";
}

int main() {
	int t;
	cin >> t;

	while (t--) {
		solve();
	}

	return 0;
}
