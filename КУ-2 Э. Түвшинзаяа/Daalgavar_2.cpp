#include <bits/stdc++.h>
using namespace std;

void print_digit(int x){
    if(x < 10){
        cout << x;
    }else{
        char lapasion = (char)(x - 10 + 'A');
        cout << lapasion;
    }
}

void print_digits(int x, int n){

    if(x == 0) return;
    print_digits(x/n, n);

    print_digit(x % n);
}

int main(){
    int x, n;
    cin >> x >> n;
    print_digits(x, n);

    return 0;
}