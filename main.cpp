#include <iostream>
#include "Puzzle.h"
#include "Puzzle.cpp"
#include "SolverBFS.h"
#include "SolverBFS.cpp"

using namespace std;

int main() {
    // Criando um quebra-cabeça de exemplo com um tabuleiro 2D
    std::vector<std::vector<int>> initialPuzzle = {
        {0, 6, 1},
        {7, 4, 2},
        {3, 8, 5}
    };
    
    Puzzle puzzle(initialPuzzle, nullptr); // nullptr porque é o estado inicial
    
    // Imprimindo o quebra-cabeça inicial
    std::cout << "Quebra-cabeça inicial:\n";
    puzzle.printPuzzle();

    // Testando os movimentos possíveis
    std::cout << "\nMovendo para cima:\n";
    puzzle.moveUp();
    puzzle.printPuzzle();

    std::cout << "\nMovendo para baixo:\n";
    puzzle.moveDown();
    puzzle.printPuzzle();

    std::cout << "\nMovendo para a direita:\n";
    puzzle.moveRight();
    puzzle.printPuzzle();

    std::cout << "\nMovendo para a esquerda:\n";
    puzzle.moveLeft();
    puzzle.printPuzzle();

    std::cout << "\nTestando se o objetivo foi alcançado:\n";
    State goal_state = {
        {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}},
        {0, 0}  // posição do espaço em branco no objetivo
    };
    std::cout << (puzzle.isGoal(goal_state) ? "Sim" : "Não") << "\n";

    // Testando movimentos válidos obtidos
    std::cout << "\n\nVerificação de movimentos válidos:\n";
    State s = {
        {{1, 2, 3}, {4, 0, 5}, {6, 7, 8}},
        {1, 1}  // posição do espaço em branco
    };
    ValidMoves moves = get_moves(&s);
    
    // Imprimindo movimentos válidos
    std::cout << "Movimentos válidos:\n";
    for (const auto& move : moves.moves) {
        std::cout << "De (" << move.first_position.first << ", " << move.first_position.second << ") ";
        std::cout << "para (" << move.second_position.first << ", " << move.second_position.second << ")\n";
    }

    std::cout << "Nomes dos movimentos:\n";
    for (const auto& move_name : moves.move_names) {
        std::cout << move_name << "\n";
    }

    // Chamando função para calcular a distância de Manhattan
    int distanceManhattan = puzzle.manhattan_distance_matrix(s.board);
    std::cout << "Distância Manhattan: " << distanceManhattan << "\n";

    // Instanciando o SolverBFS com o tabuleiro 2D
    SolverBFS sbfs(initialPuzzle);
    sbfs.solve(); // Descomente para resolver o quebra-cabeça usando BFS

    return 0;
}
