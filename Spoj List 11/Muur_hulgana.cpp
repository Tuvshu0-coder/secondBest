#include <bits/stdc++.h>
using namespace std;

int main(){
    int t;
    cin >> t;
    vector<int> data;
    while(t--){
        int cata, cab, mouz;
        cin >> cata >> cab >> mouz;
        if(abs(mouz-cata) == abs(mouz-cab)){
            data.push_back(3);
        }else if(abs(mouz-cata)>abs(mouz-cab)){
            data.push_back(2);
        }else if(abs(mouz-cata)<abs(mouz-cab)){
            data.push_back(1);
        }
    }
    for(int t : data){
        if(t == 1){
            cout << "Cat A \n";
        }else if(t==2){
            cout << "Cat B \n";
        }else{
            cout << "Mouse C \n";
        }
    }
    
    return 0;
}