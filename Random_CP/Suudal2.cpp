#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;
using ll = long long;

struct Segment {
	ll l;
	ll r;
	ll dist;
	ll seat;
};

static ll g_n = 0;

static Segment make_segment(ll l, ll r) {
	Segment s{l, r, 0, 0};
	if (l == 0) {
		s.seat = 1;
		s.dist = r - 1;
	} else if (r == g_n + 1) {
		s.seat = g_n;
		s.dist = g_n - l;
	} else {
		s.seat = (l + r) / 2;
		s.dist = (r - l) / 2;
	}
	return s;
}

struct SegmentBetter {
	bool operator()(const Segment& a, const Segment& b) const {
		if (a.dist != b.dist) return a.dist < b.dist; // max dist
		if (a.seat != b.seat) return a.seat > b.seat; // min seat
		return a.l > b.l; // deterministic
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ll n, k;
	if (!(cin >> n >> k)) return 0;
	g_n = n;

	set<ll> occupied;
	occupied.insert(0);
	occupied.insert(n + 1);

	priority_queue<Segment, vector<Segment>, SegmentBetter> pq;
	pq.push(make_segment(0, n + 1));

	vector<ll> arrival_seat;
	arrival_seat.reserve(100000);

	auto valid = [&](const Segment& s) {
		auto it = occupied.find(s.l);
		if (it == occupied.end()) return false;
		auto it2 = next(it);
		return it2 != occupied.end() && *it2 == s.r;
	};

	for (ll i = 0; i < k; ++i) {
		string cmd;
		cin >> cmd;
		if (cmd == "+") {
			while (!pq.empty() && !valid(pq.top())) pq.pop();
			Segment best = pq.top();
			pq.pop();

			ll seat = best.seat;
			cout << seat << '\n';

			occupied.insert(seat);
			arrival_seat.push_back(seat);

			if (seat - best.l > 1) pq.push(make_segment(best.l, seat));
			if (best.r - seat > 1) pq.push(make_segment(seat, best.r));
		} else {
			ll idx = stoll(cmd.substr(1)) - 1; // 0-based arrival index
			ll seat = arrival_seat[idx];

			auto it = occupied.find(seat);
			auto itL = prev(it);
			auto itR = next(it);
			ll l = *itL;
			ll r = *itR;
			occupied.erase(it);

			if (r - l > 1) pq.push(make_segment(l, r));
		}
	}

	return 0;
}
