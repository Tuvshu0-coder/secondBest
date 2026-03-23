#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int index=0, count=0;
    string s, hello="hello";
    cin >> s;
    
    for(int i=0; i<hello.size(); i++){

        for( ; index<s.size(); index++){
            if(s[index] == hello[i]){
                count++;
                break;
            }

        }
        index++;
    }
    if(count == 5) cout << "YES";
    else cout << "NO";


}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}