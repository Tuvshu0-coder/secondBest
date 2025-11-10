#include <iostream>
#include <string>
using namespace std;

int main() {
    int N;
    cin >> N;

    string front[100];
    string back[100];

    for (int i = 0; i < N; i++) {
        cin >> front[i] >> back[i];
    }

    int answer[26];
    for (int i = 0; i < 26; i++) {
        answer[i] = 0;
    }

    for (int i = 0; i < N; i++) {
        int countFront[26];
        int countBack[26];

        for (int j = 0; j < 26; j++) {
            countFront[j] = 0;
            countBack[j] = 0;
        }

        for (int j = 0; j < front[i].size(); j++) {
            char c = front[i][j];
            countFront[c - 'a']++;
        }

        for (int j = 0; j < back[i].size(); j++) {
            char c = back[i][j];
            countBack[c - 'a']++;
        }

        for (int j = 0; j < 26; j++) {
            int maxCount;
            if (countFront[j] > countBack[j]) {
                maxCount = countFront[j];
            } else {
                maxCount = countBack[j];
            }
            answer[j] = answer[j] + maxCount;
        }
    }

    for (int i = 0; i < 26; i++) {
        cout << answer[i] << "\n";
    }

    return 0;
}
