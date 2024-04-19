#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <vector>

using namespace std;

struct State {
    std::vector<std::vector<int>> board;
    std::pair<int, int> blank_pos;

    State copy();
};

struct Move {
    std::pair<int, int> first_position;
    std::pair<int, int> second_position;
};

class Puzzle {
private:
    std::vector<Puzzle*> children;
    std::vector<int> puzzle;
    Puzzle *parent;
    std::vector<std::vector<int>> goal_8puzzle = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

    int findZero();
    std::vector<Move> getMoves(State s);
    

public:
    Puzzle(std::vector<int> _puzzle, Puzzle *_parent);
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
    void printPuzzle();
    bool isGoal(State s);
};

#endif
