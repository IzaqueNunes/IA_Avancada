#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <string>
#include <chrono>

using namespace std;

// Classe para representar um nó do quebra-cabeça
class Node {
public:
    vector<int> initialVector;
    Node* parent;
    vector<Node*> children;
    int insertionOrder;

    Node(vector<int> _initialVector, Node* _parent, int _insertionOrder) {
        initialVector = _initialVector;
        parent = _parent;
        insertionOrder = _insertionOrder;
    }

    int findZero() {
        auto it = find(initialVector.begin(), initialVector.end(), 0);
        return static_cast<int>(distance(initialVector.begin(), it));
    }

    void printPuzzle() {
        int count = 0;
        for (auto i : initialVector) {
            if (count % 3 == 0) {
                cout << "\n";
            }
            cout << i << ' ';
            count++;
        }
        cout << "\n";
    }

    void moveUp(int insertionCounter) {
        int zPos = findZero();
        if (zPos >= 3) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos - 3]);
            children.push_back(new Node(temp, this, insertionCounter));
        }
    }

    void moveDown(int insertionCounter) {
        int zPos = findZero();
        if (zPos < 6) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos + 3]);
            children.push_back(new Node(temp, this, insertionCounter));
        }
    }

    void moveLeft(int insertionCounter) {
        int zPos = findZero();
        if (zPos % 3 > 0) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos - 1]);
            children.push_back(new Node(temp, this, insertionCounter));
        }
    }

    void moveRight(int insertionCounter) {
        int zPos = findZero();
        if (zPos % 3 < 2) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos + 1]);
            children.push_back(new Node(temp, this, insertionCounter));
        }
    }
};

// Estrutura para o GBFS
class GBFSNode {
public:
    Node* node;
    int gCost;
    int hCost;

    GBFSNode(Node* _node, int _gCost, int _hCost) {
        node = _node;
        gCost = _gCost;
        hCost = _hCost;
    }
};

// Comparator para a fila de prioridade do GBFS
struct GBFSComparator {
    bool operator()(const GBFSNode& a, const GBFSNode& b) const {
        if (a.hCost == b.hCost) {
            if (a.gCost == b.gCost) {
                return a.node->insertionOrder < b.node->insertionOrder; // LIFO para empates
            }
            return a.gCost < b.gCost; // Prioridade para o maior gCost
        }
        return a.hCost > b.hCost; // Prioridade para o menor hCost
    }
};

// Função para calcular a distância Manhattan
int manhattanDistance(const vector<int>& state, const vector<int>& goal) {
    int distance = 0;
    for (size_t i = 0; i < state.size(); ++i) {
        if (state[i] != 0) {
            int goalPos = find(goal.begin(), goal.end(), state[i]) - goal.begin();
            int rowDistance = abs(static_cast<int>((i / 3)) - static_cast<int>((goalPos / 3)));
            int colDistance = abs(static_cast<int>((i % 3)) - static_cast<int>((goalPos % 3)));
            distance += rowDistance + colDistance;
        }
    }
    return distance;
}

// Rastreia o caminho do nó até a raiz
vector<Node*> tracePath(Node* node) {
    vector<Node*> path;
    while (node) {
        path.push_back(node);
        node = node->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

class Puzzle {
private:
    vector<int> finalVector;

public:
    int nodesCount;
    double heuristicSum;
    int startHeuristic;

    Puzzle(vector<int> _finalVector) {
        finalVector = _finalVector;
        nodesCount = 0;
        heuristicSum = 0;
        startHeuristic = 0;
    }

    vector<Node*> GBFS(const vector<int>& initialVector) {
        priority_queue<GBFSNode, vector<GBFSNode>, GBFSComparator> openList;
        unordered_set<string> closedSet;

        Node* startNode = new Node(initialVector, nullptr, 1);
        int insertionCounter = 1;

        int startGCost = 0;
        int startHCost = manhattanDistance(initialVector, finalVector);
		startHeuristic = startHCost;
        openList.push({startNode, startGCost, startHCost});

        while (!openList.empty()) {
            GBFSNode currentGBFSNode = openList.top();
            openList.pop();

            Node* currentNode = currentGBFSNode.node;

            string stateStr;
            for (int i : currentNode->initialVector) {
                stateStr += to_string(i) + ",";
            }

            if (closedSet.count(stateStr) > 0) {
                continue;
            }

            closedSet.insert(stateStr);

            if (currentNode->initialVector == finalVector) {
                return tracePath(currentNode);
            }

            insertionCounter++;
			// Gerar filhos
            nodesCount++; // Incrementar o número de nós expandidos
            heuristicSum += currentGBFSNode.hCost; // Soma dos valores heurísticos
            currentNode->moveUp(insertionCounter++);
            currentNode->moveLeft(insertionCounter++);
            currentNode->moveRight(insertionCounter++);
            currentNode->moveDown(insertionCounter++);

            for (Node* child : currentNode->children) {
                string childStateStr;
                for (int i : child->initialVector) {
                    childStateStr += to_string(i) + ",";
                }

                if (closedSet.count(childStateStr) > 0) {
                    continue;
                }

                int hCost = manhattanDistance(child->initialVector, finalVector);
                int gCost = currentGBFSNode.gCost + 1;

                openList.push({child, gCost, hCost});
            }
        }

        return {}; // Se nenhum caminho for encontrado
    }
};

int main() {
    ifstream inputFile("input/8puzzle_instances.txt");

    if (!inputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return 1;
    }

    string line;
    vector<vector<int>> initialStates;

    while (getline(inputFile, line)) {
        istringstream iss(line);
        vector<int> initialState;
        int num;

        while (iss >> num) {
            initialState.push_back(num);
        }

        initialStates.push_back(initialState);
    }

    inputFile.close();

    vector<int> finalState = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    for (auto initialState : initialStates) {
        Puzzle puzzle(finalState);
	
		using namespace chrono;
    
    	auto start_time = chrono::high_resolution_clock::now();

        auto solutionPath = puzzle.GBFS(initialState);

        auto end_time = chrono::high_resolution_clock::now();
    	auto duration_sec = chrono::duration<double>(end_time - start_time).count();

        if (!solutionPath.empty()) {
            cout <<puzzle.nodesCount << "," << solutionPath.size() - 1 <<"," << duration_sec << "," << puzzle.heuristicSum/puzzle.nodesCount <<  "," << puzzle.startHeuristic <<endl;
        } else {
            cout << "Nenhuma solução encontrada." << endl;
        }
    }

    return 0;
}
