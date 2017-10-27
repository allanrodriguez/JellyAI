// file: Jelly.cpp
// description: main program for Jelly


#include <iostream>
#include "Vision.hpp"
#include "AI.hpp"
#include "robotArm.hpp"

using namespace JellyAI;

char getFile(int j);

/* returns true if phyical board matches virtual board, else returns false */
bool verifyBoard(Board * board, Vision vision);

int getPieceType(int pieceType);

int main(int argc, char *argv[])
{
	bool notDone = true;
	int move[2][2];
	int jellysMove[3][2];
	Board * board = new Board();
	bool isPlayersTurn = false;
	PieceColor JellysColor = Black;

	memset(move, 0, sizeof(int) * 4);

	/* initialize */
	printf("Starting Jelly...\n");
	Vision eyes;
	
	if(eyes.isJellyFirst())
	{
		isPlayersTurn = false;
		JellysColor = White;
	}
	else
	{
		isPlayersTurn = true;
		JellysColor = Black;
	}

	/* keep SINGING */
	while(notDone)
	{
		if(board->isCheckmate(White) || board->isCheckmate(Black) || board->isStalemate(White) || board->isStalemate(Black))
		{
			printf("Do you want to play again? Enter 1 for yes or 0 for no: ");
			std::cin >> notDone;
			if(!notDone)
			{
				return 0;
			}
			
			delete board;
			
			printf("Please set up the chessboard again. Press ENTER when finished.\n");
			std::cin.ignore();
			
			/* initialize */
			board = new Board();
			
			printf("\nStarting Jelly...\n");
			Vision eyes;
			
			if(eyes.isJellyFirst())
			{
				isPlayersTurn = false;
				JellysColor = White;
			}
			else
			{
				isPlayersTurn = true;
				JellysColor = Black;
			}
		}
		
		printf("\n");
		if(isPlayersTurn)
		{
			printf("Player's turn.\n");
			printf("Move piece and enter 1 to continue or 0 to quit: ");
			std::cin >> notDone;
			if(!notDone)
			{
				break;
			}
			
			/* Look for the player's move */
			printf("Looking for move...\n");
			if(eyes.getMove(move))
			{
				printf("Move found.\n");
				// print move
				std::cout << getFile(move[0][1]) << move[0][0] + 1 << " -> " << getFile(move[1][1]) << move[1][0] + 1 << std::endl;
				
				if(board->movePiece(move[0][0], move[0][1], move[1][0], move[1][1]))
				{
					while(!verifyBoard(board, eyes))
					{
						board->print();
						printf("Please fix board to match above board.\nEnter 1 to continue or 0 to quit: ");
						std::cin >> notDone;
						if(!notDone)
						{
							break;
						}
						eyes.updateCurrentBoard();
					}
					eyes.copyCurrentBoard();
					
					isPlayersTurn = !isPlayersTurn;
				}
				else
				{
					printf("Please move piece(s) back.\n");
					/*TODO (maybe): verify pieces were moved back  */
				}
			}
			else /* move not found */
			{
				board->print();
				printf("Unable to find move. Enter 1 to try again or 0 to quit: ");
				std::cin >> notDone;
				std::cout << std::endl;
			}
		}
		else 
		{
			printf("Jelly's turn.\n");
			printf("Thinking of move...");
			AI ai(board, JellysColor);
			ai.jellysMove();
			
			/*****************TODO: Mario's move();**********************/
			ai.getMove(jellysMove);
			int startRank = jellysMove[0][0] + 1;
			int startFile = jellysMove[0][1] + 1;
			int endRank = jellysMove[1][0] + 1;
			int endFile = jellysMove[1][1] + 1;
			int moveType = jellysMove[2][0];
			int pieceType = jellysMove[2][1];
			
			if(JellysColor == White)
			{
				/* rotate the rank and file */
				startRank = 7 - jellysMove[0][0] + 1;
				startFile = 7 - jellysMove[0][1] + 1;
				endRank = 7 - jellysMove[1][0] + 1;
				endFile = 7 - jellysMove[1][1] + 1;
			}
			
			switch(moveType) /* type of move */
			{
				case 0: /* move piece to empty square */
					movePiece(startFile, startRank, endFile, endRank, pieceType);
					break;
					     
				case 1: /* capture then move */
					movePiece(endFile, endRank, 9, 9, getPieceType(board->getPreviousBoard()->getPiece(endRank-1, endFile-1)->getType() ) );
					movePiece(startFile, startRank, endFile, endRank, pieceType);
					break;
					
				case 2: /* en passant */
					movePiece(startFile, startRank, endFile, endRank, pieceType);
					movePiece(endFile, startRank, 9, 9, 0);
					break;
					
				case 3: /* castle */
					movePiece(startFile, startRank, endFile, endRank, pieceType);
					if(endFile > startFile) /* kingside */
					{
						movePiece(8, startRank, 6, startRank, 5);
					}
					else /* queen side */
					{
						movePiece(1, startRank, 4, startRank, 5);
					}
					
					break;
			}
			/************************************************************/
			
			while(!eyes.getMove(move)) /* move not found */
			{
				board->print();
				printf("Unable to find move. Enter 1 to try again or 0 to quit: ");
				std::cin >> notDone;
				if(!notDone)
				{
					return 0;
				}
				std::cout << std::endl;
			}
			
			printf("Jelly's move found.\n");
			// print move
			std::cout << getFile(move[0][1]) << move[0][0] + 1 << " -> " << getFile(move[1][1]) << move[1][0] + 1 << std::endl;
			
			while(!verifyBoard(board, eyes))
			{
				board->print();
				printf("Please fix board to match above board.\nEnter 1 to continue or 0 to quit: ");
				std::cin >> notDone;
				if(!notDone)
				{
					break;
				}
				eyes.updateCurrentBoard();
			}
			eyes.copyCurrentBoard();
			
			isPlayersTurn = !isPlayersTurn;			
		}
	}


	return 0;
}

char getFile(int j)
{
	char file = '0';
	switch(j)
	{
		case 0:
			file = 'a';
			break;
		case 1:
			file = 'b';
			break;
		case 2:
			file = 'c';
			break;
		case 3:
			file = 'd';
			break;
		case 4:
			file = 'e';
			break;
		case 5:
			file = 'f';
			break;
		case 6:
			file = 'g';
			break;
		case 7:
			file = 'h';
			break;

	}
	return file;
}

bool verifyBoard(Board * board, Vision vision)
{
	bool verified = true;
	for(int i = 0; i < 8 && verified; i++)
	{
		for(int j = 0; j < 8 && verified; j++)
		{
			if(board->getPiece(i, j) != nullptr)
			{
				if(board->getPiece(i, j)->getColor() == Black) /* black piece */
				{
					if(vision.getCurrentBoard(i, j) != 2)
					{
						verified = false;
					}
				}
				else /* white piece */
				{
					if(vision.getCurrentBoard(i, j) != 1)
					{
						verified = false;
					}
				}
			}
			else /* square is empty */
			{
				if(vision.getCurrentBoard(i, j) != 0) /* square is not empty */
				{
					verified = false;
				}
			}
		}
	}
	
	return verified;
}

int getPieceType(int pieceType)
{
	int mariosTypes[] = { 3, 1, 4, 0, 2, 5 };

	if (pieceType > 0)
	{
		return mariosTypes[pieceType];
	}

	std::cerr << "Wrong piece type returned." << std::endl;
	return -1;
}
