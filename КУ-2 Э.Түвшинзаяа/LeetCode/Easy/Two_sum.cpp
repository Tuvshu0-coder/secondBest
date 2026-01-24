#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> num_map;
        int N = nums.size();

        for(int i=0; i<N; i++){
            int complement = target - nums[i];
            if(num_map.count(complement)){
                return {num_map[complement], i};
            }
            num_map[nums[i]] = i;
        }
        return {};
    }
};