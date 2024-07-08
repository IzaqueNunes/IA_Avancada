#include "idfs.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <chrono>
#include <vector>

// Construtor da classe Node
IDFSNode::IDFSNode(const std::vector<int>& _initialVector, IDFSNode* _parent)
    : initialVector(_initialVector), parent(_parent) {}

// Método para encontrar a posição do zero
int IDFSNode::findZero() const {
    auto it = std::find(initialVector.begin(), initialVector.end(), 0);
    return static_cast<int>(std::distance(initialVector.begin(), it));
}

// Métodos para mover o zero no quebra-cabeça
void IDFSNode::moveUp() {
    int zPos = findZero();
    if (zPos >= 3) { // Pode mover para cima
        std::vector<int> temp = initialVector;
        std::swap(temp[zPos], temp[zPos - 3]);
        IDFSNode* child = new IDFSNode(temp, this);
        children.push_back(child);
    }
}

void IDFSNode::moveDown() {
    int zPos = findZero();
    if (zPos < 6) { // Pode mover para baixo
        std::vector<int> temp = initialVector;
        std::swap(temp[zPos], temp[zPos + 3]);
        IDFSNode* child = new IDFSNode(temp, this);
        children.push_back(child);
    }
}

void IDFSNode::moveRight() {
    int zPos = findZero();
    if (zPos % 3 < 2) { // Pode mover para a direita
        std::vector<int> temp = initialVector;
        std::swap(temp[zPos], temp[zPos + 1]);
        IDFSNode* child = new IDFSNode(temp, this);
        children.push_back(child);
    }
}

void IDFSNode::moveLeft() {
    int zPos = findZero();
    if (zPos % 3 > 0) { // Pode mover para a esquerda
        std::vector<int> temp = initialVector;
        std::swap(temp[zPos], temp[zPos - 1]);
        IDFSNode* child = new IDFSNode(temp, this);
        children.push_back(child);
    }
}

// Método para imprimir o estado do quebra-cabeça
void IDFSNode::printPuzzle() const {
    int count = 0;
    for (auto i : initialVector) {
        if (count % 3 == 0)
            std::cout << "\n";
        std::cout << i << ' ';
        count++;
    }
}

// Função para rastrear a solução
int traceSolution(std::vector<IDFSNode*>& solution, IDFSNode* goal) {
    IDFSNode* curr = goal;
    solution.push_back(goal);

    while (curr->parent != nullptr) {
        curr = curr->parent;
        solution.push_back(curr);
    }

    std::reverse(solution.begin(), solution.end());

    int depth = 0;
    for (auto i : solution) {
        depth++;
    }

    return depth;
}

// Construtor da classe Puzzle
IDFSPuzzle::IDFSPuzzle(const std::vector<int>& _initialVector, const std::vector<int>& _finalVector)
    : finalVector(_finalVector), nodesCount(0), depth(0) {
    IDFSNode* initialNode = new IDFSNode(_initialVector, nullptr);
    queue.push(initialNode);
    visited[_initialVector] = true;
}

// Implementação do IDFS
int IDFSPuzzle::IDFS(const std::vector<int>& initialVector) {
    int objective = -1;
    depth = 0;

    IDFSNode* initialNode = new IDFSNode(initialVector, nullptr);

    while (true) {
        objective = recursive_idfs(depth, initialNode);

        if (objective != -1) {
            break;
        }

        depth++; // Aumenta a profundidade permitida
    }

    delete initialNode; // Limpa a memória do nó inicial
    
    return objective; // Retorna a profundidade onde encontrou a solução
}

// Implementação do método recursivo para IDFS
int IDFSPuzzle::recursive_idfs(int maxDepth, IDFSNode* currentState) {
    std::vector<IDFSNode *> solution;

		// Verificando se o estado atual é Goal
    if (currentState->initialVector == this->finalVector) {
        this->depth = traceSolution(solution, currentState);
        return this->depth;
    }
        
        // Verificando profundidade
        if (maxDepth > 0) {
		// Gerando filhos do estado atual
		    currentState->moveUp();
            currentState->moveLeft();
            currentState->moveRight();
            currentState->moveDown();
            
        	this->nodesCount++; 
			  
            // iterando filhos
			for (IDFSNode* child : currentState->children) {
				// evitando ciclos verificando o pai
				if(currentState->parent != nullptr && child->initialVector == currentState->parent->initialVector){
					continue;	
				}
				
				// verificando se o filho já foi visitado
				bool alreadyVisited = false;
				
				for (IDFSNode* visitedNode: solution){
					if (visitedNode->initialVector == child->initialVector){
						bool alreadyVisited = true;
						break;
					}
				}
				
				// se o filho já foi visitado, ignora e continua
				if(alreadyVisited){
					continue;
				}
                
                // realizando a busca em profundidade recursiva
                int objective = recursive_idfs(maxDepth -1, child);
                
                //retornando objetivo se o mesmo foi encontrado
                if(objective != -1){
                	return objective;
				}
				
				// limpando memória alocada para o filho
				delete child;                    
            }
			currentState->children.clear();            
        }
        return -1;
	}  	    
	

// Função para calcular a distância de Manhattan de uma matriz 3x3
// Calcula a distância de Manhattan entre um estado atual e um estado objetivo
int manhattanDistanceIDFS(const std::vector<int>& state, const std::vector<int>& goal) {
    int distance = 0; // Inicializa a distância em zero
    // Percorre cada elemento do vetor de estado para calcular a distância de Manhattan
    for (size_t i = 0; i < state.size(); ++i) {
        if (state[i] != 0) { // Ignora o espaço vazio
            // Determina a posição correspondente no vetor objetivo
            int goalPos = static_cast<int>(std::distance(goal.begin(), std::find(goal.begin(), goal.end(), state[i])));
            // Calcula a diferença de linhas
            int rowDistance = std::abs(static_cast<int>(i / 3) - (goalPos / 3));
            // Calcula a diferença de colunas
            int colDistance = std::abs(static_cast<int>((i % 3) - (goalPos % 3)));
            distance += rowDistance + colDistance; // Soma as distâncias de linha e coluna
        }
    }
    return distance; // Retorna a distância total calculada
}

int manhattan_distance_matrix(int puzzle[3][3]) {
    int sum = 0;
    int n = 3;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int num = puzzle[i][j];
            if (num == 0) {
                continue; // Ignora o espaço vazio
            }

            int row_goal = num / n;
            int col_goal = num % n;

            int row_diff = std::abs(i - row_goal);
            int col_diff = std::abs(j - col_goal);
            sum += row_diff + col_diff;
        }
    }

    return sum;
}
