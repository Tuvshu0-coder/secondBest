#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


void solve(){
    int N;
    cin >> N;

    vector<int> a(N+1);

    for(int i=1; i<=N; i++){
        cin >> a[i];
    }
    vector<bool> visited(N+1, false);

    for(int i=1; i<=N; i++){
        if(!visited[i]){
            vector<int> indexes;
            int cur = i;

            while(cur <= N){
                indexes.push_back(cur);
                visited[cur] = true;
                cur*=2;
            }

            vector<int> values;
            for(int idx : indexes){
                values.push_back(a[idx]);
            }

            sort(indexes.begin(), indexes.end());
            sort(values.begin(), values.end());

            for(int j=0; j<indexes.size(); j++){
                if(values[j] != indexes[j]){
                    cout << "NO\n";
                    return;
                }
            }

        }
    }
    cout << "YES\n";


}

int main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}