#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <chrono>
#include <map>
#include <unordered_set>

using namespace std;

enum moviment {
	cima = 0,
	baixo = 1,
	esquerda = 2, 
	direita = 3,
	null_move = 4
};

// Classe Node representa um nó no espaço de busca do quebra-cabeça
class Node {
public:
    vector<Node *> children; // Filhos do nó
    vector<int> initialVector; // Vetor representando o estado do quebra-cabeça
    Node *parent; // Ponteiro para o nó pai
    int g; // Profundidade do nó
    int h; // Valor da heurística
	int id_nodo;
	moviment generator;
	
    // Construtor da classe Node
    Node(vector<int> _initialVector, Node *_parent, int _g, int _h, int _id_nodo, moviment _generator) {
        initialVector = _initialVector;
        parent = _parent;
        g = _g;
        h = _h;
        id_nodo = _id_nodo;
        generator = _generator;
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
    void moveUp(int _id_nodo) {
        int zPos = findZero();
        vector<int> temp = initialVector;

        if (zPos >= 3 && this->generator != baixo) {
            swap(temp[zPos], temp[zPos - 3]);
            Node *child = new Node(temp, this, g + 1, h, _id_nodo, cima);
            children.push_back(child);
        }
    }

    void moveDown(int _id_nodo ) {
        int zPos = findZero();
        vector<int> temp = initialVector;

        if (zPos < 6 && this->generator != cima) {
            swap(temp[zPos], temp[zPos + 3]);
            Node *child = new Node(temp, this, g + 1, h, _id_nodo, baixo);
            children.push_back(child);
        }
    }

    void moveRight(int _id_nodo) {
        int zPos = findZero();
        vector<int> temp = initialVector;

        if (zPos % 3 < 2 && this->generator != esquerda) {
            swap(temp[zPos], temp[zPos + 1]);
            Node *child = new Node(temp, this, g + 1, h, _id_nodo, direita);
            children.push_back(child);
        }
    }

    void moveLeft(int _id_nodo) {
        int zPos = findZero();
        vector<int> temp = initialVector;

        if (zPos % 3 > 0 && this->generator != direita) {
            swap(temp[zPos], temp[zPos - 1]);
            Node *child = new Node(temp, this, g + 1, h, _id_nodo, esquerda);
            children.push_back(child);
        }
    }
};

// Comparador para a fila de prioridade
struct CompareNodes {
    bool operator()(Node* a, Node* b) const{
        int f1 = a->h;
        int f2 = b->h;

        if (f1 != f2) {
            return f1 > f2;
        } else if (a->g != b->g) {
            return a->g < b->g;
        } 
		return (a->id_nodo > b->id_nodo); 
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
    }
    return depth;
}

// Função para calcular a distância de Manhattan de uma matriz
int manhattan_distance(const vector<int>& puzzle) {
    int sum = 0;
    int size = puzzle.size();

    for (int i = 0; i < size; i++) {
        if (puzzle[i] == 0) {
            continue; // Ignora o espaço vazio.
        }

        // Encontra a posição correta do número.
        int row_goal = puzzle[i] / size;
        int col_goal = puzzle[i] % size;

        // Calcula a distância de Manhattan.
        int row_diff = abs(i / size - row_goal);
        int col_diff = abs(i % size - col_goal);
        sum += row_diff + col_diff;
    }

    return sum;
}


// Classe que representa o quebra-cabeça e resolve usando BFS
class Puzzle {
private:
    priority_queue<Node *, vector<Node *>, CompareNodes> queue; // Fila para nós ainda não explorados (open)
    map<vector<int>, bool> visited; // Mapa para estados já visitados (closed)
    vector<int> finalVector; // Estado final do quebra-cabeça
	
public:
	int depth;
    int nodesCount;
    long long int heuristic_sum;
	int count_generated_nodes;
	
    // Construtor da classe Puzzle
    Puzzle(vector<int> _initialVector, vector<int> _finalVector) {
    	int h = manhattan_distance(_initialVector);
        Node *initialNode = new Node(_initialVector, nullptr, 0, h, 0, null_move);
        this->queue.push(initialNode);
		
		this->heuristic_sum = h;
        this->finalVector = _finalVector;
        this->nodesCount = 0;
		this->depth = 0;

    }

    void GreedyBFS() {
    	
    	this->count_generated_nodes = 1;
    	
    	vector<Node *> solution;
		int id_nodo = 1;
		
	    while (!queue.empty()) {
	        Node *currentNode = this->queue.top();
	        this->queue.pop();
	        
	        // verificando se o filho já foi visitado
                bool alreadyVisited = false;

                if (visited.count(currentNode->initialVector) > 0) {
                    alreadyVisited = true;
                }

                // se o filho já foi visitado, ignora e continua
                if (alreadyVisited) {
                    continue;
                }
                
                this->visited[currentNode->initialVector] = true;
                
                
	        	
	        if (currentNode->initialVector == this->finalVector) {
	            this->depth = traceSolution(solution, currentNode);
	            return;
	        }
	
	        // Contabiliza apenas os nós explorados (não conta os filhos)
	        this->nodesCount++;
	        
	        currentNode->moveUp(id_nodo);
	        id_nodo++;
	        currentNode->moveLeft(id_nodo);
	        id_nodo++;
	        currentNode->moveRight(id_nodo);
	        id_nodo++;
	        currentNode->moveDown(id_nodo);
	        id_nodo++;
	        
	        
	
	        for (auto currentChild : currentNode->children) {              
                int childHeuristic = manhattan_distance(currentChild->initialVector);
                currentChild->h = childHeuristic;
                this->heuristic_sum += childHeuristic;
                // cout << "AQUI:" << this->heuristic_sum << endl;
                this->count_generated_nodes++;
                

                this->queue.push(currentChild);
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
    ifstream inputFile("input/teste.txt"); // Abre o arquivo de entrada

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

        puzzle.GreedyBFS();
        
        auto end_time = chrono::high_resolution_clock::now();
        auto duration_sec = chrono::duration<double>(end_time - start_time).count();


        int puzzleArray[3][3] = {{initialStates[i][0], initialStates[i][1], initialStates[i][2]},
                                  {initialStates[i][3], initialStates[i][4], initialStates[i][5]},
                                  {initialStates[i][6], initialStates[i][7], initialStates[i][8]}};
        int distanceManhattan = manhattan_distance_matrix(puzzleArray);

        cout << puzzle.nodesCount << ", " << puzzle.depth - 1 << ", " << duration_sec << ", " << double(puzzle.heuristic_sum)/double(puzzle.count_generated_nodes) << ", ";
        cout << distanceManhattan << endl;
    }

    return 0;
}