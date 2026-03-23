#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    int findKthPositive(vector<int>& arr, int k) {
        int left=0;
        int right = arr.size() - 1;
        while(left <= right){
            int mid = left + (right - left)/2;
            int miss = arr[mid] - (mid + 1);
            if(miss < k ){
                left = mid + 1;
            }else{
                right = mid - 1;
            }
        }
        return k + left;
    }
};