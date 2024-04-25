#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <fstream>

using namespace std;

string vectorToString(const vector<int>& vec) {
    stringstream ss;
    for (const auto& val : vec) {
        if (&val != &vec.front()) {
            ss << ","; // Adiciona vírgula como separador
        }
        ss << val;
    }
    return ss.str();
}

// Retorna o índice do elemento, ou -1 se não encontrado
int findElement(const vector<int>& vec, int element) {
    auto it = find(vec.begin(), vec.end(), element);
    if (it == vec.end()) {
        return -1; // Retorna -1 se o elemento não for encontrado
    }
    return distance(vec.begin(), it); // Retorna a posição do elemento
}

class Node {
public:
    vector<int> state; // Estado do quebra-cabeça
    Node* parent;
    int insertionOrder;

    Node(const vector<int>& _state, Node* _parent, int _insertionOrder) 
        : state(_state), parent(_parent), insertionOrder(_insertionOrder) {}

    // Encontra a posição do zero no vetor de estado
    int findZero() const {
        return findElement(state, 0); // Usa a função segura para encontrar zero
    }

    // Métodos de movimento do zero
    void moveUp(vector<Node*>& children, int order) {
        int zPos = findZero();
        if (zPos >= 4) {
            vector<int> temp = state;
            swap(temp[zPos], temp[zPos - 4]);
            children.push_back(new Node(temp, this, order));
        }
    }

    void moveDown(vector<Node*>& children, int order) {
        int zPos = findZero();
        if (zPos < 12) {
            vector<int> temp = state;
            swap(temp[zPos], temp[zPos + 4]);
            children.push_back(new Node(temp, this, order));
        }
    }

    void moveLeft(vector<Node*>& children, int order) {
        int zPos = findZero();
        if (zPos % 4 > 0) {
            vector<int> temp = state;
            swap(temp[zPos], temp[zPos - 1]);
            children.push_back(new Node(temp, this, order));
        }
    }

    void moveRight(vector<Node*>& children, int order) {
        int zPos = findZero();
        if (zPos % 4 < 3) {
            vector<int> temp = state;
            swap(temp[zPos], temp[zPos + 1]);
            children.push_back(new Node(temp, this, order));
        }
    }
};

// Estrutura para armazenar dados do algoritmo A*
class AstarNode {
public:
    Node* node;
    int gCost;
    int hCost;
    int fCost;


    AstarNode(Node* _node, int _gCost, int _hCost, int _fCost) 
        : node(_node), gCost(_gCost), hCost(_hCost), fCost(_fCost) {}
};

// Comparator para priorizar nós com menor fCost
struct AstarComparator {
    bool operator()(const AstarNode& a, const AstarNode& b) const {
        if (a.fCost == b.fCost) {
            if (a.hCost == b.hCost) {
                return a.node->insertionOrder < b.node->insertionOrder; // LIFO para empates
            } else {
                return a.hCost > b.hCost; // Prioridade pelo menor hCost
            }
        }
        return a.fCost > b.fCost; // Prioridade pelo menor fCost
    }
};

// Calcula a distância de Manhattan para a heurística
int manhattanDistance(const vector<int>& state, const vector<int>& goal) {
    int distance = 0;
    int dimension = static_cast<int>(sqrt(state.size())); // Calcula a dimensão do quebra-cabeça
    for (size_t i = 0; i < state.size(); ++i) {
        if (state[i] != 0) {
            int goalPos = findElement(goal, state[i]); // Usa a função segura para encontrar posição
            if (goalPos != -1) {
                int rowDist = abs(static_cast<int>(i / dimension) - static_cast<int>(goalPos / dimension));
                int colDist = abs(static_cast<int>(i % dimension) - static_cast<int>(goalPos % dimension));
                distance += rowDist + colDist; // Soma as distâncias das linhas e colunas
            }
        }
    }
    return distance;
}

// Função para reconstruir o caminho da solução
vector<Node*> tracePath(Node* node) {
    vector<Node*> path;
    while (node) {
        path.push_back(node);
        node = node->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

// Algoritmo A* para o 15-puzzle
vector<Node*> Astar(const vector<int>& initialState, const vector<int>& finalState) {
	auto start_time = chrono::high_resolution_clock::now();
    priority_queue<AstarNode, vector<AstarNode>, AstarComparator> openList;
    unordered_set<string> closedSet;
    int insertionOrder = 0;
    int nodesCount = 0; // Contador de nós expandidos
    float heuristicSum = 0; // Soma dos valores heurísticos
    int startHeuristic = 0; // Valor heurístico do estado inicial

    Node* startNode = new Node(initialState, nullptr, insertionOrder++);
    int startGCost = 0;
    int startHCost = manhattanDistance(initialState, finalState);
    int startFCost = startGCost + startHCost;
	startHeuristic = startHCost;
    openList.push({startNode, startGCost, startHCost, startFCost}); // Adiciona ao openList com comparador AstarComparator

    while (!openList.empty()) {
        AstarNode currentAstarNode = openList.top();
        openList.pop();

        Node* currentNode = currentAstarNode.node;
        string stateStr = vectorToString(currentNode->state);
        nodesCount++; // Incrementar o número de nós expandidos
        heuristicSum += currentAstarNode.hCost; // Soma dos valores heurísticos

        if (closedSet.count(stateStr) > 0) {
            continue; // Se estado já está no closedSet, ignora
        }

        closedSet.insert(stateStr); // Adiciona ao closedSet

        if (currentNode->state == finalState) {
        	auto end_time = chrono::high_resolution_clock::now();
    	    auto duration_sec = chrono::duration<double>(end_time - start_time).count();
        	cout << nodesCount << "," << tracePath(currentNode).size() - 1<< "," << duration_sec << "," << heuristicSum / nodesCount << "," << startHCost << endl;
            return tracePath(currentNode); // Retorna a solução se encontrou o final
        }

        // Gera os filhos
        vector<Node*> children;
        currentNode->moveUp(children, insertionOrder);
        currentNode->moveLeft(children, insertionOrder);
        currentNode->moveRight(children, insertionOrder);
        currentNode->moveDown(children, insertionOrder);

        for (Node* child : children) {
            string childStateStr = vectorToString(child->state);

            if (closedSet.count(childStateStr) > 0) {
                continue; // Se estado já está no closedSet, ignora
            }

            int gCost = currentAstarNode.gCost + 1;
            int hCost = manhattanDistance(child->state, finalState);
            int fCost = gCost + hCost;

            openList.push({child, gCost, hCost, fCost}); // Adiciona o nó filho na fila de prioridade
        }
    }

    return {}; // Retorna vazio se não encontrou solução
}

int main() {
	
		
	 ifstream inputFile("input/15puzzle_instances.txt"); // Abre o arquivo de entrada

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
    vector<int> finalState = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; // Estado final desejado

    for (int i = 0; i < initialStates.size(); ++i) {
    
	    vector<int> initialState = initialStates[i];
		
	    auto solutionPath = Astar(initialState, finalState);
	}

    return 0;
}