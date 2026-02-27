#include <iostream>
#include <vector>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >>t;
	while (t--) {
		int n;
		cin >> n;
		vector<int> a(n);
		for (int i = 0; i < n; ++i) {
			cin >> a[i];
		}

		bool non_decreasing = true;
		for (int i = 1; i < n; ++i) {
			if (a[i - 1] > a[i]) {
				non_decreasing = false;
				break;
			}
		}
        if(non_decreasing)
            cout << n << '\n';
        else
        cout << 1 << '\n';

	}

	return 0;
}
