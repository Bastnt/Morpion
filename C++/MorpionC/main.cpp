#include "main.h"

int PlayedActions = 0;

short opponent(short player) {
	return (player == 1 ? 2 : 1);
}

State terminalState(int** board, Move move) {
	if(PlayedActions == 0 || move.x == -1)
		return None;

	int x = move.x;
	int y = move.y;
	
	

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
	else if(PlayedActions == DIMENSION*DIMENSION)
		return Draw;
	else
		return None;
        
}

int staticEvaluation(int** board) {
	int total = 0;
	for(int i = 0; i < DIMENSION; ++i) {
		for(int j = 0; j < DIMENSION; ++j) {
			total += STATIC_VALUES[i][j];
		}
	}
	return total;
}


list<Move>* legalMoves(int** board) {
	list<Move>* moves = new list<Move>();
	for(int i = 0; i < DIMENSION; ++i) {
		for(int j = 0; j < DIMENSION; ++j) {
			if(board[i][j] == 0) {
				Move c = {i,j};
				moves->push_front(c);
			}
		}
	}
	return moves;
}

void doMove(int** board, Move pos, short color) {
	board[pos.x][pos.y] = color;
	++PlayedActions;
}

void undoMove(int** board, Move pos) {
	board[pos.x][pos.y] = 0;
	--PlayedActions;
}

int abAlgo(int** board, list<Move>* actions) {
	Move m = {-1,-1};
	return abMax(board, -INFINITE_SCORE, INFINITE_SCORE, actions, m);
}

int abMax(int** board, int alpha, int beta, list<Move>* actions, Move move) {
	switch(terminalState(board, move)) {
	case Draw:
		return 0;
		break;
	case Win:
		return -INFINITE_SCORE;
		break;
	}

	list<Move>* children = legalMoves(board);

	int v = -INFINITE_SCORE;
	int w;

	for(list<Move>::iterator it = children->begin(); it != children->end(); ++it) {
		list<Move>* tmp = new list<Move>();

		doMove(board, *it, PLAYER_COLOR);
		display(board);
		w = abMin(board, alpha, beta, tmp, *it);
		undoMove(board, *it);

		if(w > v) {
			v = w;
			actions = tmp;
			actions->push_front(*it);
		}

		if(v >= beta)
			return v;

		alpha = max(alpha, v);
	}
	return v;
}

int abMin(int** board, int alpha, int beta, list<Move>* actions, Move move) {
	switch(terminalState(board, move)) {
	case Draw:
		return 0;
		break;
	case Win:
		return INFINITE_SCORE;
		break;
	}

	list<Move>* children = legalMoves(board);

	int v = INFINITE_SCORE;
	int w;

	for(list<Move>::iterator it = children->begin(); it != children->end(); ++it) {
		list<Move>* tmp = new list<Move>();
		doMove(board, *it, opponent(PLAYER_COLOR));
		cout << endl;
		w = abMax(board, alpha, beta, tmp, *it);
		undoMove(board, *it);

		if(w < v) {
			v = w;
			actions = tmp;
			actions->push_front(*it);
		}

		if(v <= alpha)
			return v;

		beta = min(beta, v);
	}
	return v;
}

int main() {
	int **board = new int*[DIMENSION];
	for(int x = 0; x < DIMENSION; ++x) {
		board[x] = new int[DIMENSION];
		for(int y = 0; y < DIMENSION; ++y) {
			if(TEST[x][y] != 0)
				++PlayedActions;
			board[x][y] = TEST[x][y];
		}
	}

	list<Move>* actions = new list<Move>();

	cout << "PlayedActions: " << PlayedActions << endl;
	cout << "alphabeta: " << abAlgo(board, actions) << endl;
	cout << "moves: " << "[";
	for(list<Move>::iterator it = actions->begin(); it != actions->end(); ++it) {
		cout << "(" << it->x << ", " << it->y << ") ";
	}
	cout << "]" << endl;
	
	system("PAUSE");
	return 0;
}