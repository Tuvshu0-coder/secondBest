#include <bits/stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int N;
    cin >> N;
    vector<int> arr(N), manleft(N, INT_MAX), manright(N, INT_MAX);
    for(int &t : arr){cin >> t;}

    int ans = INT_MAX;
    vector<int> copy = arr, copy1 = arr;
    manleft[0] = 0;
    manright[N-1] = 0;

    for(int i=1; i < N; i++){
        copy[i] = max(copy[i], max(copy[i-1] + 1, arr[i]+manleft[i-1]));
        manleft[i] = max(manleft[i-1], copy[i-1] - arr[i]+1);
    }
    for(int i=N-2; i>=0; i--){
        copy1[i] = max(copy1[i], max(copy1[i+1] +1, arr[i] + manright[i+1]));
        manright[i] = max(manright[i+1], copy1[i+1]-arr[i]+1);
    }
    for(int i=0; i<N; i++){
        ans = min(ans, max(manleft[i], manright[i]));
    }
    cout << ans;


    return 0;
}