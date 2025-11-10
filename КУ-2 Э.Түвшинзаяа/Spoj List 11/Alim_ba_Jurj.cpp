#include <bits/stdc++.h>
using namespace std;

int main(){
    int sam1, sam2, apple, orange, amount, omount;
    cin >> sam1 >> sam2 >> apple >> orange;
    cin >> amount >> omount;
    vector<int> avec(amount), ovec(omount);
    for(int &t : avec) cin >> t;
    for(int &t : ovec) cin >> t;
    int acount=0, ocount=0;
    for(int t : avec){
        if(t+apple >= sam1 && t+apple <= sam2){
            acount++;
        }
    }
    for(int t : ovec){
        if(t+orange <=sam2 && t+orange >= sam1){
            ocount++;
        }
    }
    cout << acount << '\n' << ocount;

    return 0;
}