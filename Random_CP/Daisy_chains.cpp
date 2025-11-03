#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    cin >> N;
    vector<int> petals(N);
    int sum = 0;
    for(int &t : petals){
        cin >> t;
        sum += t;
    }
    int avg = sum/N;
    int count = 0;
    int m = 0;
    for(int i=0; i<N; i++){
        for(int j=i; j<N; j++){
            if(petals[i]==avg){count++;}
            if(petals[j]==avg){count++;}
        }
        m++;
    }
    cout << count;




    return 0;
}