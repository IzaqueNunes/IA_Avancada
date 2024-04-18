#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <chrono>
#include <map>

using namespace std;

// Classe Node representa um nó no espaço de busca do quebra-cabeça
class Node {
public:
    vector<Node *> children; // Filhos do nó
    vector<int> initialVector; // Vetor representando o estado do quebra-cabeça
    Node *parent; // Ponteiro para o nó pai

    // Construtor da classe Node
    Node(vector<int> _initialVector, Node *_parent) {
        initialVector = _initialVector;
        parent = _parent;
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

    // Método para encontrar a posição do espaço vazio (zero)
    int findZero() {
        vector<int>::iterator it = find(initialVector.begin(), initialVector.end(), 0);
        return static_cast<int>(distance(initialVector.begin(), it));
    }

    // Métodos para mover o espaço vazio nas quatro direções possíveis
    void moveUp() {
        int zPos = findZero();
        vector<int> temp = initialVector;

        if (zPos >= 3) {
            swap(temp[zPos], temp[zPos - 3]);
            Node *child = new Node(temp, this);
            children.push_back(child);
        }
    }

    void moveDown() {
        int zPos = findZero();
        vector<int> temp = initialVector;

        if (zPos < 6) {
            swap(temp[zPos], temp[zPos + 3]);
            Node *child = new Node(temp, this);
            children.push_back(child);
        }
    }

    void moveRight() {
        int zPos = findZero();
        vector<int> temp = initialVector;

        if (zPos % 3 < 2) {
            swap(temp[zPos], temp[zPos + 1]);
            Node *child = new Node(temp, this);
            children.push_back(child);
        }
    }

    void moveLeft() {
        int zPos = findZero();
        vector<int> temp = initialVector;

        if (zPos % 3 > 0) {
            swap(temp[zPos], temp[zPos - 1]);
            Node *child = new Node(temp, this);
            children.push_back(child);
        }
    }
};

// Função para rastrear a solução a partir do nó final
int traceSolution(vector<Node *> sol, Node *g) {
    Node *curr = g;
    sol.push_back(g);

    while (curr->parent != nullptr) {
        curr = curr->parent;
        sol.push_back(curr);
    }

    reverse(sol.begin(), sol.end());

    int depth = 0;
    for (auto i : sol) {
        depth += 1;
        // i->printPuzzle();
        // cout << "\n";
    }
    return depth;
}

// Classe que representa o quebra-cabeça e resolve usando BFS
class Puzzle {
private:
    queue<Node *> queue; // Fila para nós ainda não explorados (open)
    map<vector<int>, bool> visited; // Mapa para estados já visitados (closed)
    vector<int> finalVector; // Estado final do quebra-cabeça

public:
    int depth; // Profundidade da solução
    int nodesCount; // Contagem de nós explorados

    // Construtor da classe Puzzle
    Puzzle(vector<int> _initialVector, vector<int> _finalVector) {
        Node *initialNode = new Node(_initialVector, nullptr);
        this->queue.push(initialNode);

        this->finalVector = _finalVector;
        this->nodesCount = 0;

        this->visited[initialNode->initialVector] = true;
    }

    // Método para resolver o quebra-cabeça usando BFS
    void BFS() {
        vector<Node *> solution;

        while (!queue.empty()) {
            Node *currentNode = this->queue.front();
            this->queue.pop();

            currentNode->moveUp();
            currentNode->moveLeft();
            currentNode->moveRight();
            currentNode->moveDown();

            this->nodesCount++;

            for (auto currentChild : currentNode->children) {
                if (currentChild->initialVector == this->finalVector) {
                    this->depth = traceSolution(solution, currentChild);
                    return;
                }

                if (visited.count(currentChild->initialVector) == 0) {
                    this->visited[currentChild->initialVector] = true;
                    this->queue.push(currentChild);
                }
            }
        }
    }
    
    
};

// Função para calcular a distância de Manhattan de uma matriz
int manhattan_distance_matrix(int puzzle[][3]) {
    int sum = 0;
    int n = 3;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int num = puzzle[i][j];
            if (num == 0) {
                continue; // Ignora o espaço vazio.
            }

            // Encontra a posição correta do número.
            int row_goal = num / n;
            int col_goal = num % n;

            // Calcula a distância de Manhattan.
            int row_diff = abs(i - row_goal);
            int col_diff = abs(j - col_goal);
            sum += row_diff + col_diff;
        }
    }

    return sum;
}

int main() {
    ifstream inputFile("input/8puzzle_instances.txt"); // Abre o arquivo de entrada

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

    for (int i = 0; i < initialStates.size(); ++i) {
        auto start_time = chrono::high_resolution_clock::now();

        Puzzle puzzle(initialStates[i], {0, 1, 2, 3, 4, 5, 6, 7, 8});
        puzzle.BFS();

        auto end_time = chrono::high_resolution_clock::now();
        auto duration_sec = chrono::duration<double>(end_time - start_time).count();

        cout << puzzle.nodesCount << ", " << puzzle.depth - 1 << ", " << duration_sec << ", " << "0, ";

        // Calcular e imprimir a heurística (distância de Manhattan)
        int puzzleArray[3][3] = {{initialStates[i][0], initialStates[i][1], initialStates[i][2]},
                                  {initialStates[i][3], initialStates[i][4], initialStates[i][5]},
                                  {initialStates[i][6], initialStates[i][7], initialStates[i][8]}};
        int distanceManhattan = manhattan_distance_matrix(puzzleArray);
        cout << distanceManhattan << endl;
    }

    return 0;
}