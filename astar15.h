#ifndef ASTAR15_H
#define ASTAR15_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <cstdint>

uint64_t encodeState(const std::vector<int>& vec);
std::vector<int> decodeState(uint64_t state);

class A15Node {
public:
    uint64_t state;
    A15Node* parent;
    int insertionOrder;
    std::vector<A15Node*> children;

    A15Node(uint64_t _state, A15Node* _parent, int _insertionOrder);
    ~A15Node();

    int findZero() const;
    void printPuzzle() const;
    void moveUp(int order);
    void moveDown(int order);
    void moveLeft(int order);
    void moveRight(int order);
};

class AstarNode15 {
public:
    A15Node* node;
    int gCost;
    int hCost;
    int fCost;

    AstarNode15(A15Node* _node, int _gCost, int _hCost, int _fCost);
};

struct AstarComparator15 {
    bool operator()(const AstarNode15& a, const AstarNode15& b) const;
};

std::vector<A15Node*> tracePath(A15Node* node);
int manhattanDistanceA15(uint64_t state, uint64_t goal);

class A15Puzzle {
private:
    uint64_t finalState;
    std::unordered_map<uint64_t, A15Node*> allNodes;

public:
    int nodesCount = 0;
    float heuristicSum = 0;
    int startHeuristic = 0;

    A15Puzzle(const std::vector<int>& _finalVector);
    ~A15Puzzle();

    std::vector<A15Node*> Astar15(const std::vector<int>& initialVector);
};

#endif // ASTAR15_H
