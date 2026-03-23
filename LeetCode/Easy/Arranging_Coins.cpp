#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    int arrangeCoins(int n) {
        long long left= 0;
        long long right = n;
        long long ans;
        while(left <= right){
            long long mid = left + (right - left)/2;
            long long capacity = mid*(mid + 1)/2;
            if(capacity == n) return mid;
            if(capacity < n){
                ans = mid;
                left = mid + 1;
            }else{
                right = mid -1;
            }
        }
        return ans;
    }
};