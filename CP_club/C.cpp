#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
#define int long long

void solve(){
    int N;
    cin >> N;
    vector<int> arr(N);
    int maxSize=0;
    int sum=0;
    for(int i=0; i<N; i++){
        cin >> arr[i];
        if(arr[i] == 0){
            sum++;
            maxSize = max(maxSize, sum);
        }else{
            sum=0;
        }
    }
    cout << maxSize << '\n';

}

signed main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}