#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int K, N;
    cin >> K >> N;
    
    int i=(K-1)/N;
    int j=(K-1)%N;

    if(i-1>=0) cout << (i-1)*N + j + 1 << " ";
    if(j+1<N)  cout << i*N + j + 2 <<" ";
    if(i+1<N) cout << (i+1)*N + j +1 <<" ";
    if(j-1>=0) cout << i*N +j <<" ";




}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
        solve();
    
}