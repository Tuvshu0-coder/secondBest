#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int N;
    float minim = INT_MAX, maxim = INT_MIN, indexmin, indexmax;
    cin >> N;
    vector<float> array(N);
    for(int i=0; i<N; i++){
        
        cin >> array[i];
        if(array[i] < minim){
            indexmin=i;
            minim = array[i];
        }
        if(array[i] > maxim){
            indexmax = i;
            maxim = array[i];
        }
    }
    swap(array[indexmax], array[indexmin]);

    for(int i=0; i<N; i++){
        cout << fixed << setprecision(2) << array[i] << " ";
    }


}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}