#include <bits/stdc++.h>
using namespace std;
int main(){
    int y, x;
    cin >> y >> x;
    vector<pair<int, int>> v, bro;
    vector<int> ans;
    v = {{1, 1}, {y, 1}, {1, x}, {y, x}};
    int n, count=0;
    for(int i = 1; i <= x; i++) {
        for(int j = 1; j<= y; j++){
            cin >> n;
            if(n == 1){
                count++;
                bro.push_back({j, i});
            }
    
        }
    }
    for(int i = 0; i < bro.size(); i++){

    }
    


    return 0;
}