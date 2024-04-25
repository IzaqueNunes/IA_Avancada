#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <chrono>
#include <unordered_set>
#include <cmath>
#include <string>

using namespace std;

class Node {
public:
    vector<int> initialVector;
    Node* parent;
    int insertionOrder;
    vector<Node*> children;

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
            if (count % 4 == 0)
                cout << "\n";

            cout << i << ' ';
            count++;
        }
        cout << "\n";
    }

    void moveUp(int order) {
        int zPos = findZero();
        if (zPos >= 4) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos - 4]);
            Node* child = new Node(temp, this, order);
            children.push_back(child);
        }
    }

    void moveDown(int order) {
        int zPos = findZero();
        if (zPos < 12) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos + 4]);
            Node* child = new Node(temp, this, order);
            children.push_back(child);
        }
    }

    void moveLeft(int order) {
        int zPos = findZero();
        if (zPos % 4 > 0) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos - 1]);
            Node* child = new Node(temp, this, order);
            children.push_back(child);
        }
    }

    void moveRight(int order) {
        int zPos = findZero();
        if (zPos % 4 < 3) {
            vector<int> temp = initialVector;
            swap(temp[zPos], temp[zPos + 1]);
            Node* child = new Node(temp, this, order);
            children.push_back(child);
        }
    }
};

class AstarNode {
public:
    Node* node;
    int gCost;
    int hCost;
    int fCost;

    AstarNode(Node* _node, int _gCost, int _hCost, int _fCost) {
        node = _node;
        gCost = _gCost;
        hCost = _hCost;
        fCost = _fCost;
    }
};

struct AstarComparator {
    bool operator()(const AstarNode& a, const AstarNode& b) const {
        if (a.fCost == b.fCost) {
            if (a.hCost == b.hCost) {
                return a.node->insertionOrder < b.node->insertionOrder;
            } else {
                return a.hCost > b.hCost;
            }
        }
        return a.fCost > b.fCost;
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
            int rowDistance = abs(static_cast<int>(i / 4) - static_cast<int>(goalPos / 4));
            int colDistance = abs(static_cast<int>(i % 4) - static_cast<int>(goalPos % 4));
            distance += rowDistance + colDistance;
        }
    }
    return distance;
}

class Puzzle {
private:
    vector<int> finalVector;

public:
    int nodesCount = 0;
    float heuristicSum = 0;
    int startHeuristic = 0;

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
		startHeuristic = startHCost;
        openList.push({startNode, startGCost, startHCost, startFCost});

        while (!openList.empty()) {
            AstarNode currentAstarNode = openList.top();
            openList.pop();

            Node* currentNode = currentAstarNode.node;

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
			// Gerar filhos
            nodesCount++; // Incrementar o número de nós expandidos
            heuristicSum += currentAstarNode.hCost; // Soma dos valores heurísticos
            
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

                openList.push({child, gCost, hCost, fCost});
            }
        }

        return {};
    }
};

int main() {
    ifstream inputFile("input/15puzzle_instances.txt");

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

    vector<int> finalState = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    for (const auto& initialState : initialStates) {
        Puzzle puzzle(finalState);

        auto start_time = chrono::high_resolution_clock::now();

        auto solutionPath = puzzle.Astar(initialState);

        auto end_time = chrono::high_resolution_clock::now();
        auto duration_sec = chrono::duration<double>(end_time - start_time).count();

        if (!solutionPath.empty()) {
            cout << puzzle.nodesCount << "," << solutionPath.size() - 1 << "," << duration_sec << "," << puzzle.heuristicSum / puzzle.nodesCount << "," << puzzle.startHeuristic << endl;
        } else {
            cout << "Nenhuma solução encontrada." << endl;
        }
    }

    return 0;
}
