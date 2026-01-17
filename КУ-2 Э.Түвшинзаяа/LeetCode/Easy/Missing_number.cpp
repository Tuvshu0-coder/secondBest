#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int left = 0;
        int right = nums.size() -1;
        int ans = nums.size();
        while(left <= right){
            int mid = left + (right - left)/2;
            if(mid == nums[mid]){
                left = mid + 1;
            }else{
                ans = mid;
                right = mid - 1;
            }
        }
        return ans;

    }
};