#include <bits/stdc++.h>
using namespace std;

struct LaPasion{
    int height;
    int weight;
};

bool dexter(LaPasion a, LaPasion b){
    if(a.height == b.height){
        return a.weight < b.weight;
    }
    return a.height < b.height;
}

int main(){
    int N;
    cin >> N;
    vector<LaPasion> arr(N);
    for(int i=0; i<N; i++){
        cin >> arr[i].height >> arr[i].weight;
    }
    sort(arr.begin(), arr.end(), dexter);
    cout << "====[SORTED RESULT]====" << '\n';
    for(int i=0; i<N; i++){
        cout << arr[i].height << " " << arr[i].weight << '\n';
    }
    

}