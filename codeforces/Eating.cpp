// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;

	while (t--) {
		int n;
		cin >> n;
		vector<int> a(n);
		for (int i = 0; i < n; ++i)  cin >> a[i];
		

		set<int> winners;
		for (int start = 0; start < n; ++start) {
			vector<int> rem = a;
			int last = -1, i = start, total = 0;
			for (int v : rem) total += v;
			while (total > 0) {
				if (rem[i] > 0) {
					--rem[i];
					--total;
					last = i;
				}
				i = (i + 1) % n;
			}
			winners.insert(last);
		}

		cout << winners.size() << '\n';
	}

	return 0;
}
