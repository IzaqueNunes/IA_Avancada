#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

// 1- Estruturar o header
// 2- Criar main
// 3- Testar métodos (Puzzle)
// 4- Chamar bfs na main e testar


using namespace std;

Struct state;

class Puzzle {
/*private:
    stack<Node *> stack; // Fila para nós ainda não explorados (open)
    map<vector<int>, bool> visited; // Mapa para estados já visitados (closed)
    vector<int> finalVector; // Estado final do quebra-cabeça
*/
    
    vector<Puzzle*> children;
  	vector<int> puzzle;
  	Puzzle *parent;
  	Puzzle(vector<int> _puzzle, Puzzle *_parent) {
    	puzzle = _puzzle;
    	parent = _parent;
  	}
	  
	void printPuzzle();
  	int findZero();
/*
  	void moveUp();
  	void moveDown();
  	void moveRight();
  	void moveLeft();
*/
/*
Struct State
	1 2 3
	4 0 5
	6 7 8
	
	(1,1)

Move{last_pos, curr_pos}
(1,1) (0,1)
(1,1) (1,2)
(1,1) (1,0)
*/
	vector<Moves> getMoves(State s);	  
};
#endif