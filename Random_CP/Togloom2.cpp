#include <bits/stdc++.h>
using namespace std;

unordered_map<string,int> alzheimer;



int solve(string current, int K){
    if(alzheimer.count(current)) return alzheimer[current];

    // int N = current.size();
    bool possible = false;

    for(int i=0; i<= current.size() - K; i++){

        bool canFill = true;
        for(int j = 0; i< K; j++){
            if(current[i+j] == 'X'){
                canFill = false;
                break;
            }
        }
        if(!canFill) continue;

        possible = true;

        string nextCur = current;
        for(int j=0; j<K; j++){
            nextCur[i+j] = 'X';
        }

        int gansuhWin = solve(nextCur, K);
        if(gansuhWin == 2){
            alzheimer[current] = 1;
            return 1;
        }

    }

    if(!possible) alzheimer[current] = 0;
    else alzheimer[current] = 2;

    
    return alzheimer[current];


}

int main(){
    int N, K;
    cin >> N >> K;
    string autism;
    cin >> autism;
    int result = solve(autism, K);

    if(result == 1) {cout << 1 << '\n';
    }else if(result == 2) {
        cout << 2 << '\n';
    }else cout << 0 << '\n';

    return 0;
}