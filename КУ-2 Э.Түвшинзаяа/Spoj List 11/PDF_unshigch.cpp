#include <bits/stdc++.h>
using namespace std;

int main(){

    vector<int> alp(26);
    for(int &t : alp){
        cin >> t;
    }
    string word;
    cin >> word;
    int maxh=0;
    for(int i=0; i < word.size(); i++){
        maxh = max(maxh, alp[word[i] - 97]);
    }
    cout << maxh * word.size();

    return 0;
}