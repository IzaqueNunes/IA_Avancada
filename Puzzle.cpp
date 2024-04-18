#include "Puzzle.h"

using namespace std;

Struct state{
	vector<vector<int>> board;
	std::tuple<int,int> blank_pos;

	state copy(){
		state new_state;
		new_state.board = vector.copy(this.board;
	}
};

Struct move{
	std::tuple<int,int> first_position;
	std::tuple<int,int> second_position;
};


State make_move(State S, move m){
	State next_S = copia(S);
	tuple<int,int> last_pos = m.first_position;
	tuple<int,int> next_pos = m.second_position;
	int s = S[next_pos[0]][next_pos[1]];
	next_S[next_pos[0]][next_pos[1]] = 0;
	next_S[last_pos[0]][last_pos[1]] = s;
	
	next_S.blank_pos = m.second_pos;
}

vector<Moves> get_moves(State *s){
	tuple<int, int> blank_pos =s->blank_pos;
	vector<Move> valid_moves;
	if(blank[0]-1>=0){
		Move left = new Move(blank_pos, new tuple<blank_pos[0]-1, blank_pos[1]>)
		valid_moves.push_back(left);
	}else if(...) //check right
	else if(...) //check up
	..
	return valid_moves;
}

bool is_goal(State s){
	
	
}



// Método para imprimir o estado do quebra-cabeça
void Puzzle::printPuzzle() {
    int count = 0;
    for (auto i : Puzzle::puzzle) {
        if (count % 3 == 0)
        	cout << "\n";
            cout << i << ' ';
            count++;
    }
}

// Método para encontrar a posição do espaço vazio (zero)
int Puzzle::findZero() {
    vector<int>::iterator it = find(Puzzle::puzzle.begin(), Puzzle::puzzle.end(), 0);
    return static_cast<int>(distance(Puzzle::puzzle.begin(), it));
}
    
// Métodos para mover o espaço vazio nas quatro direções possíveis
void Puzzle::moveUp() {
    int zPos = findZero();
    vector<int> temp = Puzzle::puzzle;
    if (zPos >= 3) {
        swap(temp[zPos], temp[zPos - 3]);
        Puzzle *child = new Puzzle(temp, this);
        children.push_back(child);
    }
}

void Puzzle::moveDown() {
    int zPos = findZero();
    vector<int> temp = Puzzle::puzzle;
    if (zPos < 6) {
        swap(temp[zPos], temp[zPos + 3]);
        Puzzle *child = new Puzzle(temp, this);
        children.push_back(child);
    }
}

void Puzzle::moveRight() {
    int zPos = findZero();
    vector<int> temp = Puzzle::puzzle;
    if (zPos % 3 < 2) {
        swap(temp[zPos], temp[zPos + 1]);
        Puzzle *child = new Puzzle(temp, this);
        children.push_back(child);
    }
}

void Puzzle::moveLeft() {
    int zPos = findZero();
    vector<int> temp = Puzzle::puzzle;
	if (zPos % 3 > 0) {
        swap(temp[zPos], temp[zPos - 1]);
        Puzzle *child = new Puzzle(temp, this);
        children.push_back(child);
    }
}

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