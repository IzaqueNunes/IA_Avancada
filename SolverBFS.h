#ifndef SOLVERBFS_H
#define SOLVERBFS_H
#include "Puzzle.h"
#include <iostream>
#include <vector>

using namespace std;

class SolverBFS {
public:
    Puzzle* puzzle;

public:
    SolverBFS();
    explicit SolverBFS(const std::vector<int>& initialState);
    ~SolverBFS();
    void solve();
};

#endif // SOLVERBFS_H
