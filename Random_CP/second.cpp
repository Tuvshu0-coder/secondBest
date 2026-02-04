
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N;
	int K;
	cin >> N >> K;

	int totalMasks = 1 << N;
	vector<long long> diff(totalMasks, 0);

	for (int i = 0; i < K; ++i) {
		int x, y, z;
		cin >> x >> y >> z;
		--x;
		--y;
		--z;
		int A = (1 << y) | (1 << z);
		int B = (1 << x);
		diff[A] += 1;
		diff[A | B] -= 1;
	}

	for (int bit = 0; bit < N; ++bit) {
		for (int mask = 0; mask < totalMasks; ++mask) {
			if (mask & (1 << bit)) {
				diff[mask] += diff[mask ^ (1 << bit)];
			}
		}
	}

	long long best = -1;
	long long count = 0;
	for (int mask = 0; mask < totalMasks; ++mask) {
		long long score = diff[mask];
		if (score > best) {
			best = score;
			count = 1;
		} else if (score == best) {
			count += 1;
		}
	}

	cout << best << " " << count;
	return 0;
}
