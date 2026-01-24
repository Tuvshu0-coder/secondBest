#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    cin >> N;
    int res;
    if (N == 0){
        cout << 1;
    }else{
        res = pow(1 + 4*N , 2) - pow(4*N - 1, 2);
        cout << res;
    }

    return 0;
}