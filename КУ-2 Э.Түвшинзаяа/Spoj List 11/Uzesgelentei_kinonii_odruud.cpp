#include <bits/stdc++.h>
using namespace std;

int reverseNum(int n) {
    int rev = 0;
    while (n) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return rev;
}

int main() {
    int i, j, k;
    cin >> i >> j >> k;

    int count = 0;
    for (int a = i; a <= j; a++) {
        if (abs(a - reverseNum(a)) % k == 0)
            count++;
    }

    cout << count;
    return 0;
}
