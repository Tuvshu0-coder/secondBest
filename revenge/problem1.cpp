#include <bits/stdc++.h>
using namespace std;
int main(){
    int x, y;
    cin >> x >> y;
    int arr[x][y];
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            cin >> arr[i][j];
        }
    }
    for(int i=0; i<x; i++){
        int minimum= INT_MAX;
        int xi, yi;
        for(int j=0; j<y; j++){
            if(arr[i][j]<minimum){
                yi = i;
                minimum = arr[i][j];
            }
        }
        int sergei = 1;
        bool isSergei = true;
        for(int j=0; j<x; j++){
            if(arr[j][yi]> minimum){
                isSergei = false;
                break;
            }
        }

        if(isSergei){
            cout << minimum;
        }

    }

}