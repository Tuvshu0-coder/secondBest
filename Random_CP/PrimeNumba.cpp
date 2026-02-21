#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
using i128 = __int128_t;

static int64 mul_mod(int64 a, int64 b, int64 mod) {
	return (int64)((i128)a * b % mod);
}

static int64 pow_mod(int64 a, int64 d, int64 mod) {
	int64 r = 1;
	while (d > 0) {
		if (d & 1) r = mul_mod(r, a, mod);
		a = mul_mod(a, a, mod);
		d >>= 1;
	}
	return r;
}

static bool is_prime(int64 n) {
	if (n < 2) return false;
	if (n % 2 == 0) return n == 2;
	if (n % 3 == 0) return n == 3;

	int64 d = n - 1;
	int s = 0;
	while ((d & 1) == 0) {
		d >>= 1;
		++s;
	}

	auto witness = [&](int64 a) {
		if (a % n == 0) return false;
		int64 x = pow_mod(a, d, n);
		if (x == 1 || x == n - 1) return false;
		for (int i = 1; i < s; ++i) {
			x = mul_mod(x, x, n);
			if (x == n - 1) return false;
		}
		return true;
	};

	// Deterministic for 64-bit integers.
	static const int64 bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
	for (int64 a : bases) {
		if (a >= n) break;
		if (witness(a)) return false;
	}
	return true;
}

static bool has_edge(int64 a, int64 b) {
	if (a == b) return false;
	if (a == 2 || b == 2) {
		int64 other = (a == 2) ? b : a;
		if (other <= 2) return false;
		return is_prime(other - 2);
	}
	int64 diff = a > b ? a - b : b - a;
	return diff == 2;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int64 A, B;
	if (!(cin >> A >> B)) return 0;

	const int64 LIMIT = (int64)1e15;

	vector<int64> candidates;
	candidates.push_back(A);
	if (B != A) candidates.push_back(B);
	if (A != 2 && B != 2) candidates.push_back(2);
	if (A == 2 || B == 2) candidates.push_back(2);

	auto add_if_prime = [&](int64 x) {
		if (x < 2 || x > LIMIT) return;
		if (is_prime(x)) candidates.push_back(x);
	};

	add_if_prime(A - 2);
	add_if_prime(A + 2);
	add_if_prime(B - 2);
	add_if_prime(B + 2);

	sort(candidates.begin(), candidates.end());
	candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

	int n = (int)candidates.size();
	vector<vector<int>> adj(n);
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (has_edge(candidates[i], candidates[j])) {
				adj[i].push_back(j);
				adj[j].push_back(i);
			}
		}
	}

	auto find_index = [&](int64 x) {
		auto it = lower_bound(candidates.begin(), candidates.end(), x);
		if (it == candidates.end() || *it != x) return -1;
		return (int)(it - candidates.begin());
	};

	int s = find_index(A);
	int t = find_index(B);
	if (s == -1 || t == -1) {
		cout << -1 << "\n";
		return 0;
	}

	vector<int> parent(n, -1);
	queue<int> q;
	q.push(s);
	parent[s] = s;
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		if (v == t) break;
		for (int to : adj[v]) {
			if (parent[to] != -1) continue;
			parent[to] = v;
			q.push(to);
		}
	}

	if (parent[t] == -1) {
		cout << -1 << "\n";
		return 0;
	}

	vector<int64> path;
	for (int v = t; v != s; v = parent[v]) {
		path.push_back(candidates[v]);
	}
	path.push_back(candidates[s]);
	reverse(path.begin(), path.end());

	cout << path.size() << "\n";
	for (size_t i = 0; i < path.size(); ++i) {
		if (i) cout << " ";
		cout << path[i];
	}
	cout << "\n";
	return 0;
}
