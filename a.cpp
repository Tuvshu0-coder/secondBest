#include <bits/stdc++.h>
using namespace std;
int main(){
    int N;
    long long K;
    cin >> N >> K;
    vector<long long> arr(N);
    long long maxbro=0;
    for(int i=0; i<N; i++){
        cin >> arr[i];
        maxbro = max(maxbro, arr[i]);
    }
    long long left=1, right = maxbro, ans = 0;
    while (left <= right){
        long long mid = left + (right - left)/2;
        long long count = 0;
        for(int i=0; i<N; i++){
            count+= arr[i] / mid;
        }
        if(count >= K){
            ans = mid;
            left = mid+1;
        }else{
            right = mid - 1;
        }
    }
    cout << ans;
    
    return 0;
}