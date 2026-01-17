#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    bool isPerfectSquare(int num) {
        long long left = 0;
        long long right = num;  // use long long to prevent overflow
        
        for(int i = 0; i < 28; i++){
            long long mid = left + (right - left) / 2;
            long long square = mid * mid;  // store as long long
            
            if(square == num) return true;
            if(square > num){
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        return false;
    }
};
