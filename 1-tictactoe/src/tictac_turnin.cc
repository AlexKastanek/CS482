#include <tictac_support.h>
#include <stdio.h>

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
		(current implementation returns 1 by default, 0 if no move made)
**/

int make_move( int board[][3] )
{
	int state = 0;
	int moveCount = 0;
	int optimalScore = 0;
	int optimalRow = 0, optimalCol = 0;

	// figure out what move it is
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
			state += board[i][j];

	state = -state;
	if( state == 0 )
		state = 1;

	//initialize optimal score with least optimal value
	optimalScore = state * -1;

	// check if game is already in terminal state
	int terminalState = check_terminal_state(board);
	if (terminalState)
	{
		printf("This game has already ended in ");
		switch (terminalState)
		{
			case 1:
				printf("a draw.\n");
				break;
			case 2:
				printf("a victory for player 1\n");
				break;
			case 3:
				printf("a vectory for player 2\n");
				break;
			default:
				printf("\nError: Invalid rvalue from check_terminal_state()\n");
		}
		return 0;
	}

	printf("state: %d\n", state);

	//print the board state
	printf("\nBoard state before move:\n");
	print_board(board);
	printf("\n");

	//check if game can be won this turn
	if (check_winnable_now(board, state))
	{
		//print the board state
		printf("\nBoard state after move:\n");
		print_board(board);
		printf("\n");
		return 1;
	}

	//loop through each available space
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == 0)
			{
				int moveScore;

				//try the move
				board[i][j] = state;

				//evaluate the move using the minimax function
				moveScore = minimax(board, &moveCount, state*-1);
				printf("[%d,%d] move score: %d\n", i, j, moveScore);

				//undo the move
				board[i][j] = 0;

				//update the optimal values if needed
				//(will always update if optimalScore is a loss)
				if ((moveScore > optimalScore && state == 1) ||
					(moveScore < optimalScore && state == -1) ||
					(optimalScore == (state * -1)))
				{
					optimalScore = moveScore;
					optimalRow = i;
					optimalCol = j;
				}
			}
		}
	}

	printf("optimal score: %d\n", optimalScore);

	// use optimal values to make the actual move
	printf( "player [%d] made move after evaluating %d steps: [%d,%d]\n", state, moveCount, optimalRow, optimalCol );
	board[optimalRow][optimalCol] = state;

	//print the board state
	printf("\nBoard state after move:\n");
	print_board(board);
	printf("\n");

	// check if win condition was met
	terminalState = check_terminal_state(board);
	switch (terminalState)
	{
		case 1:
			printf("DRAW\n");
			break;
		case 2:
			printf("PLAYER 1 WINS\n");
			break;
		case 3:
			printf("PLAYER 2 WINS\n");
			break;
		default:
			printf("Ready for next move!\n");
	}

	return moveCount;

	/*
	// default behavior: find any unoccupied square and make the move
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
			// find an empty square
			if( board[i][j] == 0 )
			{
				// that's the move
				printf( "player [%d] made move: [%d,%d]\n", state, i, j );
				board[i][j] = state;
				return 1;
			}
	*/
	
	// no move was made (board was full)
	//return 0;
}

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

int minimax( int board[][3], int* depth, int state )
{
	int optimalMove;
	int terminalState = check_terminal_state(board);

	//check base case - game over condition met
	if (terminalState != 0)
	{
		//initialized to some integer other than 1, -1, and 0
		int returnValue = 10; 

		switch (terminalState)
		{
			case 1:
				//draw
				returnValue = 0;
				break;
			case 2:
				//player 1 wins
				returnValue = 1;
				break;
			case 3:
				//player 2 wins
				returnValue = -1;
				break;
			default:
				printf("Error: Invalid rvalue from check_terminal_state()\n");
		}

		return returnValue;
	}

	//loop through each available space
	if (state == 1)
	{
		//initialize optimal move with least optimal value
		optimalMove = -1;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (board[i][j] == 0)
				{
					//try the move
					board[i][j] = 1;

					//evaluate the move
					*depth = *depth + 1;
					//printf("%d\n", optimalMove);
					optimalMove = max (
						optimalMove,
						minimax(board, depth, -1)
						);

					//undo the move
					board[i][j] = 0;
				}
			}
		}

		return optimalMove;
	}
	else
	{
		//initialize optimal score with least optimal value
		optimalMove = 1;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{	
				if (board[i][j] == 0)
				{
					//try the move
					board[i][j] = -1;

					//evaluate the move
					*depth = *depth + 1;
					//printf("%d\n", optimalMove);
					optimalMove = min (
						optimalMove,
						minimax(board, depth, 1)
						);

					//undo the move
					board[i][j] = 0;
				}
			}
		}

		return optimalMove;
	}
}

bool check_winnable_now( int board[][3], int state )
{
	//try each available space and check if move leads to terminal state
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == 0)
			{
				int terminalState;

				//try the move
				board[i][j] = state;

				//check if move leads to terminal state
				terminalState = check_terminal_state(board);

				//if the game is still going, undo the move
				if (!terminalState)
				{
					board[i][j] = 0;
					//return false;
				}
				//if the game is won, end the game
				else
				{
					printf( "player [%d] made move after observing game could be finished this turn: [%d,%d]\n", state, i, j);
					switch (terminalState)
					{
						case 1:
							printf("DRAW\n");
							break;
						case 2:
							printf("PLAYER 1 WINS\n");
							break;
						case 3:
							printf("PLAYER 2 WINS\n");
							break;
						default:
							printf("Error: Invalid rvalue from check_terminal_state()\n");
					}
					return true;
				}
			}
		}
	}

	return false;
}

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

int check_terminal_state( int board[][3] )
{
	int spacesFilled = 0;

	int rowSums[3] = {0, 0, 0};
	int colSums[3] = {0, 0, 0};
	int diagSums[2] = {0, 0};

	//loop through entire board
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//check if space is filled
			if (board[i][j] == 1 || board[i][j] == -1)
			{
				spacesFilled++;

				//determine score by adding current space value to sum arrays
				rowSums[i] += board[i][j];
				colSums[j] += board[i][j];
				if (i == j)
				{
					diagSums[0] += board[i][j];
				}
				if ((i + j) == 2)
				{
					diagSums[1] += board[i][j];
				}
			}
		}
	}

	//if any of the sums are equal to 3 or -3, return result
	//(2 for player 1 & 3 for player 2)
	for (int i = 0; i < 3; i++)
	{
		//check diagonal sum (only for first 2 iterations)
		if (i < 2)
		{
			if (diagSums[i] == 3)
			{
				return 2;
			}
			else if (diagSums[i] == -3)
			{
				return 3;
			}
		}

		//check row sum
		if (rowSums[i] == 3)
		{
			return 2;
		}
		else if (rowSums[i] == -3)
		{
			return 3;
		}

		//check col sum
		if (colSums[i] == 3)
		{
			return 2;
		}
		else if (colSums[i] == -3)
		{
			return 3;
		}
	}

	//If no player has won but all spaces are filled, draw.
	//otherwise, the game is not in a terminal state
	if (spacesFilled == 9)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//returns the max of two values a and b
int max( int a, int b )
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

//returns the min of two values a and b
int min( int a, int b )
{
	if (a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

//prints the board
void print_board( int board[][3] )
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d ", board[i][j]);
		}
		printf("\n");
	}
}
