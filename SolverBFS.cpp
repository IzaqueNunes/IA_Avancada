#include "SolverBFS.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include "Puzzle.h"

using namespace std;

SolverBFS::SolverBFS() : puzzle(nullptr) {}

SolverBFS::SolverBFS(const std::vector<int>& initialState) {
    puzzle = new Puzzle(initialState, nullptr);
}

SolverBFS::~SolverBFS() {
    delete puzzle;
}

void SolverBFS::solve() {
    if (puzzle == nullptr) {
        std::cerr << "SolverBFS: Puzzle não inicializado." << std::endl;
        return;
    }

    std::queue<Puzzle*> statesQueue;
    
    
    /*std::unordered_set<std::vector<std::vector<int>>, std::hash<std::vector<std::vector<int>>>, std::equal_to<std::vector<std::vector<int>>>> visited;

    statesQueue.push(puzzle);

    while (!statesQueue.empty()) {
        Puzzle* currentPuzzle = statesQueue.front();
        statesQueue.pop();

        // Verifica se o estado atual é o objetivo
        if (currentPuzzle->isGoal(currentPuzzle->getState())) {
            // Estado objetivo encontrado, faça o que for necessário
            // Você pode implementar a lógica para rastrear a solução aqui
            return;
        }

        // Expande os estados sucessores
        std::vector<Move> possibleMoves = currentPuzzle->getMoves(currentPuzzle->getState());
        for (const auto& move : possibleMoves) {
            Puzzle* successor = new Puzzle(currentPuzzle->getStateAfterMove(move), currentPuzzle);
            if (visited.emplace(successor->getState()).second) { // Inserir e verificar se a inserção foi bem-sucedida
                statesQueue.push(successor);
            } else {
                delete successor;
            }
        }
    }

    std::cerr << "SolverBFS: Não foi possível encontrar uma solução." << std::endl;*/
}
