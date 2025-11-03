#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> nums(N);
    for(int &t : nums){
        cin >> t;
    }
    int target;
    cin >> target;
    int n = nums.size();
 
    for (int mask = 0; mask < (1 << n); ++mask) {
        int sum = 0;
        vector<int> combo;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                sum += nums[i];
                combo.push_back(nums[i]);
            }
        }
        if (sum == target) {
            for (int x : combo) cout << x << " ";
            cout << endl;
        }
    }
}
