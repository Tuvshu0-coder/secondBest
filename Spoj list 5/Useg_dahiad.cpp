#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    unordered_map<char, int> george;
    string sergei;
    cin >> sergei;
    int sum=0;
    for(char c : sergei){
        if(george[c] > 0) sum++;
        else george[c]++;
    }
    cout << sum;

}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}