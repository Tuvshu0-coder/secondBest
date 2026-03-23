#include <bits/stdc++.h>
using namespace std;

int main(){
    int d, m, y;
    int d1, m1, y1;
    cin >> d >> m >> y >> d1 >> m1 >> y1;

    int fine = 0;

    if (y > y1) fine = 10000;
    else if (y == y1 && m > m1) fine = (m - m1) * 500;
    else if (y == y1 && m == m1 && d > d1) fine = (d - d1) * 15;

    cout << fine;
    return 0;
}
