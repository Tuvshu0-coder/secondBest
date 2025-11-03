#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    cin >> N;
    vector<int> x(N), y(N);

    for(int &t : x){cin >> t;}
    for(int &t : y){cin >> t;}

    int maxi = 0;
    for(int i=0; i< N; i++){
        for(int j=0 ; j<N; j++){
            int dx = x[i] - x[j];
            int dy = y[i] - y[j];
            int square = dx*dx+dy*dy;
            maxi = max(maxi, square);
        }
    }
    cout << maxi;



    return 0;
}