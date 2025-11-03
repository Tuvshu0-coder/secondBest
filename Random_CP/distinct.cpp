#include <bits/stdc++.h>
using namespace std;
int main(){
    set<int> st;
    int N;
    cin >> N;
    for(int i=0; i<N; i++){
        int t;
        cin >> t;
        st.insert(t);
    }
    cout << st.size();

    return 0;
}