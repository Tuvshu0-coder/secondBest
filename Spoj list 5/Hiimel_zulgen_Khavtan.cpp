#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){

    int L, W, C;
    cin >> L >> W >> C;

    int horizontal = L / C;
    if(L%C !=0) horizontal++;
    int vertical = W/C;
    if(W%C !=0) vertical ++;

    int total = horizontal * vertical;
    cout << total << '\n';

}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}