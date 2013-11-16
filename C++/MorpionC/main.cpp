#include <iostream>
#include <list>

using namespace std;

enum State { Draw=0, Win=1, None=2 };

int DIMENSION = 3;
int WINROW = 3;
short PLAYER_COLOR = 1;
int INFINITE_SCORE = DIMENSION * DIMENSION * 4;

int PlayedActions = 1;

static int STATIC_VALUES[3][3] = 
{
	0, 0, 0, 
	0, 0, 0, 
	0, 0, 0, 
};

int TEST[3][3] = 
{
	0, 0, 1, 
	2, 1, 2, 
	1, 1, 0, 
};

State terminalState(int** board, int x, int y) {
	if(PlayedActions == 0)
		return None;
	if(PlayedActions == DIMENSION*DIMENSION)
		return Draw;

	short color = board[x][y];
	int a,b,row;

	// Vertical
	for(a=x; a >= 0 && board[a][y] == color; --a);
	row = x-a;

	for(a=x; a < DIMENSION && board[a][y] == color; ++a);
	row += a - x - 1;

	if(row >= WINROW)
		return Win;

	// Horizontal
	for(b=y; b >= 0 && board[x][b] == color; --b);
	row = y - b;

	for(b=y; b < DIMENSION && board[x][b] == color; ++b);
	row += row + b - y - 1;

	if(row >= WINROW)
		return Win;

	// Diagonal \

	for(a=x, b=y; a >= 0 && b >= 0 && board[a][b] == color; --b, --a);
	row = x - a;

	for(a=x, b=y; a < DIMENSION && b < DIMENSION && board[a][b] == color; ++b, ++a);
	row += a - x - 1;

	// Diagonal /

	for(a=x, b=y; a >= 0 && b < DIMENSION && board[a][b] == color; ++b, --a);
	row = x - a;

	for(a=x, b=y; a < DIMENSION && b >= 0 && board[a][b] == color; --b, ++a);
	row += a - x - 1;

	if(row >= WINROW)
		return Win;
	else
		return None;
        
}

list<int[2]>* legalMoves(int** board) {
	list<int[2]>* moves = new list<int[2]>();
	for(int i = 0; i < DIMENSION - 1; ++i) {
		for(int j = 0; j < DIMENSION - 1; ++j) {
			if(board[i][j] == 0) {
				int c[] = {i,j};
				moves->push_front(c);
			}
		}
	}
}

void doMove(int** board, int pos[2], short color) {
	board[pos[0]][pos[1]] = color;
	++PlayedActions;
}

void undoMove(int** board, int pos[2]) {
	board[pos[0]][pos[1]] = 0;
	--PlayedActions;
}

int abAlgo(int** board, int depth, int alpha, int beta, short player) {

}

int main() {
	int **board = new int*[DIMENSION];
	for(int x = 0; x < DIMENSION; ++x) {
		board[x] = new int[DIMENSION];
		for(int y = 0; y < DIMENSION; ++y)
			board[x][y] = TEST[x][y];
	}

	cout << "State: " << terminalState(board, 1,1) << endl;
	system("PAUSE");
	return 0;
}