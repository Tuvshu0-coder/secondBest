#include <iostream>
using namespace std;

int main() {
    int N;
    cin >> N;

    int start[100], endt[100], buckets[100];

    for (int i = 0; i < N; i++) {
        cin >> start[i] >> endt[i] >> buckets[i];
    }

    int maxBuckets = 0;

    for (int time = 1; time <= 1000; time++) {
        int currentBuckets = 0;

        for (int i = 0; i < N; i++) {
            if (time >= start[i] && time < endt[i]) {
                currentBuckets = currentBuckets + buckets[i];
            }
        }

        if (currentBuckets > maxBuckets) {
            maxBuckets = currentBuckets;
        }
    }

    cout << maxBuckets << "\n";

    return 0;
}
