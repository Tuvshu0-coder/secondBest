#include <bits/stdc++.h>
using namespace std;
#define int long long


signed main(){
    int N;
    cin >> N;
    string hood;
    cin >> hood;
    
    set<char> types(hood.begin(), hood.end());
    int totalTypes = types.size();
    
    unordered_map<char, int> count;
    int left=0 , minWin = N;
    int currentTypes=0;

    for(int right = 0; right<N; right++){
        count[hood[right]]++;

        if(count[hood[right]] == 1) currentTypes++;

        while(totalTypes == currentTypes){
            minWin = min(minWin, right - left + 1);
            count[hood[left]]--;
            if(count[hood[left]]==0) currentTypes--;
            left++;
        }
    }
    cout << minWin;

}