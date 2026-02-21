// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
using namespace std;
#define int long long

void solve(){
    int N;
    cin >> N;

    bool found = false;

    for(int i = 0; i < N; i++){
        int x;
        cin >> x;
        if(x == 67){
            found = true;
        }
    }

    if(found) cout << "YES\n";
    else cout << "NO\n";
}


signed main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}