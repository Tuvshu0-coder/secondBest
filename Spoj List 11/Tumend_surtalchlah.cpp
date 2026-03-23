#include <bits/stdc++.h>
using namespace std;

int solve(int x){
    int cur=5, sum=0;
    for(int i=0; i<x; i++){
        sum += cur/2;
        cur = cur/2*3;
    }
    return sum;
}

int main(){
    int T;
    cin >> T;
    cout << solve(T);


    return 0;
}