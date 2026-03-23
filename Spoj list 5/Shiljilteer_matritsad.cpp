#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int N, sum=0;
    cin >> N;
    while(N!=1){
        if(N%2==0){
            N/=2;
        }else{
            N = N*3 + 1;
        }
        sum++;
    }
    cout << sum;


}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}