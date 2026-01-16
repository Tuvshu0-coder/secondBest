#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0;
        int right = nums.size()-1;
        int minimum = INT_MAX;
        while(left <= right){
            int mid = left + (right - left)/2;
            if(nums[mid] >= nums[left]){ // left side sorted
                minimum = min(minimum, nums[left]);
                left = mid + 1;
            }else{ // right side sorted
                minimum = min(minimum, nums[mid]);
                right = mid -1;
            }
        }
        return minimum;
    }
};