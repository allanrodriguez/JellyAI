/**
 * Piece.hpp, header file for the chess pieces.
 *
 * Created on 21 December, 2016
 *
 * @author	Allan Rodriguez <allanr1@uci.edu>
 */

#ifndef PIECE_HPP_
#define PIECE_HPP_

#ifdef NO_C11
	#define nullptr NULL
#endif

namespace JellyAI
{
	enum PieceColor
	{
		Black,
		White
	};

	enum PieceType
	{
		Bishop,
		King,
		Knight,
		Pawn,
		Queen,
		Rook
	};

	class Piece
	{
		private:
			PieceColor color;
			PieceType type;
		public:
			Piece(PieceColor, PieceType);
			Piece(Piece *);
			PieceColor getColor();
			PieceType getType();
	};
}

#endif /* PIECE_HPP_ */
