#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
	cin.tie(nullptr);
    int N , target;
    cin >> N >> target;
    vector<int> a(N);
    for(int &t : a) cin >> t;
    int left=0;
    long long sum=0, count=0;
 
    for(int right =0; right<N; right++){
        sum += a[right];
        while(sum > target && left <= right){
            sum -=a[left];
            left++;
        }
        if(sum == target) count++;
    }
    cout << count;
    
 
    return 0;
}
