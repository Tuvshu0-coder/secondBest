#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int A, B;

}

#include <bits/stdc++.h>
using namespace std;

signed main(){
    int n = 1050;
    

    vector<bool> isPrime(n+1, true);

    isPrime[0] = false;
    isPrime[1] = false;

    for(int i=2; i*i <= n; i++){
        if(isPrime[i]){
            for(int j=i*i; j<= n; j += i){
                isPrime[j] = false;
            }
        }
    }
    int count =0;
    unordered_map<int , int> um;
    vector<int> primeNums;
    for(int i=2; i<=n; i++){
        if(isPrime[i]){
            count++;
            //cout << i << " ";
            primeNums.push_back(i);
            um[i] = 1;
        }
    }
    int A, B;
    cin >> A >> B;
    for(int i=0; i<primeNums.size(); i++){
        if(um[A + primeNums[i]] == 1){
            if(um[A - B - primeNums[i]] == 1){
                cout  <<3 << '\n' << A << ' ' << A - primeNums[i] << ' ' << B;
            }else if(um[A - B + primeNums[i]] == 1){ 
                cout << 3 << '\n' << A << ' ' << A + primeNums[i] << ' ' << B;                
            }
            
        }else continue;
        
    }
    
    
}


