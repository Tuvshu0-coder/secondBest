#include <bits/stdc++.h>
using namespace std;


int main(){
    int N, nah=0;
    cin >> N;
    vector<int> arr(N);
    for(int &t : arr){
        int a;
        cin >> a;
        if(a%2==0){
            t = 0;
        }else{
            t = 1;
            nah++;
        }
    }
    int count=0;
    if(nah % 2 == 0){
        for(int i=0; i<N; i++){
            if(arr[i]==1){
                if(arr[i+1]==0){
                    arr[i+1]=1;
                }else{
                    arr[i+1]=0;
                }
                count += 2;

            }
        }
        cout << count;
    }else{
        cout << "NO";
    }
    

    return 0;
}