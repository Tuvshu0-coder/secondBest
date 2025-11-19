#include <bits/stdc++.h>
using namespace std;

int main(){
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        int count, b;
        count = log10(n)+1;
        int temp=1;
        for(int i=0; i<count-1; i++){
            temp*=10;
        }
        b = n%temp;
        cout << n-b-1;
        // b = n/(pow(10, count));

    }




    return 0;
}