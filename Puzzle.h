#ifndef PUZZLE_H
#define PUZZLE_H
#include <iostream>
#include <vector>

// Define a estrutura de um estado com um tabuleiro 2D
struct State {
    std::vector<std::vector<int>> board;
    std::pair<int, int> blank_pos;

    State copy();
};

struct Move {
    std::pair<int, int> first_position;
    std::pair<int, int> second_position;
};

typedef struct ValidMoves {
    std::vector<Move> moves;
    std::vector<std::string> move_names;
} ValidMoves;

// Classe do quebra-cabeça com um tabuleiro 2D
class Puzzle {
private:
    std::vector<std::vector<int>> board; // Alterado para matriz 2D
    Puzzle *parent;
    std::vector<Puzzle *> children;
    std::vector<std::vector<int>> goal_8puzzle = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

public:
    Puzzle(const std::vector<std::vector<int>> &initial_board, Puzzle *_parent);
    int manhattan_distance_matrix(const std::vector<std::vector<int>> &puzzle);
    const std::vector<std::vector<int>>& getBoard() const { 
        return board; 
    }
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
    void printPuzzle();
    bool isGoal(const State &s);

    friend ValidMoves get_moves(State *s);
};

ValidMoves get_moves(State *s);

#endif