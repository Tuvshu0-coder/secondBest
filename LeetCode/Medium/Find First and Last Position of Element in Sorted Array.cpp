#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        vector<int> result = {-1, -1};
        int left = BinarySearch(nums, target, true);
        int right = BinarySearch(nums, target, false);
        result[0] = left;
        result[1] = right;
        return result;
    }
    int BinarySearch(vector<int>& nums, int target, bool isSearchingLeft){
        int left =0;
        int right = nums.size() - 1;
        int index = -1;
        while(left <= right){
            int mid = left + (right - left)/2;
            // if(nums[mid] == target) index = mid;

            if(nums[mid] < target){
                left = mid + 1;
            }else if(nums[mid] > target){
                right = mid - 1;
            }else{
                index = mid;
                if(isSearchingLeft){
                    right = mid -1;
                }else{
                    left = mid + 1;
                }
            }
        }
        return index;
    }
};