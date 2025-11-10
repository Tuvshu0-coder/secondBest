#include <bits/stdc++.h>
using namespace std;



int main(){
    int N;
    cin >> N;
    vector<int> arr(N);
    for(int &t : arr) cin >> t;
    sort(arr.begin(), arr.end());
    int streak=1, maxim=0;
    for(int i=0; i<N; i++){
        if(abs(arr[i]-arr[i+1]) <= 1){
            streak++;
        }else{
            streak=1;
        }
        maxim = max(maxim, streak);

    }
    cout << maxim;





    return 0;
}