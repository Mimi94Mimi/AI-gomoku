#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <queue>
#include <climits>
#include <algorithm>
#include <utility>
#include <vector>

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

enum PATTERN {
    NONE,
    OPEN_ONE,
    HALF_ONE,
    OPEN_TWO,
    HALF_TWO,
    OPEN_THREE,
    HALF_THREE,
    OPEN_FOUR,
    HALF_FOUR,
    GAP_THREE,
    FIVE,
};

enum DIRECTIONS {
    HORIZONTAL,
    VERTICAL,
    DIAG_TOP_LEFT,
    DIAG_TOP_RIGHT
};
 
int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;
//std::queue<state> Q;

struct point{
    int i;
    int j;
};

bool is_on_board_ev(int pos_i, int pos_j){
    return 0 <= pos_i && pos_i <= SIZE + 1 && 0 <= pos_j && pos_j <= SIZE + 1;
}

bool is_on_board(int pos_i, int pos_j){
    return 0 <= pos_i && pos_i < SIZE && 0 <= pos_j && pos_j < SIZE;
}

class state{
    public:
    state(std::array<std::array<int, SIZE>, SIZE> Board){
        value = 0;
        cntdisc = 0;
        cntpattern[BLACK].fill(0);
        cntpattern[WHITE].fill(0);
        winner = EMPTY;

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                recommended[i][j] = false;
            }
        }

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                stateboard[i][j] = Board[i][j];

                if(Board[i][j]){
                    cntdisc++;
                    for(int v = -1; v < 2; v++){
                        for(int h = -1; h < 2; h++){
                            if(is_on_board(i + v, j + h))
                            recommended[i + v][j + h] = true;
                        }
                    }
                }
            }
        }
    }

    state(const state& S){
        value = S.value;
        cntdisc = S.cntdisc;
        stateboard = S.stateboard;
        cntpattern = S.cntpattern;
        winner = S.winner; 
    }

    void set_recommended(){
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j < SIZE; j++){
                if(stateboard[i][j]){
                    for(int v = -1; v < 2; v++){
                        for(int h = -1; h < 2; h++){
                            if(is_on_board(i + v, j + h))
                            recommended[i + v][j + h] = true;
                        }
                    }
                }
            }
        }
    }

    void update_cntpattern(SPOT_STATE turn){
        cntpattern[turn].fill(0);

        std::array<std::array<int, SIZE + 2>, SIZE + 2> board_to_ev;
        int vec_i[] = {0, 1, -1, 1};
        int vec_j[] = {1, 0, 1, 1};
        int self = turn;
        int opp = 3 - turn;

        for(int i = 0; i < SIZE + 2; i++){
            for(int j = 0; j < SIZE + 2; j++){
                if(i == 0 || i == SIZE + 1 || j == 0 || j == SIZE + 1)
                board_to_ev[i][j] = opp;
                else board_to_ev[i][j] = stateboard[i - 1][j - 1];
            }
        }

        for(int dir = 0; dir < 4; dir++){
            for(int i = 0; i < SIZE + 2; i++){
                for(int j = 0; j < SIZE + 2; j++){
                    if(!is_on_board_ev(i, j)) continue;
                    if(!is_on_board_ev(i + vec_i[dir], j + vec_j[dir])) continue;
                    if(board_to_ev[i + vec_i[dir]][j + vec_j[dir]] != self) continue;

                    if(is_on_board_ev(i + 4 * vec_i[dir], j + 4 * vec_j[dir])){
                        std::vector<int> seq = {self, self, self, self, self};
                        std::vector<int> seq_b;
                        for(int l = 0; l <= 4; l++){
                            seq_b.push_back(board_to_ev[i + l * vec_i[dir]][j + l * vec_j[dir]]);
                        }
                        if(seq_b == seq){
                            cntpattern[self][FIVE]++;
                            winner = self;
                            return;
                        }
                    }

                    if(board_to_ev[i][j] == self) continue;

                    if(is_on_board_ev(i, j) && is_on_board_ev(i + 2 * vec_i[dir], j + 2 * vec_j[dir])){
                        std::vector<int> seq[4] = {
                            {EMPTY, self, EMPTY}, 
                            {EMPTY, self, opp}, 
                            {opp, self, EMPTY}, 
                            {opp, self, opp}
                        };
                        std::vector<int> seq_b;
                        for(int l = 0; l <= 2; l++){
                            seq_b.push_back(board_to_ev[i + l * vec_i[dir]][j + l * vec_j[dir]]);
                        }
                        if(seq_b == seq[0]){cntpattern[self][OPEN_ONE]++;}
                        else if(seq_b == seq[1]){cntpattern[self][HALF_ONE]++;}
                        else if(seq_b == seq[2]){cntpattern[self][HALF_ONE]++;}
                        else if(seq_b == seq[3]){}
                    }

                    if(is_on_board_ev(i + 3 * vec_i[dir], j + 3 * vec_j[dir])){
                        std::vector<int> seq[4] = {
                            {EMPTY, self, self, EMPTY}, 
                            {EMPTY, self, self, opp}, 
                            {opp, self, self, EMPTY}, 
                            {opp, self, self, opp}
                        };
                        std::vector<int> seq_b;
                        for(int l = 0; l <= 3; l++){
                            seq_b.push_back(board_to_ev[i + l * vec_i[dir]][j + l * vec_j[dir]]);
                        }
                        if(seq_b == seq[0]){cntpattern[self][OPEN_TWO]++;}
                        else if(seq_b == seq[1]){cntpattern[self][HALF_TWO]++;}
                        else if(seq_b == seq[2]){cntpattern[self][HALF_TWO]++;}
                        else if(seq_b == seq[3]){}
                    }
                    //two

                    if(is_on_board_ev(i + 4 * vec_i[dir], j + 4 * vec_j[dir])){
                        std::vector<int> seq[4] = {
                            {EMPTY, self, self, self, EMPTY}, 
                            {EMPTY, self, self, self, opp}, 
                            {opp, self, self, self, EMPTY}, 
                            {opp, self, self, self, opp}
                        };
                        std::vector<int> seq_b;
                        for(int l = 0; l <= 4; l++){
                            seq_b.push_back(board_to_ev[i + l * vec_i[dir]][j + l * vec_j[dir]]);
                        }
                        if(seq_b == seq[0]){cntpattern[self][OPEN_THREE]++;}
                        else if(seq_b == seq[1]){cntpattern[self][HALF_THREE]++;}
                        else if(seq_b == seq[2]){cntpattern[self][HALF_THREE]++;}
                        else if(seq_b == seq[3]){}
                    }
                    //three

                    if(is_on_board_ev(i + 5 * vec_i[dir], j + 5 * vec_j[dir])){
                        std::vector<int> seq[4] = {
                            {EMPTY, self, self, self, self, EMPTY}, 
                            {EMPTY, self, self, self, self, opp}, 
                            {opp, self, self, self, self, EMPTY}, 
                            {opp, self, self, self, self, opp}
                        };
                        std::vector<int> seq_b;
                        for(int l = 0; l <= 5; l++){
                            seq_b.push_back(board_to_ev[i + l * vec_i[dir]][j + l * vec_j[dir]]);
                        }
                        if(seq_b == seq[0]){cntpattern[self][OPEN_FOUR]++;}
                        else if(seq_b == seq[1]){cntpattern[self][HALF_FOUR]++;}
                        else if(seq_b == seq[2]){cntpattern[self][HALF_FOUR]++;}
                        else if(seq_b == seq[3]){}
                    }
                    //four

                    if(is_on_board_ev(i + 2 * vec_i[dir], j + 2 * vec_j[dir]) &&
                       is_on_board_ev(i - 1 * vec_i[dir], j - 1 * vec_j[dir])){
                        std::vector<int> seq[1] = {
                            {self, EMPTY, self, self}
                        };
                        std::vector<int> seq_b;
                        for(int l = -1; l <= 2; l++){
                            seq_b.push_back(board_to_ev[i + l * vec_i[dir]][j + l * vec_j[dir]]);
                        }
                        if(seq_b == seq[0]){
                            cntpattern[self][GAP_THREE]++;
                            // std::cout << "discovered GAP_THREE at (" << i << ", " << j << ")" << std::endl;
                        }
                    }
                    //gap three
                }
            }
        }
        
        
    }

    void update_value(){
        if(winner == BLACK) {value = INT_MAX; return;}
        if(winner == WHITE) {value = INT_MIN; return;}
        this->value = 480 * (this->cntpattern[BLACK][OPEN_FOUR] - this->cntpattern[WHITE][OPEN_FOUR]) +
                        0 * (this->cntpattern[BLACK][GAP_THREE] - this->cntpattern[WHITE][GAP_THREE]) +
                       50 * (this->cntpattern[BLACK][HALF_FOUR] - this->cntpattern[WHITE][HALF_FOUR]) +
                       50 * (this->cntpattern[BLACK][OPEN_THREE] - this->cntpattern[WHITE][OPEN_THREE]) + 
                       20 * (this->cntpattern[BLACK][HALF_THREE] - this->cntpattern[WHITE][HALF_THREE]) +
                        5 * (this->cntpattern[BLACK][OPEN_TWO] - this->cntpattern[WHITE][OPEN_TWO]) +
                        1 * (this->cntpattern[BLACK][HALF_TWO] - this->cntpattern[WHITE][HALF_TWO]);
        return;
    }

    void update(int color, int pos_i, int pos_j){
        cntdisc++;
        stateboard[pos_i][pos_j] = color;
        update_cntpattern(BLACK);
        update_cntpattern(WHITE);
        update_value();
    }
    
    std::array<std::array<int, SIZE>, SIZE> stateboard;
    int value;
    int cntdisc;
    std::array<std::array<int, 10>, 3> cntpattern;
    int winner;
    std::array<std::array<bool, SIZE>, SIZE> recommended;
};

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

