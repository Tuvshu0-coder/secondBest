#include <bits/stdc++.h>
using namespace std;
int main(){
    int N;
    cin >> N;
    int arr[N][N];
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            cin >> arr[i][j];
        }
    }
    int maxx=0, maxy=0;
    for(int i=0; i<N; i++){
        maxx = max(maxx, arr[0][i]);
        maxy = max(maxy, arr[i][N-1]);
    }
    cout << max(maxx, maxy);



    return 0;
}