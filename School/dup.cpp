#include <bits/stdc++.h>
using namespace std;



int main(){
    int n, maxi=0, cur=1;
    cin >> n;
    vector<int> v(n);
    for(int i=0; i < n; i++){cin >> v[i];}
    for(int i = 1; i<=n; i++){
        if(v[i]==v[i-1]){
            cur++;
            if(cur > maxi){
                maxi = cur;
            }
        }else{
            cur = 1;
        }
    }
    cout << maxi;



    return 0;
}