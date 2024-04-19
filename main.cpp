#include <iostream>
#include "Puzzle.h"

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
    std::cout << puzzle2.isGoal(s);

    return 0;
}
