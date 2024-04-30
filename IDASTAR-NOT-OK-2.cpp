#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <algorithm>
#include <cmath>

using namespace std;

// Função de hash para vetor de inteiros
struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        size_t hash = 0;
        for (int i : v) {
            hash ^= std::hash<int>{}(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

// Definição da classe Node para representar os estados do quebra-cabeça
class Node {
public:
    vector<int> state;
    Node* parent;
    int g_cost;
    int h_cost;
    int f_cost;

    Node(vector<int> _state, Node* _parent)
        : state(_state), parent(_parent), g_cost(0), h_cost(0), f_cost(0) {}

    int findZero() {
        for (int i = 0; i < state.size(); ++i) {
            if (state[i] == 0) {
                return i;
            }
        }
        return -1; // Se não encontrar
    }

    Node* moveUp() {
        int zero_pos = findZero();
        if (zero_pos >= 3) { // Pode mover para cima
            vector<int> new_state = state;
            swap(new_state[zero_pos], new_state[zero_pos - 3]);
            return new Node(new_state, this);
        }
        return nullptr;
    }

    Node* moveLeft() {
        int zero_pos = findZero();
        if (zero_pos % 3 > 0) { // Pode mover para a esquerda
            vector<int> new_state = state;
            swap(new_state[zero_pos], new_state[zero_pos - 1]);
            return new Node(new_state, this);
        }
        return nullptr;
    }

    Node* moveRight() {
        int zero_pos = findZero();
        if (zero_pos % 3 < 2) { // Pode mover para a direita
            vector<int> new_state = state;
            swap(new_state[zero_pos], new_state[zero_pos + 1]);
            return new Node(new_state, this);
        }
        return nullptr;
    }

    Node* moveDown() {
        int zero_pos = findZero();
        if (zero_pos < 6) { // Pode mover para baixo
            vector<int> new_state = state;
            swap(new_state[zero_pos], new_state[zero_pos + 3]);
            return new Node(new_state, this);
        }
        return nullptr;
    }
};

// Calcular a distância Manhattan (heurística)
int manhattanDistance(const vector<int>& state, const vector<int>& goal) {
    int distance = 0;
    for (int i = 0; i < state.size(); ++i) {
        if (state[i] != 0) {
            int goal_pos = find(goal.begin(), goal.end(), state[i]) - goal.begin();
            int row_distance = abs(i / 3 - goal_pos / 3);
            int col_distance = abs(i % 3 - goal_pos % 3);
            distance += row_distance + col_distance;
        }
    }
    return distance;
}

// Classe para o algoritmo IDA*
class IDAStar {
public:
    vector<int> goal_state;
    int nodes_expanded;
    int nodes_generated;
    int heuristic_sum;
    int start_heuristic;

    IDAStar(const vector<int>& _goal_state)
        : goal_state(_goal_state), nodes_expanded(0), heuristic_sum(0), start_heuristic(0), nodes_generated(0) {}

    // Função para rastrear o caminho do nó final ao início
    vector<Node*> tracePath(Node* node) {
        vector<Node*> path;
        while (node) {
            path.push_back(node);
            node = node->parent;
        }
        reverse(path.begin(), path.end()); // Inverter para obter a ordem correta
        return path;
    }

    // Função recursiva para busca de profundidade limitada
    pair<int, vector<Node*>> depthLimitedSearch(Node* node, int g_cost, int limit, unordered_set<vector<int>, VectorHash>& visited) {
        heuristic_sum += node->h_cost;
        nodes_generated++;
		nodes_expanded++;
        int h_cost = manhattanDistance(node->state, goal_state);
        int f_cost = g_cost + h_cost;

        node->g_cost = g_cost;
        node->h_cost = h_cost;
        node->f_cost = f_cost;

        if (f_cost > limit) {
            return {f_cost, {}};  // Se f_cost ultrapassar o limite, retorna o próximo limite
        }

        if (node->state == goal_state) {
            return {INT_MAX, tracePath(node)};  // Solução encontrada
        }

        //nodes_expanded++;

        // Gerar filhos na ordem especificada
        vector<Node*> children = {
            node->moveUp(),
            node->moveLeft(),
            node->moveRight(),
            node->moveDown(),
        };

        // Remover filhos nulos e evitar ciclos
        children.erase(remove(children.begin(), children.end(), nullptr), children.end());

        int next_limit = INT_MAX;

        for (Node* child : children) {
            if (visited.find(child->state) != visited.end()) {
                nodes_expanded--;
            }

            // Adicionar o filho ao conjunto de estados visitados
            visited.insert(child->state);

            auto result = depthLimitedSearch(child, g_cost + 1, limit, visited);

            if (!result.second.empty()) {  // Se uma solução foi encontrada
                return result;
            }

            next_limit = min(next_limit, result.first);

            delete child;  // Limpeza de memória para evitar vazamentos
        }

        return {next_limit, {}};  // Retornar o próximo limite
    }

    // Implementação do IDA* com gerenciamento de ciclos
    vector<Node*> idaStar(const vector<int>& initial_state) {
        start_heuristic = manhattanDistance(initial_state, goal_state);
        nodes_expanded = 0;
        heuristic_sum = start_heuristic;

        Node* initial_node = new Node(initial_state, nullptr);
        initial_node->h_cost = start_heuristic;

        int limit = start_heuristic;  // Limite inicial
        vector<Node*> solution_path;

        unordered_set<vector<int>, VectorHash> visited;  // Conjunto para rastrear estados visitados

        while (solution_path.empty()) {  // Repetir até encontrar uma solução
            visited.insert(initial_node->state);  // Adicionar estado inicial ao conjunto de visitados
			
            auto result = depthLimitedSearch(initial_node, 0, limit, visited);
			nodes_expanded--;
            if (!result.second.empty()) {  // Se uma solução foi encontrada
            	
                solution_path = result.second;
            } else {
            	
                limit = result.first;  // Atualizar o limite para a próxima iteração
            }
        }

        return solution_path;  // Retornar o caminho para a solução
    }
};

// Teste do IDA* com quebra-cabeças 8-puzzle
void testIDAStar(const vector<vector<int>>& puzzle_instances, const vector<int>& goal_state) {
    IDAStar idaStar(goal_state);

    for (const auto& instance : puzzle_instances) {
        auto start_time = chrono::high_resolution_clock::now();

        idaStar.heuristic_sum = manhattanDistance(instance, goal_state);
        auto solution_path = idaStar.idaStar(instance);

        auto end_time = chrono::high_resolution_clock::now();
        double duration = chrono::duration<double>(end_time - start_time).count();

        int solution_length = solution_path.size() - 1;
        double heuristic_avg = (double)idaStar.heuristic_sum / idaStar.nodes_generated;

        cout << idaStar.nodes_expanded << ", ";
        cout << solution_length << ", ";
        cout << duration << ", ";
        cout << heuristic_avg << ", ";
        cout << idaStar.start_heuristic << endl;
    }
}

// Exemplo de teste para 8-puzzle
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

    vector<int> goal_state = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    testIDAStar(initialStates, goal_state);

    return 0;
}
