#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;

	while (t--) {
		int n;
		long long h, k;
		cin >> n >> h >> k;

		vector<long long> a(n + 1, 0);
		for (int i = 1; i <= n; i++) {
			cin >> a[i];
		}

		vector<long long> prefix(n + 1, 0);
		vector<long long> prefMin(n + 1, 0);
		vector<long long> sufMax(n + 2, 0);

		prefMin[0] = (long long)4e18;
		for (int i = 1; i <= n; i++) {
			prefix[i] = prefix[i - 1] + a[i];
			prefMin[i] = min(prefMin[i - 1], a[i]);
		}

		sufMax[n + 1] = 0;
		for (int i = n; i >= 1; i--) {
			sufMax[i] = max(sufMax[i + 1], a[i]);
		}

		long long sum = prefix[n];
		long long bestTime = (long long)4e18;

		for (int r = 0; r <= n; r++) {
			long long bestPrefix = 0;

			if (r == 0) {
				bestPrefix = 0;
			} else if (r == n) {
				bestPrefix = sum;
			} else {
				long long maxOutside = sufMax[r + 1];
				long long gain = maxOutside - prefMin[r];
				if (gain < 0) {
					gain = 0;
				}
				bestPrefix = prefix[r] + gain;
			}

			long long remaining = h - bestPrefix;
			long long q = 0;
			if (remaining > 0) {
				q = (remaining + sum - 1) / sum;
			}

			long long m = q * n + r;
			if (m == 0) {
				continue;
			}

			long long reloads = 0;
			if (r == 0) {
				reloads = q - 1;
			} else {
				reloads = q;
			}

			long long time = m + reloads * k;
			if (time < bestTime) {
				bestTime = time;
			}
		}

		cout << bestTime << "\n";
	}

	return 0;
}