int alphabeta(state curstate, int depth, int alpha, int beta, bool maximize_value){
    int value;
    if(depth == 0 || curstate.cntdisc == SIZE * SIZE) return curstate.value;

    if(maximize_value){
        if(curstate.winner == WHITE) return INT_MIN;
        if(curstate.winner == BLACK) return INT_MAX;

        value = INT_MIN;
        curstate.set_recommended();
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j < SIZE; j++){
                if(curstate.stateboard[i][j] == EMPTY && (curstate.recommended[i][j] == true || curstate.cntdisc == 0)){
                    state next(curstate);
                    next.update(BLACK, i, j);
                    value = std::max(value, alphabeta(next, depth - 1, alpha, beta, false));
                    alpha = std::max(alpha, value);
                    if(alpha >= beta) return alpha;
                }
            }
        }
        return value;
    }

    else{
        if(curstate.winner == WHITE) return INT_MIN;
        if(curstate.winner == BLACK) return INT_MAX;

        value = INT_MAX;
        curstate.set_recommended();
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j < SIZE; j++){
                if(curstate.stateboard[i][j] == EMPTY && (curstate.recommended[i][j] == true || curstate.cntdisc == 0)){
                    state next(curstate);
                    next.update(WHITE, i, j);
                    value = std::min(value, alphabeta(next, depth - 1, alpha, beta, true));
                    beta = std::min(beta, value);
                    if(alpha >= beta) return beta;
                }
            }
        }
        return value;
    }
}

