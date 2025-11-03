#include <bits/stdc++.h>
using namespace std;
int main(){
    long long t; cin >> t;
    while(t--){
        long long n, neg = 0, zero = 0;
        cin >> n;
        vector<long long> vec(n);
        for(int i=0; i<n; i++){
            cin >> vec[i];
            if(vec[i] == -1) neg++;
            else if(vec[i] == 0) zero++;
        }   

       
        int sum = zero + 2 * neg;

       
        if(neg % 2 == 1 && zero == 0) sum += 2;

        cout << sum << '\n';
    }
    return 0;
}
