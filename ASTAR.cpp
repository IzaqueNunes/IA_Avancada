#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <chrono>
#include <map>
#include <unordered_set>
#include <cmath>
#include <string>


using namespace std;

class Node {
public:
    vector<int> initialVector;
    Node* parent; // pai deste nó
    int insertionOrder;
    vector<Node*> children; // adicione isto para corrigir o erro

    Node(vector<int> _initialVector, Node* _parent, int _insertionOrder) {
        initialVector = _initialVector;
        parent = _parent;
        insertionOrder = _insertionOrder;
    }

    int findZero() {
        auto it = find(initialVector.begin(), initialVector.end(), 0);
        return static_cast<int>(distance(initialVector.begin(), it));
    }
    // Método para imprimir o estado do quebra-cabeça
    void printPuzzle() {
        int count = 0;
        for (auto i : initialVector) {
            if (count % 3 == 0)
                cout << "\n";

            cout << i << ' ';
            count++;
        }                       
    }
    void moveUp(int order) {
        int zPos = findZero();
        if (zPos >= 3) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos - 3]);
            Node* child = new Node(temp, this, order);
            children.push_back(child); // adicione ao vetor de filhos
        }
    }

    void moveDown(int order) {
        int zPos = findZero();
        if (zPos < 6) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos + 3]);
            Node* child = new Node(temp, this, order);
            children.push_back(child); // adicione ao vetor de filhos
        }
    }

    void moveLeft(int order) {
        int zPos = findZero();
        if (zPos % 3 > 0) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos - 1]);
            Node* child = new Node(temp, this, order);
            children.push_back(child); // adicione ao vetor de filhos
        }
    }

    void moveRight(int order) {
        int zPos = findZero();
        if (zPos % 3 < 2) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos + 1]);
            Node* child = new Node(temp, this, order);
            children.push_back(child); // adicione ao vetor de filhos
        }
    }
};



class AstarNode {
public:
    Node* node;
    int gCost;
    int hCost; // Armazenar o custo heurístico
    int fCost;

    AstarNode(Node* _node, int _gCost, int _hCost, int _fCost) {
        node = _node;
        gCost = _gCost;
        hCost = _hCost; // Armazenar o custo heurístico
        fCost = _fCost;
    }
};

struct AstarComparator {
    bool operator()(const AstarNode& a, const AstarNode& b) const {
		if (a.fCost == b.fCost) {
			if(a.hCost == b.hCost){        		
        		return a.node->insertionOrder < b.node->insertionOrder; // LIFO para empates entre valores de f e h
			}else{
				return a.hCost > b.hCost; // Prioridade pelo menor hCost
			}
        }
        return a.fCost > b.fCost; // Prioridade pelo menor fCost
   	}
};

vector<Node*> tracePath(Node* node) {
    vector<Node*> path;
    while (node) {
        path.push_back(node);
        node = node->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

int manhattanDistance(const vector<int>& state, const vector<int>& goal) {
    int distance = 0;
    for (size_t i = 0; i < state.size(); ++i) {
        if (state[i] != 0) {
            int goalPos = find(goal.begin(), goal.end(), state[i]) - goal.begin();
            int rowDistance = abs((int)(i / 3) - (int)(goalPos / 3));
            int colDistance = abs((int)(i % 3) - (int)(goalPos % 3));
            distance += rowDistance + colDistance;
        }
    }
    return distance;
}

class Puzzle {
private:
    vector<int> finalVector;

public:
    int nodesCount = 0; // Contador de nós expandidos
    float heuristicSum = 0; // Soma dos valores heurísticos
    int startHeuristic = 0; // Valor heurístico do estado inicial

    Puzzle(vector<int> _finalVector) {
        finalVector = _finalVector;
    }

    vector<Node*> Astar(const vector<int>& initialVector) {

        priority_queue<AstarNode, vector<AstarNode>, AstarComparator> openList;
        unordered_set<string> closedSet;
        int insertionCounter = 0;

        Node* startNode = new Node(initialVector, nullptr, insertionCounter++);
        int startGCost = 0;
        int startHCost = manhattanDistance(initialVector, finalVector);
        int startFCost = startGCost + startHCost;

        openList.push({ startNode, startGCost, startHCost, startFCost });
        startHeuristic = startHCost; // Armazenar o valor heurístico do estado inicial

        while (!openList.empty()) {
            AstarNode currentAstarNode = openList.top();
            openList.pop();

            Node* currentNode = currentAstarNode.node;

            nodesCount++; // Incrementar o número de nós expandidos
            heuristicSum += currentAstarNode.hCost; // Soma dos valores heurísticos

            string stateStr;
            for (int i : currentNode->initialVector) {
                stateStr += to_string(i) + ",";
            }

            if (closedSet.count(stateStr) > 0) {
                continue;
            }

            closedSet.insert(stateStr); // Adicionar ao conjunto fechado

            if (currentNode->initialVector == finalVector) {
                auto solutionPath = tracePath(currentNode);                                
                return solutionPath;
            }

            // Gerar filhos
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

                int gCost = currentAstarNode.gCost + 1;
                int hCost = manhattanDistance(child->initialVector, finalVector);
                int fCost = gCost + hCost;

                openList.push({ child, gCost, hCost, fCost });
            }
        }

        return {}; // Se nenhum caminho foi encontrado
    }
};


int main() {
    vector<int> initialState = {0, 6, 1, 7, 4, 2, 3, 8, 5};
    //vector<int> initialState = {5, 0, 2, 6, 4, 8, 1, 7, 3};
    //vector<int> initialState = {2, 4, 7, 0, 3, 6, 8, 1, 5};


    vector<int> finalState = {0, 1, 2,3, 4, 5, 6, 7, 8};

    Puzzle puzzle(finalState);
    
    
    using namespace chrono;
    
    auto start_time = chrono::high_resolution_clock::now();


    auto solutionPath = puzzle.Astar(initialState);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration_sec = chrono::duration<double>(end_time - start_time).count();
    // Se uma solução foi encontrada
    if (!solutionPath.empty()) {
        cout <<puzzle.nodesCount << "," << solutionPath.size() - 1 <<"," << duration_sec << "," << puzzle.heuristicSum/puzzle.nodesCount <<  "," << puzzle.startHeuristic <<endl;
    }

    if (solutionPath.empty()) {
        
        cout << "Nenhuma solução encontrada." << endl;
    }

    return 0;
}