
#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int T, k;
	if (!(cin >> T >> k)) {
		return 0;
	}

	while (T--) {
		int N;
		string S;
		cin >> N >> S;

		cout << "YES\n";
		if (k == 1) {
			string K(N, 'M');
			int flip = 0;
			for (int i = N - 1; i >= 0; --i) {
				char c = S[i];
				if (flip) {
					c = (c == 'M') ? 'O' : 'M';
				}
				if (c == 'M') {
					K[i] = 'M';
				} else {
					K[i] = 'O';
					flip ^= 1;
				}
			}
			cout << K << "\n";
		}
	}

	return 0;
}
