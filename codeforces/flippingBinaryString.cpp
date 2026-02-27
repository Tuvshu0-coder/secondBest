#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;

	while (t--) {
		int n;
		cin >> n;

		string s;
		cin >> s;

		vector<int> ones;
		vector<int> zeros;

		for (int i = 0; i < n; i++) {
			if (s[i] == '1') {
				ones.push_back(i + 1);
			} else {
				zeros.push_back(i + 1);
			}
		}

		if (static_cast<int>(ones.size()) % 2 == 0) {
			cout << ones.size() << "\n";
			for (int i = 0; i < static_cast<int>(ones.size()); i++) {
				if (i) {
					cout << ' ';
				}
				cout << ones[i];
			}
			if (!ones.empty()) {
				cout << "\n";
			}
		} else if (static_cast<int>(zeros.size()) % 2 == 1) {
			cout << zeros.size() << "\n";
			for (int i = 0; i < static_cast<int>(zeros.size()); i++) {
				if (i) {
					cout << ' ';
				}
				cout << zeros[i];
			}
			cout << "\n";
		} else {
			cout << -1 << "\n";
		}
	}

	return 0;
}
