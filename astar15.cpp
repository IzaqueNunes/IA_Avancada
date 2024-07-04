#include "astar15.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <string>
#include <functional>

// Classe para representar o estado do nó como string para o conjunto fechado
struct HashVector {
    size_t operator()(const std::vector<int>& vec) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : vec) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

A15Node::A15Node(std::vector<int> _initialVector, A15Node* _parent, int _insertionOrder) {
    initialVector = _initialVector;
    parent = _parent;
    insertionOrder = _insertionOrder;
}

int A15Node::findZero() const {
    auto it = std::find(initialVector.begin(), initialVector.end(), 0);
    return static_cast<int>(std::distance(initialVector.begin(), it));
}

void A15Node::printPuzzle() const {
    int count = 0;
    for (auto i : initialVector) {
        if (count % 4 == 0) {
            std::cout << "\n";
        }
        std::cout << i << ' ';
        count++;
    }
    std::cout << "\n";
}

void A15Node::moveUp(int order) {
    int zPos = findZero();
    if (zPos >= 4) {
        std::vector<int> temp = initialVector;
        std::swap(temp[zPos], temp[zPos - 4]);
        A15Node* child = new A15Node(temp, this, order);
        children.push_back(child);
    }
}

void A15Node::moveDown(int order) {
    int zPos = findZero();
    if (zPos < 12) {
        std::vector<int> temp = initialVector;
        std::swap(temp[zPos], temp[zPos + 4]);
        A15Node* child = new A15Node(temp, this, order);
        children.push_back(child);
    }
}

void A15Node::moveLeft(int order) {
    int zPos = findZero();
    if (zPos % 4 > 0) {
        std::vector<int> temp = initialVector;
        std::swap(temp[zPos], temp[zPos - 1]);
        A15Node* child = new A15Node(temp, this, order);
        children.push_back(child);
    }
}

void A15Node::moveRight(int order) {
    int zPos = findZero();
    if (zPos % 4 < 3) {
        std::vector<int> temp = initialVector;
        std::swap(temp[zPos], temp[zPos + 1]);
        A15Node* child = new A15Node(temp, this, order);
        children.push_back(child);
    }
}

AstarNode15::AstarNode15(A15Node* _node, int _gCost, int _hCost, int _fCost)
    : node(_node), gCost(_gCost), hCost(_hCost), fCost(_fCost) {}

bool AstarComparator15::operator()(const AstarNode15& a, const AstarNode15& b) const {
    if (a.fCost == b.fCost) {
        if (a.hCost == b.hCost) {
            return a.node->insertionOrder < b.node->insertionOrder;
        } else {
            return a.hCost > b.hCost;
        }
    }
    return a.fCost > b.fCost;
}

std::vector<A15Node*> tracePath(A15Node* node) {
    std::vector<A15Node*> path;
    while (node) {
        path.push_back(node);
        node = node->parent;
    }
    std::reverse(path.begin(), path.end()); 
    return path;
}

int manhattanDistanceA15(const std::vector<int>& state, const std::vector<int>& goal) {
    int distance = 0;
    for (size_t i = 0; i < state.size(); ++i) {
        if (state[i] != 0) {
            int goalPos = std::find(goal.begin(), goal.end(), state[i]) - goal.begin();
            int rowDistance = std::abs(static_cast<int>(i / 4) - static_cast<int>(goalPos / 4));
            int colDistance = std::abs(static_cast<int>(i % 4) - static_cast<int>(goalPos % 4));
            distance += rowDistance + colDistance;
        }
    }
    return distance;
}

A15Puzzle::A15Puzzle(std::vector<int> _finalVector) : finalVector(_finalVector) {}

std::vector<A15Node*> A15Puzzle::Astar15(const std::vector<int>& initialVector) {
    std::priority_queue<AstarNode15, std::vector<AstarNode15>, AstarComparator15> openList;
    std::unordered_set<std::vector<int>, HashVector> closedSet;
    int insertionCounter = 0;

    A15Node* startNode = new A15Node(initialVector, nullptr, insertionCounter++);
    int startGCost = 0;
    int startHCost = manhattanDistanceA15(initialVector, finalVector);
    int startFCost = startGCost + startHCost;
    startHeuristic = startHCost;

    openList.push({startNode, startGCost, startHCost, startFCost});

    while (!openList.empty()) {
        AstarNode15 currentAstarNode = openList.top();
        openList.pop();

        A15Node* currentNode = currentAstarNode.node;    

        // Verifica se o estado atual já foi visitado
        if (closedSet.count(currentNode->initialVector) > 0) {
            continue;
        }

        // Marca o estado atual como visitado
        closedSet.insert(currentNode->initialVector);

        if (currentNode->initialVector == finalVector) {
            return tracePath(currentNode);
        }

        nodesCount++; // Incrementa o número de nós expandidos
        heuristicSum += currentAstarNode.hCost; // Soma das heurísticas

        currentNode->moveUp(insertionCounter++);
        currentNode->moveLeft(insertionCounter++);
        currentNode->moveRight(insertionCounter++);
        currentNode->moveDown(insertionCounter++);

        for (A15Node* child : currentNode->children) {
            // Verifica se o estado do nó filho já foi visitado
            if (closedSet.count(child->initialVector) > 0) {
                continue;
            }

            int gCost = currentAstarNode.gCost + 1;
            int hCost = manhattanDistanceA15(child->initialVector, finalVector);
            int fCost = gCost + hCost;

            openList.push({child, gCost, hCost, fCost});
        }
        //delete currentNode;
    }

    return {}; // Nenhuma solução encontrada
}
