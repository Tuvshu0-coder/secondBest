#include<bits/stdc++.h>
using namespace std;

int main(){
    vector<int> lalar = {1, 1, 2, 2, 3, 3, 3, 4, 5};
    cout << lower_bound(lalar.begin(), lalar.end(), 3) - lalar.begin()<< '\n' << upper_bound(lalar.begin(), lalar.end(), 3) - lalar.begin(); 



    return 0;
}