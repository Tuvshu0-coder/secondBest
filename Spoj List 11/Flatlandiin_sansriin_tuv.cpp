#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    cin >> n >> m;
    vector<int> arr(m);
    for(int &t : arr){cin >> t;}
    int maxa= INT_MIN;
    for(int i=0; i<n; i++){
        int mini= INT_MAX;
        for(int j=0; j<m; j++){
            mini = min(mini, abs(i-arr[j]));
        }
        maxa = max(maxa, mini);
    }
    cout << maxa;


    return 0;
}