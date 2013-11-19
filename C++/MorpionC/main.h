#include <iostream>
#include <list>

#define DIMENSION 3
#define WINROW 3
#define PLAYER_COLOR 1
#define INFINITE_SCORE DIMENSION * DIMENSION * 4

enum State { Draw=0, Win=1, None=2 };

static int TEST[DIMENSION][DIMENSION] = 
{
	2, 0, 0, 
	0, 1, 0, 
	0, 2, 1, 
};

struct Move {
	int x;
	int y;
};

using namespace std;

short opponent(short player);
State terminalState(int** board, Move);
int staticEvaluation(int** board);
list<Move>* legalMoves(int** board);
void doMove(int** board, Move, short color);
void undoMove(int** board, Move);
int abAlgo(int** board, list<Move>& actions, int depth);
int abMax(int** board, int alpha, int beta, list<Move>& actions, Move, int depth);
int abMin(int** board, int alpha, int beta, list<Move>& actions, Move, int depth);

void display(int** board) {
	cout << "======" <<endl;
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