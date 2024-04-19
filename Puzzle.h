#ifndef PUZZLE_H
#define PUZZLE_H
#include <iostream>
#include <vector>

//using namespace std;


struct State
{
    std::vector<std::vector<int>> board;
    std::pair<int, int> blank_pos;

    State copy();
};

struct Move
{
    std::pair<int, int> first_position;
    std::pair<int, int> second_position;
};

// Para retornar movimento + nome
typedef struct ValidMoves {
    std::vector<Move> moves;
    std::vector<std::string> move_names;
} ValidMoves;

class Puzzle
{
private:
    std::vector<int> puzzle;
    Puzzle *parent;
    std::vector<Puzzle *> children;
    std::vector<std::vector<int>> goal_8puzzle = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

public:
    Puzzle(std::vector<int> _puzzle, Puzzle *_parent);
    int manhattan_distance_matrix(int puzzle[][3]);
    int findZero();
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
    void printPuzzle();
    bool isGoal(State s);

    //friend std::vector<Move> get_moves(State *s);
    friend ValidMoves get_moves(State *s);
};

//std::vector<Move> get_moves(State *s);
ValidMoves get_moves(State *s);

#endif

/*

struct State
{
    std::vector<std::vector<int>> board;
    std::pair<int, int> blank_pos;

    State copy();
};

struct Move
{
    std::pair<int, int> first_position;
    std::pair<int, int> second_position;
};

class Puzzle
{
public:
    std::vector<Puzzle *> children;
    std::vector<int> puzzle;
    Puzzle *parent;
    std::vector<std::vector<int>> goal_8puzzle = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

    int findZero();
    //std::vector<Move> getMoves(State s);

public:
    Puzzle(std::vector<int> _puzzle, Puzzle *_parent);
    int manhattan_distance_matrix(int puzzle[][3]);
    // int traceSolution(std::vector<int> _puzzle);
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
    void printPuzzle();
    bool isGoal(State s);
    friend std::vector<Move> get_moves(State *s);
};

#endif

*/