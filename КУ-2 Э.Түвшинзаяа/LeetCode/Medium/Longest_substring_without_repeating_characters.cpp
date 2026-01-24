#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int left = 0;
        int maximum = 0;
        unordered_set<char> charSet;

        for(int right = 0; right < s.size(); right++){
            while(charSet.find(s[right]) != charSet.end()){
                charSet.erase(s[left]);
                left++;
            }
            charSet.insert(s[right]);
            maximum = max(maximum, right - left + 1); 
        } 
        return maximum;
    }
};