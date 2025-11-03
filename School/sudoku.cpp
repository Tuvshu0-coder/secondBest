// sudoku.cpp
// Simple console Sudoku: generator, solver, play, hint, auto-solve.
// Compile: g++ -std=c++17 sudoku.cpp -O2 -o sudoku

#include <bits/stdc++.h>
using namespace std;

struct Sudoku {
    array<array<int,9>,9> grid{};
    array<array<bool,9>,9> fixed{}; // cells of the puzzle (not editable)

    Sudoku() {
        for(int r=0;r<9;r++) for(int c=0;c<9;c++) grid[r][c]=0;
        for(int r=0;r<9;r++) for(int c=0;c<9;c++) fixed[r][c]=false;
    }

    void display() const {
        cout << "    1 2 3   4 5 6   7 8 9\n";
        cout << "  +-------+-------+-------+\n";
        for(int r=0;r<9;r++){
            cout << char('A'+r) << " | ";
            for(int c=0;c<9;c++){
                if(grid[r][c]==0) cout << ".";
                else cout << grid[r][c];
                cout << ( (c%3==2) ? " | " : " " );
            }
            cout << "\n";
            if(r%3==2) cout << "  +-------+-------+-------+\n";
        }
    }

    bool valid_move(int r, int c, int val) const {
        if(val<1 || val>9) return false;
        if(grid[r][c]!=0 && grid[r][c]!=val) {
            // if cell already has a different value, not allowed for tentative checks
            return false;
        }
        // row
        for(int j=0;j<9;j++) if(j!=c && grid[r][j]==val) return false;
        // col
        for(int i=0;i<9;i++) if(i!=r && grid[i][c]==val) return false;
        // 3x3
        int br=(r/3)*3, bc=(c/3)*3;
        for(int i=br;i<br+3;i++) for(int j=bc;j<bc+3;j++) if((i!=r||j!=c) && grid[i][j]==val) return false;
        return true;
    }

    // solver using backtracking; if stopAtOne==true will stop after finding first solution
    bool solve_backtrack(bool stopAtOne=true) {
        // find empty cell
        int sr=-1, sc=-1;
        for(int i=0;i<9;i++){ for(int j=0;j<9;j++){ if(grid[i][j]==0){ sr=i; sc=j; goto found; } } }
    found:
        if(sr==-1) return true; // solved

        // try numbers 1..9 (we can try a random order for variety)
        vector<int> vals{1,2,3,4,5,6,7,8,9};
        // no shuffle here for deterministic solver; generator will use shuffle
        for(int v : vals){
            if(valid_move(sr,sc,v)){
                grid[sr][sc]=v;
                if(solve_backtrack(stopAtOne)){
                    return true;
                }
                grid[sr][sc]=0;
            }
        }
        return false;
    }

    // solver that counts number of solutions (limit to maxCount)
    int count_solutions(int maxCount=2) {
        // find empty
        int sr=-1, sc=-1;
        for(int i=0;i<9;i++){ for(int j=0;j<9;j++){ if(grid[i][j]==0){ sr=i; sc=j; goto found2; } } }
    found2:
        if(sr==-1) return 1; // one solution found

        int count=0;
        for(int v=1; v<=9 && count<maxCount; v++){
            if(valid_move(sr,sc,v)){
                grid[sr][sc]=v;
                count += count_solutions(maxCount - count);
                grid[sr][sc]=0;
            }
        }
        return count;
    }

    // generate a full board randomly
    bool generate_full(mt19937 &rng) {
        // fill using randomized backtracking
        vector<pair<int,int>> empties;
        for(int i=0;i<9;i++) for(int j=0;j<9;j++) grid[i][j]=0;
        for(int i=0;i<9;i++) for(int j=0;j<9;j++) empties.emplace_back(i,j);

        function<bool(int)> backtrack = [&](int idx)->bool {
            if(idx == (int)empties.size()) return true;
            auto [r,c] = empties[idx];
            vector<int> nums(9);
            iota(nums.begin(), nums.end(), 1);
            shuffle(nums.begin(), nums.end(), rng);
            for(int v: nums){
                if(valid_move(r,c,v)){
                    grid[r][c]=v;
                    if(backtrack(idx+1)) return true;
                    grid[r][c]=0;
                }
            }
            return false;
        };

        return backtrack(0);
    }

    // create puzzle by removing cells while keeping unique solution
    void make_puzzle(int clues, mt19937 &rng) {
        // clues = number of filled cells to remain (between ~17 and 81)
        // start from full solution
        array<array<int,9>,9> full = grid;
        vector<pair<int,int>> cells;
        for(int r=0;r<9;r++) for(int c=0;c<9;c++) cells.emplace_back(r,c);
        shuffle(cells.begin(), cells.end(), rng);

        int toRemove = 81 - clues;
        for(auto [r,c] : cells){
            if(toRemove<=0) break;
            int old = grid[r][c];
            grid[r][c]=0;
            // check uniqueness: we limit check to seeing if more than 1 solution exists
            Sudoku tmp = *this;
            int sols = tmp.count_solutions(2);
            if(sols != 1){
                // can't remove, restore
                grid[r][c]=old;
            } else {
                toRemove--;
            }
        }

        // set fixed flags
        for(int r=0;r<9;r++) for(int c=0;c<9;c++) fixed[r][c] = (grid[r][c]!=0);
    }

