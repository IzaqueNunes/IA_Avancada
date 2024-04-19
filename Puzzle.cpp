#include <iostream>
#include <algorithm>
#include "Puzzle.h"


using namespace std;

State State::copy() {
    State new_state;
    new_state.board = this->board;
    new_state.blank_pos = this->blank_pos;
    return new_state;
}

ValidMoves get_moves(State *s) {
    // Declaração de uma variável para armazenar a posição do espaço em branco (zero)
    std::pair<int, int> blank_pos = s->blank_pos;
    // Inicialização de um vetor para armazenar os movimentos válidos
    std::vector<Move> valid_moves;
    // Vetor para armazenar os nomes dos movimentos válidos
    std::vector<std::string> move_names; 

    // Verifica se é possível mover para cima
    if (blank_pos.first - 1 >= 0) {
        // Criação de um movimento para cima e o adiciona ao vetor de movimentos válidos
        Move up = {blank_pos, {blank_pos.first - 1, blank_pos.second}};
        valid_moves.push_back(up);
        move_names.push_back("up"); // Adiciona o nome do movimento ao vetor paralelo
    }

    // Verifica se é possível mover para baixo
    if (blank_pos.first + 1 < s->board.size()) {
        // Criação de um movimento para baixo e o adiciona ao vetor de movimentos válidos
        Move down = {blank_pos, {blank_pos.first + 1, blank_pos.second}};
        valid_moves.push_back(down);
        move_names.push_back("down"); // Adiciona o nome do movimento ao vetor paralelo
    }

    // Verifica se é possível mover para a esquerda
    if (blank_pos.second - 1 >= 0) {
        // Criação de um movimento para a esquerda e o adiciona ao vetor de movimentos válidos
        Move left = {blank_pos, {blank_pos.first, blank_pos.second - 1}};
        valid_moves.push_back(left);
        move_names.push_back("left"); // Adiciona o nome do movimento ao vetor paralelo
    }

    // Verifica se é possível mover para a direita
    if (blank_pos.second + 1 < s->board[0].size()) {
        // Criação de um movimento para a direita e o adiciona ao vetor de movimentos válidos
        Move right = {blank_pos, {blank_pos.first, blank_pos.second + 1}};
        valid_moves.push_back(right);
        move_names.push_back("right"); // Adiciona o nome do movimento ao vetor paralelo
    }

    // Retorna os movimentos válidos e seus nomes correspondentes
    return {valid_moves, move_names};
}

Puzzle::Puzzle(std::vector<int> _puzzle, Puzzle *_parent) {
    puzzle = _puzzle;
    parent = _parent;

    // Encontra a posição do espaço em branco
    /*for (int i = 0; i < _puzzle.size(); ++i) {
        if (_puzzle[i] == 0) {
            State initial_state;
            initial_state.board = {{_puzzle[0], _puzzle[1], _puzzle[2]},
                                   {_puzzle[3], _puzzle[4], _puzzle[5]},
                                   {_puzzle[6], _puzzle[7], _puzzle[8]}};
            initial_state.blank_pos = {i / 3, i % 3};
            break;
        }
    }*/
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


bool Puzzle::isGoal(State s) { 
    // Verifica se o estado atual é igual ao estado objetivo
    return s.board == Puzzle::goal_8puzzle;
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
State State::copy() {
    State new_state;
    new_state.board = this->board;
    return new_state;
}
*/
/*
std::vector<Move> get_moves(State *s) {
    std::pair<int, int> blank_pos = s->blank_pos;
    std::vector<Move> valid_moves;

    // Verifica se é possível mover para cima
    if (blank_pos.first - 1 >= 0) {
        Move up = {blank_pos, {blank_pos.first - 1, blank_pos.second}};
        valid_moves.push_back(up);
    }

    // Verifica se é possível mover para baixo
    if (blank_pos.first + 1 < s->board.size()) {
        Move down = {blank_pos, {blank_pos.first + 1, blank_pos.second}};
        valid_moves.push_back(down);
    }

    // Verifica se é possível mover para a esquerda
    if (blank_pos.second - 1 >= 0) {
        Move left = {blank_pos, {blank_pos.first, blank_pos.second - 1}};
        valid_moves.push_back(left);
    }

    // Verifica se é possível mover para a direita
    if (blank_pos.second + 1 < s->board[0].size()) {
        Move right = {blank_pos, {blank_pos.first, blank_pos.second + 1}};
        valid_moves.push_back(right);
    }

    return valid_moves;
}
*/

/*
Puzzle::Puzzle(std::vector<int> _puzzle, Puzzle *_parent) {
    puzzle = _puzzle;
    parent = _parent;
}*/

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