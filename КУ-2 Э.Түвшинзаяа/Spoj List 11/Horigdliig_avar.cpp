#include <bits/stdc++.h>
using namespace std;

int main(){
    int t;
    cin >> t;
    vector<int> data;

    while(t--){
        int man, candy, start;
        cin >> man >> candy >> start;

        int ans = (candy + start - 1) % man;
        if (ans == 0) ans = man;  // if modulo gives 0, wrap around
        data.push_back(ans);     
    }

    for(int x : data){
        cout << x << '\n';
    }
    
    return 0;
}
