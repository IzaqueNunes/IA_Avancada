#include "SolverBFS.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include "Puzzle.h"
#include <chrono> 

// Função de hash personalizada para vetor de vetores (matriz 2D)
struct HashVector {
    std::size_t operator()(const std::vector<std::vector<int>>& v) const {
        std::hash<int> intHasher; // Função hash para inteiros
        std::size_t hash = 0;

        // Calcular hash para a matriz 2D
        for (const auto& row : v) {
            for (int val : row) {
                // Combinar o hash de cada valor com um fator arbitrário (31)
                hash ^= intHasher(val) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
        }
        return hash;
    }
};

using namespace std;

// Construtor padrão
SolverBFS::SolverBFS() : puzzle(nullptr) {}

// Construtor que aceita uma matriz 2D
SolverBFS::SolverBFS(const std::vector<std::vector<int>>& initialState) { 
    puzzle = new Puzzle(initialState, nullptr); // Criar um novo Puzzle
}

// Destrutor para limpar a memória
SolverBFS::~SolverBFS() { 
    delete puzzle; 
}

void SolverBFS::solve() {
    if (puzzle == nullptr) {
        std::cerr << "SolverBFS: Puzzle não inicializado." << std::endl;
        return;
    }

    std::queue<Puzzle*> statesQueue;

    // Inicialize a fila e o contador de nós expandidos
    statesQueue.push(puzzle);
    int expanded_nodes = 0;

    while (!statesQueue.empty()) {
        Puzzle* currentPuzzle = statesQueue.front(); // Retira o nó da frente da fila
        statesQueue.pop();

        // Incrementa o contador de nós expandidos
        expanded_nodes++;

        // Crie um objeto `State` para representar o estado atual
        State current_state;
        current_state.board = currentPuzzle->getBoard(); // Atribuir a matriz do puzzle ao campo `board`

        // Verifica se o estado atual é o objetivo
        if (currentPuzzle->isGoal(current_state)) {
            std::cout << "Estado objetivo encontrado!" << std::endl;
            std::cout << "Número de nós expandidos: " << expanded_nodes << std::endl;
            return; // Solução encontrada, encerra o loop
        }

        // Obter movimentos válidos para o estado atual
        ValidMoves possibleMoves = get_moves(&current_state);

        std::vector<std::vector<int>> original_board = currentPuzzle->getBoard();

        for (const auto& move : possibleMoves.moves) {
            // Crie um objeto `State` para representar o estado sucessor
            State successor_state = make_move(current_state, move);

            // Verifique se o estado sucessor é o mesmo que o estado original
            if (successor_state.board == original_board) {
                continue; // Se for igual ao estado original, pula para evitar loops
            }

            // Crie um novo Puzzle com o estado sucessor
            Puzzle* successor = new Puzzle(successor_state.board, currentPuzzle);
            statesQueue.push(successor); // Adiciona o sucessor à fila
        }
    }

    std::cerr << "SolverBFS: Não foi possível encontrar uma solução." << std::endl;
    std::cout << "Número de nós expandidos: " << expanded_nodes << std::endl;
}