#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, T;
    cin >> N >> T;
    vector<int> sergei(N), data(T);
    for(int &t : sergei){cin >> t;}
    for(int i=0; i<T; i++){
        int a, b, mini=INT_MAX;
        cin >> a >> b;
        for(int j=a; j<=b; j++){
            mini = min(mini, sergei[j]);
        }
        data[i]=mini;
    }
    for(int t : data){cout << t << '\n';}



    return 0;
}