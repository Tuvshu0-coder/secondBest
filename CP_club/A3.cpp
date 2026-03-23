//#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#define int long long

void solve(){
    int N, a, b, c;
    cin >> N;
    vector<int> arr(10);
    vector<int> res(3);
    for(int i=0; i<10; i++){
        arr[i]=i+1;
    }
    bool isYes = false;
    for(int i=0; i<10; i++){
        int grab=arr[i];
        int left=0;
        int right = 9;
        while(left <= right){
            if( grab * arr[left] * arr[right] == N && grab!=arr[left] && grab!=arr[right] && arr[left]!=arr[right]){
                // cout << "YES \n";
                // cout << grab << " "<<arr[left] << " "<<arr[right] << '\n';
                a = grab;
                b = arr[left];
                c = arr[right];
                res.push_back(a);
                res.push_back(b);
                res.push_back(c);

                isYes = true; 
                break;
        
            }
            if( grab * arr[left] * arr[right] > N){
                right--;
            }else{
                left++;
            }
        }
        
        // cout << "NO \n";
        
    }

    if(!isYes){
        cout << "NO \n";
    }else{
        cout << "YES" << '\n';
        sort(res.begin(), res.end());
        for(int i=0; i<3; i++){
            cout << res[i] << ' ';
        }
        cout << '\n';
        // cout << a << ' ' << b << ' ' << c << '\n';
    }

}

signed main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}