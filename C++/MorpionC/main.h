#include <iostream>
#include <list>

using namespace std;

static int STATIC_VALUES[3][3] = 
{
	2, 3, 2, 
	3, 4, 3, 
	2, 3, 2, 
};

static int TEST[3][3] = 
{
	1, 2, 1, 
	2, 0, 2, 
	1, 2, 1, 
};

enum State { Draw=0, Win=1, None=2 };

struct Move {
	int x;
	int y;
};

static int DIMENSION = 3;
static int WINROW = 3;
static short PLAYER_COLOR = 1;
static int INFINITE_SCORE = DIMENSION * DIMENSION * 4;

short opponent(short player);
State terminalState(int** board, Move);
int staticEvaluation(int** board);
list<Move>* legalMoves(int** board);
void doMove(int** board, Move, short color);
void undoMove(int** board, Move);
int abAlgo(int** board, list<Move>* actions);
int abMax(int** board, int alpha, int beta, list<Move>* actions, Move);
int abMin(int** board, int alpha, int beta, list<Move>* actions, Move);

void display(int** board) {
	for(int x = 0; x < DIMENSION; ++x) {
		for(int y = 0; y < DIMENSION; ++y)
			cout << board[x][y] << " ";
		cout << endl;
	}
}

void displayL(list<Move>& l) {
	cout << endl << "Move : ";
	for(list<Move>::iterator it = l.begin(); it != l.end(); ++it) {
			Move m = *it;
			cout << "(" << m.x << ", " << m.y << ") ";
	}
}