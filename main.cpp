#include <iostream>
#include "Puzzle.h"
#include "Puzzle.cpp"
#include "SolverBFS.h"
#include "SolverBFS.cpp"

using namespace std;


int main() {
    // Criando um quebra-cabeça de exemplo
    std::vector<int> initialPuzzle = {1, 2, 3, 4, 0, 5, 6, 7, 8};
    Puzzle puzzle(initialPuzzle, nullptr); // nullptr porque é o estado inicial
    
    // Imprimindo o quebra-cabeça inicial
    std::cout << "Quebra-cabeca inicial:\n";
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
    
      
    std::cout << "\nTestando se o Goal foi alcancado:\n";
    std::vector<int> testeGoalPuzzle = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    Puzzle puzzle2(testeGoalPuzzle, nullptr); // 
	
	State s;
	s.board = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
    std::cout << puzzle2.isGoal(s);
    
    //Testando movimentos válidos obtidos
    std::cout << "\n\n\nVerificacao de movimentos validos:\n";
    State x;
	x.board = {{1, 2, 3}, {4, 0, 5}, {6, 7, 8}};
	cout << "Estado atual do quebra-cabeca:\n";
    for (const auto& row : x.board) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << "\n";
    }
	// Testando movimentos válidos obtidos
    std::cout << "\nMovimentos validos para o estado atual:\n";
    ValidMoves moves = get_moves(&x);
    // Imprime os movimentos
	std::cout << "Movimentos:\n";
	for (const auto& move : moves.moves) {
	    std::cout << "De (" << move.first_position.first << ", " << move.first_position.second << ") ";
	    std::cout << "para (" << move.second_position.first << ", " << move.second_position.second << ")\n";
	}
	
	// Imprime os nomes dos movimentos
	std::cout << "Nomes dos movimentos:\n";
	for (const auto& move_name : moves.move_names) {
	    std::cout << move_name << "\n";
	}


    
    //Chamando função para calcular distancia manhattan
    int puzzleArray[3][3] = {{2, 4, 7}, {0, 3, 6}, {8, 1, 5}};
    cout << "\n\n\nEstado atual do quebra-cabeca:\n";
    for (const auto& row : x.board) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << "\n";
    }
    int distanceManhattan = puzzle2.manhattan_distance_matrix(puzzleArray);
    
	std::cout << "Distancia manhattan: " << distanceManhattan << std::endl;
	
	SolverBFS sbfs(initialPuzzle);
	//sbfs.solve();

    return 0;
}
