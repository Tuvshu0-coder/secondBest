#include <bits/stdc++.h>
using namespace std;
#define int long long

struct mystruct{
    int count;
    int cowo;
};

signed main(){
    freopen("pairup.in", "r", stdin);
    freopen("pairup.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;
    mystruct arr[N];
    for(int i=0; i<N; i++){
        cin >> arr[i].count >> arr[i].cowo;
    }
    sort(arr, arr + N, [](const mystruct &a, const mystruct &b){
        return a.cowo < b.cowo;
    });


    int l=0, r=N-1, maxim=INT_MIN;
    while(l<=r){
        int sum;
        sum = arr[l].cowo + arr[r].cowo;
        maxim = max(maxim, sum);
        arr[l].count--;
        arr[r].count--;
        if(arr[l].count==0) l++;
        if(arr[r].count==0) r--;
    }
    cout << maxim;



    
    

    return 0;
}