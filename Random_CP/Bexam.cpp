#include <bits/stdc++.h>
using namespace std;
int main(){
    int t;
    vector<int> arr;
    while(cin >> t){
        arr.push_back(t);
    }
    sort(arr.begin(), arr.end());
    int count=1;
    for(int i=0; i<arr.size(); i++){
        if(arr[i] == arr[i+1]){
            count ++;
        }else{
            if(count != 3){
                cout << arr[i];
            }else{
                count = 1;
            }
        }
        
    }
    


    return 0;
}