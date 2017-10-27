/**
 * Board.hpp
 *
 * Created on 21 December, 2016
 *
 * @author	Allan Rodriguez <allanr1@uci.edu>
 */

#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "Piece.hpp"

namespace JellyAI
{
	enum Result
	{
		Legal,
		Illegal,
		Castling,
		EnPassant,
		Check
	};

	enum Direction
	{
		Kingside,
		Queenside
	};

	class Board
	{
		private:
			bool castling[2];
			bool check[2];
			bool rookMoved[2][2];
			int enPassantRank;
			int enPassantFile;
			int numPieces;
			int moveCount;
			int kingLocations[2][2];
			PieceColor colorTurn;
			Board * previousBoard;
			Piece * board[8][8];

			bool canPathBeBlocked(int, int, int, int, PieceColor);
			bool move(int, int, int, int);
		public:
			Board();
			Board(Board *, bool);
			Board(std::string fen);
			~Board();
			Piece * getPiece(int, int);
			Piece * setPiece(PieceColor, PieceType);
			bool movePiece(int, int, int, int);
			bool _movePiece(int, int, int, int);
			void print();
			int getNumPieces();
			Result checkLegalMove(int, int, int, int);
			bool isInCheck(int, int, PieceColor);
			bool isPieceCapturable(int, int, PieceColor);
			bool isCheckmate(PieceColor);
			bool isStalemate(PieceColor);
			Board * getPreviousBoard();
			bool getCheck(PieceColor);
			bool canCastle(PieceColor);
			bool hasRookMoved(PieceColor, Direction);
			int getKingLocations(PieceColor, int);
	};
}

#endif /* BOARD_HPP_ */
