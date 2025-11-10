#include <bits/stdc++.h>
using namespace std;

int main() {
	freopen("cbarn.in", "r", stdin);
	freopen("cbarn.out", "w", stdout);

	int room_num;
	cin >> room_num;
	vector<int> rooms(room_num);
	int total_cows = 0;
	for (int r = 0; r < room_num; r++) {
		cin >> rooms[r];
		total_cows += rooms[r];
	}

	int min_dist = INT32_MAX;
	for (int unlock = 0; unlock < room_num; unlock++) {
		int dist = 0;
		int cows_left = total_cows;
		for (int r = 0; r < room_num; r++) {
			cows_left -= rooms[(unlock + r) % room_num];
			dist += cows_left;
		}
		min_dist = min(min_dist, dist);
	}

	cout << min_dist << endl;
}