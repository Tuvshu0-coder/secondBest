#include <bits/stdc++.h>
using namespace std;
int main(){
    int sum=0, n, k, anna;;
    cin >> n >> k;
    vector<int> bones(n);
    for(int i=0; i<n; i++){
        cin >> bones[i];
        sum += bones[i];
    }
    cin >> anna;
    if(anna - (sum - bones[k])/2 > 0 ){
        cout << anna - (sum - bones[k])/2 << endl;
    }else{
        cout << "Bon Appetit" << endl;
    }

    cin >> anna;



}