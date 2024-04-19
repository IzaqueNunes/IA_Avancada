#include <iostream>
#include "Puzzle.h"
#include "SolverBFS.h"

using namespace std;


int main() {
    // Criando um quebra-cabeça de exemplo
    std::vector<int> initialPuzzle = {1, 2, 3, 4, 0, 5, 6, 7, 8};
    Puzzle puzzle(initialPuzzle, nullptr); // nullptr porque é o estado inicial
    
	
    //Imprimindo o quebra-cabeça inicial
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
    
    //Chamando função para calcular distancia manhattan
    int puzzleArray[3][3] = {{2, 4, 7}, {0, 3, 6}, {8, 1, 5}};
    int distanceManhattan = puzzle2.manhattan_distance_matrix(puzzleArray);
    
	std::cout << "Distancia manhattan: " << distanceManhattan << std::endl;
	
	SolverBFS sbfs(initialPuzzle);
	//sbfs.solve();

    return 0;
}
