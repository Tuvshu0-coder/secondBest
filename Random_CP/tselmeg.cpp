#include <bits/stdc++.h>
using namespace std;
int main(){
    int N;
    cin >> N;

    vector<int> a(N);
    for(int &t : a){
        cin >> t;
    }
    sort(a.begin(), a.end());
    int T;
    cin >> T;
    vector<int> data(T);
    for(int i=0; i< T; i++){
        int x, y, index1, index2;
        cin >> x >> y;
        index1 = lower_bound(a.begin(), a.end(), x) - a.begin();
        index2 = upper_bound(a.begin(), a.end(), y) - a.begin() - 1;
        data[i] = index2 - index1 + 1;
    }
    for(int t: data){
        cout << t << " ";
    }
    return 0;
}