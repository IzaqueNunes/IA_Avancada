#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <chrono>
#include <map>
#include "Puzzle.h"

using namespace std;
    
/*
    #import "puzzle.h"
    int depth_limited_search(State s, int depth, int depth_limit){
    	if(puzzle::is_goal(s)){
    		return depth;
		}
		
		if (depth_limit > 0){
			for(puzzle::Move m:puzzle::successors(s)){
			 //PARA LEMBRAR: Move = (current_blank_position, next_position)
			 
			 State next_state = puzzle::make_move(s, m);
			 int solution = depth_limited_search(next_state, depth+1, depth_limit);
			 if(solution != SEM_SOLUCAO){ //SEM_SOLUCAO=-1
			 	return solution;
			 }
			 }
		}
		return NO_SOLUTION;
			
	}
		
}
*/ 

int traceSolution(vector<Puzzle *> sol, Puzzle *g) {
    Puzzle *curr = g;
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

int recursive_idfs(int maxDepth, State *currentState){
		int objective = 0;
    	vector<Puzzle *> solution;
        if (Puzzle::is_goal(currentState)) {
            	this->depth = traceSolution(solution, currentChild);
            	return depth;
        }
        if (maxDepth > 0) {
            this->Puzzle::nodesCount++;          
           //for (auto currentChild : currentNode->children) {              
			for (move m : Puzzle::get_moves(S)) {              
        		State next_state = Puzzle::make_move(S, m);
				if (visited.count(next_state) == 0) {
                    this->visited[next_state] = true;
                    //this->stack.push(currentChild);
                    objective = recursive_idfs(maxDepth -1, next_state);
                    //achou
                    if(objective != -1){
                    	return objective;
					}                    
                }
            }            
        }
        return -1;
        
}  

void IDFS(vector<int> initialVector){
        int objective = 0;
       int depth = 0;
        
        Puzzle *initialNode = new Puzzle(initialVector, nullptr);
        
        while (objective == 0 && depth < 32 ){
        	cout << depth << endl;
        	objective = recursive_idfs(depth, initialNode);
        	if(objective != -1){break;}
			}
        	depth++;
		}		
	}
	
 

int main() {
    ifstream inputFile("input/teste.txt"); // Abre o arquivo de entrada

    if (!inputFile.is_open()) { // Verifica se o arquivo foi aberto corretamente
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return 1;
    }

    string line;
    vector<vector<int>> initialStates; // Vetor para armazenar os estados iniciais

    // Lê os estados iniciais do arquivo
    while (getline(inputFile, line)) {
        istringstream iss(line);
        vector<int> initialState;
        int num;

        while (iss >> num) {
            initialState.push_back(num);
        }

        initialStates.push_back(initialState);
    }

    inputFile.close(); // Fecha o arquivo de entrada

    for (int i = 0; i < initialStates.size(); ++i) {
        auto start_time = chrono::high_resolution_clock::now();

        Puzzle puzzle(initialStates[i], {0, 1, 2, 3, 4, 5, 6, 7, 8});
        puzzle.IDFS(initialStates[i]);

        auto end_time = chrono::high_resolution_clock::now();
        auto duration_sec = chrono::duration<double>(end_time - start_time).count();

        cout << puzzle.nodesCount << ", " << puzzle.depth - 1 << ", " << duration_sec << ", " << "0, ";

        // Calcular e imprimir a heurística (distância de Manhattan)
        int puzzleArray[3][3] = {{initialStates[i][0], initialStates[i][1], initialStates[i][2]},
                                  {initialStates[i][3], initialStates[i][4], initialStates[i][5]},
                                  {initialStates[i][6], initialStates[i][7], initialStates[i][8]}};
        int distanceManhattan = manhattan_distance_matrix(puzzleArray);
        cout << distanceManhattan << endl;
    }

    return 0;
}