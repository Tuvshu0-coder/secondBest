#include <bits/stdc++.h>
using namespace std;
int main(){
    freopen("diamond.in", "r", stdin);
    freopen("diamond.out", "w", stdout);
    int N, K;
    cin >> N >> K;
    vector<int> arr(N);
    for(int &t : arr){cin >> t;}

    sort(arr.begin(), arr.end());
    
    int maxDia = 0;
    int i=0;
    
    for(int j=0; j<N; j++){    // j++ if delta is lower than K
        while(arr[j]-arr[i] > K ){ // if the delta is higher then i++
            i++;
        }
        maxDia = max(maxDia, j - i + 1); // j(front pointer) - i(back pointer) THE DELTA finding the max
    }
    cout << maxDia;


    return 0;
}