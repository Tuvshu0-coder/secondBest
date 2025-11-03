#include <bits/stdc++.h>
using namespace std;

bool bnasd(char letter, const string &word) {
    int count = 0;
    for (char t : word) {
        if (t == letter && t != '_')
            count++;
    }
    return count > 1;
}

bool bolnsd(const string &word) {
    int n = word.size();
    for (int j = 0; j < n; j++) {
        bool leftSame  = (j > 0 && word[j] == word[j - 1]);
        bool rightSame = (j < n - 1 && word[j] == word[j + 1]);
        if (!leftSame && !rightSame)
            return false;
    }
    return true;
}



int main(){
    int T;
    cin >> T;
    while(T--){
        int size;
        string ship;
        bool hasL = false;
        cin >> size >> ship;
        for(char t : ship){
            if(t == '_'){
                hasL = true;
            }
        }
        if(hasL){
            bool isWork = true;
            for(int i=0; i<ship.size(); i++){
                if(bnasd(ship[i], ship) == false){
                    isWork = false;
                    break;
                }
            }


            if(isWork){
                cout << "YES";
            }else{
                cout << "NO";
            }
            cout << '\n';
        }else{
            if(bolnsd(ship)){
                cout << "YES";
            }else{
                cout << "NO";
            }
            cout << '\n';

        }   


    }
    return 0;
}