#include <iostream>
#include <algorithm>
#include "Puzzle.h"

State State::copy() {
    State new_state;
    new_state.board = this->board;
    new_state.blank_pos = this->blank_pos;
    return new_state;
}

ValidMoves get_moves(State *s) {
    std::pair<int, int> blank_pos = s->blank_pos;
    std::vector<Move> valid_moves;
    std::vector<std::string> move_names;

    if (blank_pos.first - 1 >= 0) {
        Move up = {blank_pos, {blank_pos.first - 1, blank_pos.second}};
        valid_moves.push_back(up);
        move_names.push_back("up");
    }

    if (blank_pos.first + 1 < s->board.size()) {
        Move down = {blank_pos, {blank_pos.first + 1, blank_pos.second}};
        valid_moves.push_back(down);
        move_names.push_back("down");
    }

    if (blank_pos.second - 1 >= 0) {
        Move left = {blank_pos, {blank_pos.first, blank_pos.second - 1}};
        valid_moves.push_back(left);
        move_names.push_back("left");
    }

    if (blank_pos.second + 1 < s->board[0].size()) {
        Move right = {blank_pos, {blank_pos.first, blank_pos.second + 1}};
        valid_moves.push_back(right);
        move_names.push_back("right");
    }

    return {valid_moves, move_names};
}

Puzzle::Puzzle(const std::vector<std::vector<int>> &initial_board, Puzzle *_parent) {
    board = initial_board;
    parent = _parent;
}

bool Puzzle::isGoal(const State &s) {
    return s.board == goal_8puzzle;
}

void Puzzle::printPuzzle() {
    for (const auto &row : board) {
        for (int num : row) {
            std::cout << num << " ";
        }
        std::cout << "\n";
    }
}

void Puzzle::moveUp() {
    int row = 0, col = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                row = i;
                col = j;
                break;
            }
        }
    }
    if (row > 0) {
        std::swap(board[row][col], board[row - 1][col]);
    }
}

void Puzzle::moveDown() {
    int row = 0, col = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                row = i;
                col = j;
                break;
            }
        }
    }
    if (row < 2) {
        std::swap(board[row][col], board[row + 1][col]);
    }
}

void Puzzle::moveRight() {
    int row = 0, col = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                row = i;
                col = j;
                break;
            }
        }
    }
    if (col < 2) {
        std::swap(board[row][col], board[row][col + 1]);
    }
}

void Puzzle::moveLeft() {
    int row = 0, col = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                row = i;
                col = j;
                break;
            }
        }
    }
    if (col > 0) {
        std::swap(board[row][col], board[row][col - 1]);
    }
}

int Puzzle::manhattan_distance_matrix(const std::vector<std::vector<int>> &puzzle) {
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
            int row_diff = std::abs(i - row_goal);
            int col_diff = std::abs(j - col_goal);
            sum += row_diff + col_diff;
        }
    }

    return sum;
}

State make_move(State s, Move m) {
    State next_s = s.copy();
    std::pair<int, int> last_pos = m.first_position;
    std::pair<int, int> next_pos = m.second_position;

    int temp = next_s.board[next_pos.first][next_pos.second];
    next_s.board[next_pos.first][next_pos.second] = 0;
    next_s.board[last_pos.first][last_pos.second] = temp;
    next_s.blank_pos = m.second_position;

    return next_s;
}