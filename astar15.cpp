#include "astar15.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <string>
#include <functional>

uint64_t encodeState(const std::vector<int>& vec) {
    uint64_t state = 0;
    for (size_t i = 0; i < vec.size(); ++i) {
        state |= (static_cast<uint64_t>(vec[i]) << (4 * i));
    }
    return state;
}

std::vector<int> decodeState(uint64_t state) {
    std::vector<int> vec(16);
    for (size_t i = 0; i < 16; ++i) {
        vec[i] = (state >> (4 * i)) & 0xF;
    }
    return vec;
}

A15Node::A15Node(uint64_t _state, A15Node* _parent, int _insertionOrder) 
    : state(_state), parent(_parent), insertionOrder(_insertionOrder) {}

A15Node::~A15Node() {
    for (A15Node* child : children) {
        delete child;
    }
}

int A15Node::findZero() const {
    std::vector<int> vec = decodeState(state);
    auto it = std::find(vec.begin(), vec.end(), 0);
    return static_cast<int>(std::distance(vec.begin(), it));
}

void A15Node::printPuzzle() const {
    std::vector<int> vec = decodeState(state);
    int count = 0;
    for (auto i : vec) {
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
        std::vector<int> vec = decodeState(state);
        std::swap(vec[zPos], vec[zPos - 4]);
        A15Node* child = new A15Node(encodeState(vec), this, order);
        children.push_back(child);
    }
}

void A15Node::moveDown(int order) {
    int zPos = findZero();
    if (zPos < 12) {
        std::vector<int> vec = decodeState(state);
        std::swap(vec[zPos], vec[zPos + 4]);
        A15Node* child = new A15Node(encodeState(vec), this, order);
        children.push_back(child);
    }
}

void A15Node::moveLeft(int order) {
    int zPos = findZero();
    if (zPos % 4 > 0) {
        std::vector<int> vec = decodeState(state);
        std::swap(vec[zPos], vec[zPos - 1]);
        A15Node* child = new A15Node(encodeState(vec), this, order);
        children.push_back(child);
    }
}

void A15Node::moveRight(int order) {
    int zPos = findZero();
    if (zPos % 4 < 3) {
        std::vector<int> vec = decodeState(state);
        std::swap(vec[zPos], vec[zPos + 1]);
        A15Node* child = new A15Node(encodeState(vec), this, order);
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

int manhattanDistanceA15(uint64_t state, uint64_t goal) {
    std::vector<int> stateVec = decodeState(state);
    std::vector<int> goalVec = decodeState(goal);
    int distance = 0;
    for (size_t i = 0; i < stateVec.size(); ++i) {
        if (stateVec[i] != 0) {
            int goalPos = std::find(goalVec.begin(), goalVec.end(), stateVec[i]) - goalVec.begin();
            int rowDistance = std::abs(static_cast<int>(i / 4) - static_cast<int>(goalPos / 4));
            int colDistance = std::abs(static_cast<int>(i % 4) - static_cast<int>(goalPos % 4));
            distance += rowDistance + colDistance;
        }
    }
    return distance;
}

A15Puzzle::A15Puzzle(const std::vector<int>& _finalVector) : finalState(encodeState(_finalVector)) {}

A15Puzzle::~A15Puzzle() {
    for (auto& pair : allNodes) {
        delete pair.second;
    }
}

std::vector<A15Node*> A15Puzzle::Astar15(const std::vector<int>& initialVector) {
    std::priority_queue<AstarNode15, std::vector<AstarNode15>, AstarComparator15> openList;
    std::unordered_set<uint64_t> closedSet;
    int insertionCounter = 0;

    A15Node* startNode = new A15Node(encodeState(initialVector), nullptr, insertionCounter++);
    int startGCost = 0;
    int startHCost = manhattanDistanceA15(startNode->state, finalState);
    int startFCost = startGCost + startHCost;
    startHeuristic = startHCost;

    openList.push({startNode, startGCost, startHCost, startFCost});

    while (!openList.empty()) {
        AstarNode15 currentAstarNode = openList.top();
        openList.pop();

        A15Node* currentNode = currentAstarNode.node;    

        if (closedSet.count(currentNode->state) > 0) {
            continue;
        }

        closedSet.insert(currentNode->state);

        if (currentNode->state == finalState) {
            std::vector<A15Node*> path = tracePath(currentNode);
            for (auto node : path) {
                delete node;
            }
            return path;
        }

        nodesCount++;
        heuristicSum += currentAstarNode.hCost;

        currentNode->moveUp(insertionCounter++);
        currentNode->moveLeft(insertionCounter++);
        currentNode->moveRight(insertionCounter++);
        currentNode->moveDown(insertionCounter++);

        for (A15Node* child : currentNode->children) {
            if (closedSet.count(child->state) > 0) {
                delete child;
                continue;
            }
            
			allNodes[child->state] = child;
            int gCost = currentAstarNode.gCost + 1;
            int hCost = manhattanDistanceA15(child->state, finalState);
            int fCost = gCost + hCost;

            openList.push({child, gCost, hCost, fCost});
            
        }
        
        currentNode->children.clear();
    }

    return {}; // Nenhuma solução encontrada
}
