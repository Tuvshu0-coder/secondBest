#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin >> n;
    if(n%4==0 && n%100==0 && n%400!=0){
        cout << "Undur bish jil" << endl;
    }
    else{
        cout << "Undur jil" << endl;
    }


    return 0;
}