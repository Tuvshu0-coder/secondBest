#include <bits/stdc++.h>
using namespace std;
int main(){
    
    int N;
    cin >> N;
    vector<pair<int, int>> coordinate(N);
    for(int i=0; i<N; i++){
        cin >> coordinate[i].first >> coordinate[i].second;
    }
    int sergei=0;
    for(int i=0; i<N; i++){
        int dy=0, dx=0, maxx=0, maxy=0;
        for(int j=0; j<N; j++){
            if(coordinate[i].first == coordinate[j].first && i!=j){
                if(maxy < abs(coordinate[i].second-coordinate[j].second)){
                    maxy = abs(coordinate[i].second-coordinate[j].second);
                }
            }
            if(coordinate[i].second == coordinate[j].second && i!=j){
                if(maxx < abs(coordinate[i].first-coordinate[j].first)){
                    maxx = abs(coordinate[i].first-coordinate[j].first);
                }
            }
        }
        sergei = max(sergei, maxy*maxx);
        

    }
    cout << sergei;



    return 0;
}