#include <bits/stdc++.h>
using namespace std;

string encrypt(string text, int k){
    k %= 26;
    for(char &c : text){
        if(isupper(c)){
            c = 'A' + (c - 'A' + k) % 26;
        }else if(islower(c)){
            c = 'a' + (c - 'a' + k) % 26;
        }

    }
    return text;
}

int main(){
    string text;
    cin >> text;
    int k;
    cin >> k;

    cout << encrypt(text, k);
}