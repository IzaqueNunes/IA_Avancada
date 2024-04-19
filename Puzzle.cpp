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
    // Define o estado objetivo
    std::vector<std::vector<int>> goal_board = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

    // Verifica se o estado atual é igual ao estado objetivo
    return s.board == goal_board;
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

int manhattan_distance_matrix(std::vector<std::vector<int>> puzzle) {
    int sum = 0;
    int n = puzzle.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int num = puzzle[i][j];
            if (num == 0) {
                continue; // Ignore the blank space.
            }

            // Find the correct position of the number.
            int row_goal = num / n;
            int col_goal = num % n;

            // Calculate Manhattan distance.
            int row_diff = abs(i - row_goal);
            int col_diff = abs(j - col_goal);
            sum += row_diff + col_diff;
        }
    }

    return sum;
}