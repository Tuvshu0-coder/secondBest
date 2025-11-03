#include <iostream>
#include <cmath>
using namespace std;

void toh(int n, char from, char to, char aux){
    if(n == 1){
        cout << from << " " << to << endl;
    }else{
        toh(n-1, from, aux, to);
        cout << from << " " << to << endl;
        toh(n-1, aux, to, from);
    }   
}
int main(){
    int N;
    cin >> N;
    cout << pow(2, N)-1 << endl;
    toh(N, '1', '3', '2');
    return 0;
}