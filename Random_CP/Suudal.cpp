#include <iostream>
#include <vector>
#include <string>
using namespace std;
using ll = long long;

int main(){

    int N, niggers;
    cin >> N >> niggers;

    vector<ll> intervals, loc(niggers);
    intervals.push_back(N);

    for(int i=0; i<niggers*2; i++){
        string command;
        cin >> command;
        if(command == "+"){
            if(intervals.size() == 1) {
                cout << 1 << '\n';

            }else{
                ll maxIndex = 0;


                for(int j=0; j<intervals.size(); j++){
                    if(intervals[maxIndex] <= intervals[j]) maxIndex = j;
                    // minIndex = min(intervals[minIndex], intervals[j]); 
                }

                if (maxIndex == 0)
                {
                    cout << 1 << '\n';
                }
                else if (maxIndex == intervals.size() - 1)
                {
                    cout << N;
                }
                else
                {
                    cout << intervals[maxIndex] / 2;
                }

                cout << maxIndex << '\n';
                ll intSize = intervals[maxIndex];
                // vec.erase(vec.begin() + index_to_remove);
                intervals.erase(intervals.begin() + maxIndex);
                intervals.insert(intervals.begin() + maxIndex, intSize/2);
                intervals.insert(intervals.begin() + maxIndex + 1, intSize/2);
            }
        }else{
            ll leaveIndex = (stoll(command) * -1) - 1;
            ll sum = intervals[leaveIndex] + intervals[leaveIndex + 1]; 
            intervals.erase(intervals.begin() + leaveIndex);
            intervals.erase(intervals.begin() + leaveIndex);
            intervals.insert(intervals.begin() + leaveIndex, sum);


        }
    }
    // cout << intervals.size() << "\n";

    




}