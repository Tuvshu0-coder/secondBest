#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
using ll = long long;
int n;
vector<ll> weights(n);

ll apple(int index, ll sum1, ll sum2){
    if(index == n){
        return abs(sum1 - sum2);
    }
    return min(apple(index+1, sum1+ weights[index], sum2), apple(index+1, sum1, sum2+weights[index]));

}
int main(){
    cin >> n;
    weights.resize(n);
    for(int i=0; i<n; i++){cin >> weights[i];}
    cout << apple(0, 0, 0);


    return 0;
}
