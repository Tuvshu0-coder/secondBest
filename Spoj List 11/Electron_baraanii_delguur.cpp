#include <bits/stdc++.h>
using namespace std;



int main(){
    int bank, mawuz, flash;
    cin >> bank >> mawuz >> flash;
    vector<int> a(mawuz), b(flash);
    for(int i=0; i<mawuz; i++){cin >> a[i];}
    for(int i=0; i<flash; i++){cin >> b[i];}
    int max = -1;
    for(int i =0; i<mawuz; i++){
        for(int j=0; j<flash; j++){
            if(a[i]+b[j] <= bank && a[i]+b[j] > max){
                max = a[i]+b[j];
            }
        }
    }
    cout << max << endl;
}
