#include <bits/stdc++.h>
using namespace std;
int main(){
    freopen("mowing.in", "r", stdin);
    freopen("mowing.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;

    // Directions
    map<char, pair<int, int>> dir;
    dir['N'] = make_pair(0, 1);
    dir['S'] = make_pair(0, -1);
    dir['E'] = make_pair(1, 0);
    dir['W'] = make_pair(-1, 0);

    char D;
    int S;
    vector<char> directions; // direction data
    vector<int> steps; // steps data

    for(int i=0; i<N; i++){ // input data
        cin >> D >> S;
        directions.push_back(D);
        steps.push_back(S);
    }

    // Starting point and time
    int x =0, y=0, t=0;
    map<pair<int, int>, int> visited;
    visited[make_pair(x, y)] = t; // starting from (0, 0) at time 0 so you already visited the  (0,0)

    int min_delta = INT_MAX; 
    bool has_repeat = false; // bool that if FJ repeated same cell

    // Simulate
    for(int i=0; i<N; i++){
        char d = directions[i];
        int s = steps[i];
        int dx = dir[d].first;  // input the change
        int dy = dir[d].second; // input the change

        for(int j=0; j<s; j++){
            t++;
            x += dx; // there goes the change
            y += dy; // same shit as the previous (upper) comment

            pair<int, int> pos = make_pair(x, y); // current position holder
            if(visited.count(pos)){ // if you visited the cell 
                int delta = t - visited[pos]; // Get the change in time
                if(delta < min_delta){  // check if it is the minimum
                    min_delta = delta;
                }
                has_repeat = true; // since it repeated make the bool true so it doesnt output -1
            }
            visited[pos] = t; // mark that the position was visited
        }
    }
    if(has_repeat){ // if FJ repeated
        cout << min_delta << '\n'; // if true then cout the time
    }else{
        cout << -1 << '\n'; // if didnt repeated then cout -1;
    }





    return 0;
}