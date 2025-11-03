#include <bits/stdc++.h>
using namespace std;
int main(){
    int maxi = -1e9, count=0;
    vector<int> v(100);
    for(int i = 0; i < 100; i++){
        cin >> v[i];
        if(v[i] > maxi){
            maxi = v[i];
        }
    }
    for(int i = 0; i < 100; i++){
        if(v[i] == maxi){
            count++;
        }
    }
    cout << maxi << " " << count << endl;




    return 0;
}