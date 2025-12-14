#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int cata, cab, mouz;
    cin >> cata >> cab >> mouz;

    int da = abs(mouz - cata);
    int db = abs(mouz - cab);

    if (da == db){
        cout << "Mouse C\n";
    } 
    else if (da < db){
        cout << "Cat A\n";
    } 
    else {
        cout << "Cat B\n";
    }
}

signed main(){
    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}
