#include <bits/stdc++.h>
using namespace std;

// Function to convert the row into a string of 'O' and 'X' to represent state
// Memoization map: state string -> winner (1=Player1 wins, 2=Player2 wins)
unordered_map<string,int> memo;

// Function to check if the current player can win from a given state
int canWin(string state, int K) {
    if (memo.count(state)) return memo[state]; // return cached result

    int N = state.size();
    bool movePossible = false;

    // Try all segments in the state
    for (int i = 0; i <= N - K; ++i) {
        // Check if K consecutive O's are available
        bool canFill = true;
        for (int j = 0; j < K; ++j) {
            if (state[i + j] != 'O') {
                canFill = false;
                break;
            }
        }
        if (!canFill) continue;

        movePossible = true;

        // Make the move: fill K O's with X
        string nextState = state;
        for (int j = 0; j < K; ++j) nextState[i + j] = 'X';

        // Recursive call: if opponent cannot win, current player wins
        int opponentWin = canWin(nextState, K);
        if (opponentWin == 2) { // opponent loses
            memo[state] = 1;     // current player wins
            return 1;
        }
    }

    // If no move possible or all moves let opponent win
    if (!movePossible) memo[state] = 0;       // no moves left
    else memo[state] = 2;                     // opponent can always win

    return memo[state];
}

int main() {
    int N, K;
    cin >> N >> K;
    string row;
    cin >> row;

    int result = canWin(row, K);

    // Output according to problem statement
    if (result == 1) cout << 1 << endl;      // Player 1 wins
    else if (result == 2) cout << 2 << endl; // Player 2 wins
    else cout << 0 << endl;                  // no moves possible

    return 0;
}
