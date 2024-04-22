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
    explicit SolverBFS(const std::vector<std::vector<int>>& initialState); // Alterado para vetor 2D
    ~SolverBFS();
    void solve();
};

#endif // SOLVERBFS_H
