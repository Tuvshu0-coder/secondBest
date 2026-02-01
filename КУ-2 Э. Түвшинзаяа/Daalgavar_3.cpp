#include <bits/stdc++.h>
using namespace std;

// int zeroSlayer(int N){
//     while(N%10 == 0){
//         N/=10;
//     }
// }

void numFlipper(int N, int count){
    if(count <= 0) return;
    
    
    cout << N%10;
    numFlipper(N/10, count-1);
}

int main(){
    int N;
    cin >> N;
    int count = log10(N) + 1;
    while(N%10 == 0){
        N/=10;
        count--;
    }
    numFlipper(N, count);
    return 0;
}
