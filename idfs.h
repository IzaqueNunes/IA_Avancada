#ifndef IDFS_H
#define IDFS_H

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

// Declaração da classe Node
class IDFSNode {
public:
    std::vector<IDFSNode*> children; // Filhos do nó
    std::vector<int> initialVector; // Estado do quebra-cabeça
    IDFSNode* parent; // Ponteiro para o nó pai

    // Construtor do Node
    IDFSNode(const std::vector<int>& _initialVector, IDFSNode* _parent = nullptr);

    // Métodos para movimentos do zero no quebra-cabeça
    void moveUp(); // Mover para cima
    void moveDown(); // Mover para baixo
    void moveRight(); // Mover para a direita
    void moveLeft(); // Mover para a esquerda

    // Função para encontrar a posição do zero
    int findZero() const;

    // Método para imprimir o estado do quebra-cabeça
    void printPuzzle() const;
};

// Função para rastrear a solução
int traceSolution(std::vector<IDFSNode*>& solution, IDFSNode* goal);

// Declaração da classe Puzzle para resolver o quebra-cabeça usando IDFS
class IDFSPuzzle {
private:
    std::queue<IDFSNode*> queue; // Fila de nós não explorados
    std::map<std::vector<int>, bool> visited; // Estados visitados
    std::vector<int> finalVector; // Estado final do quebra-cabeça

public:
    int depth; // Profundidade da solução
    int nodesCount; // Número de nós explorados
    
    // Construtor do Puzzle
    IDFSPuzzle(const std::vector<int>& _initialVector, const std::vector<int>& _finalVector);

    // Método para busca IDFS
    int IDFS(const std::vector<int>& initialVector);

    // Função recursiva para busca de profundidade limitada
    int recursive_idfs(int maxDepth, IDFSNode* currentState);
};

// Função para calcular a distância de Manhattan
int manhattan_distance_matrix(int puzzle[3][3]);

int manhattanDistanceIDFS(const std::vector<int>& state, const std::vector<int>& goal);

#endif // IDFS_H
