#include <iostream>
#include <algorithm>
#include "Puzzle.h"


using namespace std;

State State::copy() {
    State new_state;
    new_state.board = this->board;
    return new_state;
}

State make_move(State S, Move m) {
    State next_S = S.copy();
    std::pair<int,int> last_pos = m.first_position;
    std::pair<int,int> next_pos = m.second_position;
    int s = S.board[next_pos.first][next_pos.second];
    next_S.board[next_pos.first][next_pos.second] = 0;
    next_S.board[last_pos.first][last_pos.second] = s;
    next_S.blank_pos = m.second_position;
    return next_S;
}

std::vector<Move> get_moves(State *s) {
    std::pair<int, int> blank_pos = s->blank_pos;
    std::vector<Move> valid_moves;
    if (blank_pos.first - 1 >= 0) {
        Move left = {blank_pos, {blank_pos.first - 1, blank_pos.second}};
        valid_moves.push_back(left);
    }
    // Check other directions and add valid moves
    return valid_moves;
}

bool Puzzle::isGoal(State s) { 
    // Verifica se o estado atual é igual ao estado objetivo
    return s.board == Puzzle::goal_8puzzle;
}


Puzzle::Puzzle(std::vector<int> _puzzle, Puzzle *_parent) {
    puzzle = _puzzle;
    parent = _parent;
}

void Puzzle::printPuzzle() {
    int count = 0;
    for (auto i : puzzle) {
        if (count % 3 == 0)
            std::cout << "\n";
        std::cout << i << ' ';
        count++;
    }
}

int Puzzle::findZero() {
    auto it = std::find(puzzle.begin(), puzzle.end(), 0);
    return static_cast<int>(std::distance(puzzle.begin(), it));
}

void Puzzle::moveUp() {
    int zPos = findZero();
    std::vector<int> temp = puzzle;
    if (zPos >= 3) {
    	swap(temp[zPos], temp[zPos - 3]);
        Puzzle *child = new Puzzle(temp, this);
        children.push_back(child);
        std::swap(puzzle[zPos], puzzle[zPos - 3]);
    }
}

void Puzzle::moveDown() {
    int zPos = findZero();
    std::vector<int> temp = puzzle;
    if (zPos < 6) {
    	swap(temp[zPos], temp[zPos + 3]);
        Puzzle *child = new Puzzle(temp, this);
        children.push_back(child);    	
        std::swap(puzzle[zPos], puzzle[zPos + 3]);
    }
}

void Puzzle::moveRight() {
    int zPos = findZero();
    std::vector<int> temp = puzzle;
    if (zPos % 3 < 2) {
    	swap(temp[zPos], temp[zPos + 1]);
        Puzzle *child = new Puzzle(temp, this);
        children.push_back(child);
        std::swap(puzzle[zPos], puzzle[zPos + 1]);
    }
}

void Puzzle::moveLeft() {
    int zPos = findZero();
    std::vector<int> temp = puzzle;
    if (zPos % 3 > 0) {
    	swap(temp[zPos], temp[zPos - 1]);
        Puzzle *child = new Puzzle(temp, this);
        children.push_back(child);
        std::swap(puzzle[zPos], puzzle[zPos - 1]);
    }
}

/*
// Função para rastrear a solução a partir do nó final
int Puzzle::traceSolution(vector<Puzzle *> sol, Puzzle *g) {
    Node *curr = g;
    sol.push_back(g);

    while (curr->parent != nullptr) {
        curr = curr->parent;
        sol.push_back(curr);
    }

    reverse(sol.begin(), sol.end());

    int depth = 0;
    for (auto i : sol) {
        depth += 1;
        // i->printPuzzle();
        // cout << "\n";
    }
    return depth;
}
*/

// Função para calcular a distância de Manhattan de um puzzle 8 tile 
int Puzzle::manhattan_distance_matrix(int puzzle[][3]) {
    int sum = 0;
    int n = 3;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int num = puzzle[i][j];
            if (num == 0) {
                continue; // Ignora o espaço vazio.
            }

            // Encontra a posição correta do número.
            int row_goal = num / n;
            int col_goal = num % n;

            // Calcula a distância de Manhattan.
            int row_diff = abs(i - row_goal);
            int col_diff = abs(j - col_goal);
            sum += row_diff + col_diff;
        }
    }

    return sum;
}