void write_valid_spot(std::ofstream& fout) {
    state now(board);

    if(player == BLACK){
    int value = INT_MIN;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(board[i][j] == EMPTY && (now.recommended[i][j] == true || now.cntdisc == 0)){
                
                if(now.cntdisc == 0){
                    fout << 7 << " " << 7 << std::endl;
                    return;
                }
                state next(now);
                next.update(BLACK, i, j);
                // std::cout << i << " " << j << std::endl;
                // std::cout << next.value << std::endl;
                
                if(next.winner == BLACK){
                    value = INT_MAX;
                    fout << i << " " << j << std::endl;
                    fout.flush();
                }
                else{
                    int nextvalue = alphabeta(next, 1, INT_MIN, INT_MAX, false);
                    if(value <= nextvalue){
                        value = nextvalue;
                        fout << i << " " << j << std::endl;
                        fout.flush();
                    }
                }
            }
        }
    }
    }

    else if(player == WHITE){
    int value = INT_MAX;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(board[i][j] == EMPTY && (now.recommended[i][j] == true || now.cntdisc == 0)){
                
                if(now.cntdisc == 0){
                    fout << 7 << " " << 7 << std::endl;
                    return;
                }
                state next(now);
                next.update(WHITE, i, j);
                // std::cout << i << " " << j << std::endl;
                // std::cout << next.value << std::endl;
                
                if(next.winner == WHITE){
                    value = INT_MIN;
                    fout << i << " " << j << std::endl;
                    fout.flush();
                }
                else{
                    int nextvalue = alphabeta(next, 1, INT_MIN, INT_MAX, true);
                    if(value >= nextvalue){
                        value = nextvalue;
                        fout << i << " " << j << std::endl;
                        fout.flush();
                    }
                }
            }
        }
    }
    }
}


int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
