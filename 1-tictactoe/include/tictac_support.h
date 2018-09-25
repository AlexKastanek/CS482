#ifndef _TICTAC_SUPPORT_H_
#define _TICTAC_SUPPORT_H_

#include <string>

/**
	load_board: loads a tic-tac-toe board state from a file

	args:
		std::string filename: file to read the board from
		int [][3] board: 3x3 array of ints representing the board state
			0: empty
			1: x
		 -1: o

	returns (int):
		0: if file loaded OK
		-1: if file doesn't exist

**/

int load_board( std::string filename, int board[][3] );


/**
	save_board: saves a tic-tac-toe board state to a file

	args:
		std::string filename: file to write the board to
		int [][3] board: 3x3 array of ints representing the board state
			0: empty
			1: x
		 -1: o

	returns (int):
		0: if file opened OK
		-1: if file can't be opened

**/

int save_board( std::string filename, int board[][3] );


/**
	make_move: takes a board state and makes a legal 
	(hopefully optimal) move

	args:
		int [][3] board: 3x3 array of ints representing the 
		board state. The values of board are altered based
		on the move
			0: empty
			1: x
		 -1: o
		
	returns (int):
		the number of steps it took to choose the best move
		(returns 1 by default, 0 if no move made)
**/

int make_move( int board[][3] );

/**
	minimax: takes a board state and recursively calls itself
	to evaluate what is the optimal move assuming the other
	player is also playing optimally

	args:
		int [][3] board: 3x3 array of ints representing the 
		board state. The values of board are altered based
		on the move
			0: empty
			1: x
		   -1: o

		depth: an int passed by reference used to keep track
		how many steps have been checked. Increments every time
		minimax is called.

		state: an int (1 or -1) used to track whose turn it is.
		This could also be calculated every call to minimax, but
		this seems a bit more efficient.
		
	returns (int):
		The optimal score of the board state
**/

int minimax( int board[][3], int* depth, int state );

/**
	check_terminal_state: takes a board state and checks if it
	is in the terminal state or game over condition by find the
	sum of each row, column, and diagonal. It then checks to see
	if any of those sums are 3 or -3. If 3, player 1 wins, and if
	-3, player 2 wins. Also checks for draws (the board is full
	and no one has one). If no game over condition was met,
	returns 0.

	args:
		int [][3] board: 3x3 array of ints representing the 
		board state. The values of board are altered based
		on the move
			0: empty
			1: x
		   -1: o
		
	returns (int):
		0: not in a terminal state
		1: draw
		2: player 1 wins
		3: player 2 wins
**/

int check_terminal_state( int board[][3] );

//returns the max of two values a and b
int max( int a, int b );

//returns the min of two values a and b
int min( int a, int b );

#endif