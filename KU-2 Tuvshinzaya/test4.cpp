#include <bits/stdc++.h>
using namespace std;
int main(){
    int a[10][10];
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            cin >> a[i][j];
        }
    }
    int c[45];
    int m = 0;
    for(int i = 0; i<10; i++){
        for(int j = 0; j<10; j++){
            if( i < j){
                c[m] = a[i][j];
                m++;
            }
        }
    }
    for(int i = 0; i < m; i++){
        cout << c[i] << "\n";
    }


    return 0;
}