#include <bits/stdc++.h>
using namespace std;
int main(){
    int N;
    cin >> N;
    vector<int> arr(N);

    for(int &t : arr){
        cin >> t;
    }
    int maxi = 0, delta;
    for(int i=0; i<N; i++){
        for(int j=1; j<=i; j++){
            delta = arr[j]-arr[j-1];
            if(delta<=0){
                maxi = max(maxi, abs(delta)+1);
            }
        }
        for(int j=N-1; j>i; j--){
            delta = arr[j-1]- arr[j];
            if(delta <=0){
                maxi = max(maxi, abs(delta)+1);
            }
        }
    }
    cout << maxi;


    return 0;
}