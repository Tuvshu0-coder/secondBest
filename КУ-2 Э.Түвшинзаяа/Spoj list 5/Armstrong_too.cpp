#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    
    int number, sum=0, temp, power;
    cin >> number;
    power = log10(number) + 1;
    temp = number;
    while(number>0){
        int numba = pow((number%10), power);
        sum+=numba;
        number/=10;
    }
    if(temp == sum) cout <<"YES";
    else cout << "NO"; 


}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}