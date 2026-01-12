#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    string sus;
    cin >> sus;
    int count=0, cont=0;

    if (sus[0] == 'u') count++;
    sus[0] = 's';
    if(sus[sus.size()-1] == 'u') count ++;
    sus[sus.size()-1] = 's';

    int i=1, n=sus.size();
    while(i<n-1){
        if(sus[i] == 'u'){
            int cnt = 0;
            while(i<n-1 && sus[i] == 'u'){
                cnt++;
                i++;
            }
            count += (cnt+1)/2;
        }else{
            i++;
        }
    }

    cout << count<< '\n';
}

signed main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}