    bool is_complete_and_valid() const {
        for(int r=0;r<9;r++) for(int c=0;c<9;c++) if(grid[r][c]==0) return false;
        // check rows, cols, boxes
        for(int r=0;r<9;r++){
            array<bool,10> found{}; found.fill(false);
            for(int c=0;c<9;c++){
                int v=grid[r][c];
                if(v<1||v>9||found[v]) return false;
                found[v]=true;
            }
        }
        for(int c=0;c<9;c++){
            array<bool,10> found{}; found.fill(false);
            for(int r=0;r<9;r++){
                int v=grid[r][c];
                if(v<1||v>9||found[v]) return false;
                found[v]=true;
            }
        }
        for(int br=0;br<9;br+=3) for(int bc=0; bc<9; bc+=3){
            array<bool,10> found{}; found.fill(false);
            for(int r=br;r<br+3;r++) for(int c=bc;c<bc+3;c++){
                int v=grid[r][c];
                if(v<1||v>9||found[v]) return false;
                found[v]=true;
            }
        }
        return true;
    }

    // provide a simple hint: find an empty cell and fill it with correct value from solution
    bool give_hint() {
        Sudoku copy = *this;
        if(!copy.solve_backtrack(true)) return false;
        for(int r=0;r<9;r++) for(int c=0;c<9;c++){
            if(grid[r][c]==0){
                grid[r][c] = copy.grid[r][c];
                return true;
            }
        }
        return false;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Sudoku (console). Generating puzzle...\n";
    random_device rd;
    mt19937 rng(rd());

    Sudoku s;
    if(!s.generate_full(rng)){
        cerr << "Failed to generate full board.\n";
        return 1;
    }

    // choose difficulty: easy/medium/hard -> number of clues
    cout << "Choose difficulty (1=Easy,2=Medium,3=Hard): ";
    int diff = 2;
    if(!(cin>>diff)) diff=2;
    int clues = 36;
    if(diff==1) clues=40;
    else if(diff==3) clues=28;
    else clues=34;

    s.make_puzzle(clues, rng);

    // Save solution for hints/solve: we create solved copy
    Sudoku solution = s;
    solution.solve_backtrack(true);

    string cmd;
    while(true){
        s.display();
        if(s.is_complete_and_valid()){
            cout << "Congratulations! You completed the Sudoku.\n";
            break;
        }
        cout << "Commands: set <A-I><1-9> <1-9>   e.g. set A1 5\n";
        cout << "          clear <A-I><1-9>\n";
        cout << "          hint    (fills one cell)\n";
        cout << "          solve   (auto-solve puzzle)\n";
        cout << "          check   (validate current board)\n";
        cout << "          quit\n";
        cout << "> ";
        if(!(cin>>cmd)) break;
        if(cmd=="quit") break;
        else if(cmd=="set"){
            string pos; int val;
            cin >> pos >> val;
            if(pos.size()!=2 || val<1 || val>9){
                cout << "Invalid input.\n"; continue;
            }
            int r = toupper(pos[0]) - 'A';
            int c = pos[1]-'1';
            if(r<0||r>8||c<0||c>8){ cout << "Invalid cell.\n"; continue; }
            if(s.fixed[r][c]){ cout << "Cell is fixed (part of puzzle). Can't change.\n"; continue; }
            if(!s.valid_move(r,c,val)) { cout << "Move invalid (conflict).\n"; continue; }
            s.grid[r][c]=val;
        }
        else if(cmd=="clear"){
            string pos; cin>>pos;
            if(pos.size()!=2){ cout << "Invalid input.\n"; continue; }
            int r=toupper(pos[0])-'A', c=pos[1]-'1';
            if(r<0||r>8||c<0||c>8){ cout << "Invalid cell.\n"; continue; }
            if(s.fixed[r][c]) { cout << "Cell is fixed.\n"; continue; }
            s.grid[r][c]=0;
        }
        else if(cmd=="hint"){
            if(s.give_hint()) cout << "Hint applied (one cell filled).\n";
            else cout << "No hint available.\n";
        }
        else if(cmd=="solve"){
            cout << "Auto-solving...\n";
            Sudoku backup = s;
            if(s.solve_backtrack(true)){
                s.display();
                cout << "Solved. Type 'quit' or continue to edit.\n";
            } else {
                cout << "No solution found.\n";
                s = backup;
            }
        }
        else if(cmd=="check"){
            // quick validity check (no empties)
            bool ok=true;
            for(int r=0;r<9;r++) for(int c=0;c<9;c++){
                int v=s.grid[r][c];
                if(v!=0 && !s.valid_move(r,c,v)) ok=false;
            }
            if(ok) cout << "No obvious conflicts found.\n";
            else cout << "There are conflicts in the board.\n";
        }
        else {
            cout << "Unknown command.\n";
            // flush rest of line
            string rest;
            getline(cin, rest);
        }
    }

    cout << "Thanks for playing. Goodbye!\n";
    return 0;
}
