#include <iostream>
#include "Board.hpp"
#include "AI.hpp"

/*
 * main.cpp
 *
 * Created on 21 December, 2016
 *
 * @author	Allan Rodriguez <allanr1@uci.edu>
 */

using namespace JellyAI;

void printMoveMap(Board * board, int rank, int file);

void playGame(Board * board, bool withAI);

int main()
{
	Board * board = new Board();
	// TESTS
	// Board * board = new Board("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
	// Board * board = new Board("6k1/8/8/8/8/7b/8/R3K2R w KQ-- - 1 2");
	// Board * board = new Board("rnb1k2r/1ppp1ppp/p3Q2n/4p1q1/2P1P3/8/PP1P1PPP/RNB1KBNR b KQkq - 1 2");
	// Board * board = new Board("k1q5/6Q1/3Qp3/pp2P3/P1P5/R4N2/1P1NBnP1/2B2RK1 w ---- - 1 2");
	// Board * board = new Board("rnbqk2r/pppp1P1p/6nb/4N3/6pP/8/PPPP2B1/RNBQ1RK1 b --kq - 1 2");
	// Board * board = new Board("r3k3/1pp1n3/p3Q2r/2N1p1pp/8/2P2NPP/PPn1KP2/R1B4R w ---q - 1 2");
	// Board * board = new Board("5b1r/4pkpp/8/6P1/1r6/4n2P/PPb5/2R1K1NR b K--- - 1 2");
	// Board * board = new Board("K5qk/8/8/8/1r5P/8/1r6/7b w ---- - 1 2");
	// Board * board = new Board("rnb1kbnr/ppp2ppp/3p4/4p1q1/6P1/2N1P2P/PPPP1P2/R1BQKBNR b KQkq - 1 2");

// 1/25/2015
	// Board * board = new Board("4RN1n/1Nb2pB1/P2P4/r4kpp/R7/8/p4p1K/rbQ5 w ---- - 1 2");

// 1/27/2013
	// Board * board = new Board("8/7B/6n1/8/3pp1Pq/3N1k1p/2pQ3P/2K2NR1 w ---- - 1 2");

	playGame(board, true);
	// board->movePiece(5, 4, 6, 3);
	// board->movePiece(7, 7, 7, 6);
	// board->print();

	// std::cout << board->getNumPieces() << " pieces\n" << std::endl;
	// board->print();
	// std::cout << board->movePiece(7, 3, 2, 3) << "\n" << std::endl;
	board->print();

	// std::cerr << (board->isInCheck(4, 5, Black) ? "true" : "false") << std::endl;

	// printMoveMap(board, 0, 4);

	// board->movePiece(6, 4, 7, 5);
	// board->print();
	// board->movePiece(7, 5, 7, 6);
	// board->print();

	// std::cerr << board->movePiece(7, 4, 7, 5) << std::endl;
	// std::cerr << board->movePiece(3, 2, 4, 2) << std::endl;
	// std::cerr << board->movePiece(6, 3, 4, 3) << std::endl;
	// board->print();
	// std::cerr << board->movePiece(4, 2, 5, 3) << std::endl;
	// board->print();
	// std::cerr << "Black king: " << board->isInCheck(2, 5, Black) << std::endl;
	// std::cerr << "White king: " << board->isInCheck(0, 2, White) << std::endl;

	// int b[3][2];
	// AI * answer = new AI(board, White);
	// answer->jellysMove();
	// answer->getMove(b);
	// for (int i = 0; i < 6; ++i)
	// {
	// 	std::cerr << b[i / 2][i % 2] << std::endl;
	// }
	// board->print();
	//
	// std::cerr << "Deleting AI nodes..." << std::endl;
	// delete answer;

	// board->movePiece(1, 3, 2, 4);
	// board->print();
	// board->movePiece(3, 7, 4, 7);
	// board->print();
	// board->movePiece(3, 3, 2, 4);
	// board->print();

	std::cerr << "Deleting board..." << std::endl;
	delete board;

	return 0;
}

void printMoveMap(Board * board, int rank, int file)
{
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			switch (board->checkLegalMove(rank, file, i, j))
			{
				case 1:
					std::cerr << "_";
					break;
				case 3:
					std::cerr << "C";
					break;
				default:
					std::cerr << "X";
			}
			std::cerr << " ";
		}
		std::cerr << std::endl;
	}
}

void playGame(Board * board, bool withAI)
{
	std::string temp;
	char startPiece[2], endPiece[2];
	board->print();
	int b[3][2];

	if (withAI)
	{
		AI jelly(board, White);
		jelly.jellysMove();
		board->print();
	}

	while ( !board->isCheckmate(White) && !board->isCheckmate(Black) &&
		!board->isStalemate(Black) && !board->isStalemate(White) )
	{
		std::cout << "Enter start piece location: ";
		std::cin >> temp;

		if (temp.length() != 2)
		{
			std::cout << "Try again." << std::endl;
			continue;
		}

		startPiece[0] = temp.at(0);
		startPiece[1] = temp.at(1);

		if (startPiece[0] < 'a' || startPiece[0] > 'h' || startPiece[1] < '1' || startPiece[1] > '8')
		{
			std::cout << "Try again." << std::endl;
			continue;
		}

		temp.clear();
		std::cout << "Enter end piece location: ";
		std::cin >> temp;

		if (temp.length() != 2)
		{
			std::cout << "Try again." << std::endl;
			continue;
		}

		endPiece[0] = temp.at(0);
		endPiece[1] = temp.at(1);

		if (endPiece[0] < 'a' || endPiece[0] > 'h' || endPiece[1] < '1' || endPiece[1] > '8')
		{
			std::cout << "Try again." << std::endl;
			continue;
		}

		if (board->movePiece(startPiece[1] - '1', startPiece[0] - 'a', endPiece[1] - '1', endPiece[0] - 'a'))
		{
			board->print();
			if (withAI)
			{
				AI jelly(board, White);
				jelly.jellysMove();
				board->print();
				jelly.getMove(b);
				// for (int i = 0; i < 6; ++i)
				// {
					// std::cerr << b[i / 2][i % 2] << std::endl;
				// }
				if (b[2][0] == 1)
				{
					std::cerr << board->getPreviousBoard()->getPiece(b[1][0], b[1][1])->getType() << std::endl;
				}
			}
		}
	}
}
