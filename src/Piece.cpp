#include "Piece.hpp"

/**
 * Piece.cpp, defines the constructor and methods for generating chess pieces.
 *
 * Created on 21 December, 2016
 *
 * @author	Allan Rodriguez <allanr1@uci.edu>
 */

namespace JellyAI
{
	Piece::Piece(PieceColor color, PieceType type)
	{
		this->color = color;
		this->type = type;
	}

	Piece::Piece(Piece * piece)
	{
		this->color = piece->color;
		this->type = piece->type;
	}

	PieceColor Piece::getColor()
	{
		return this->color;
	}

	PieceType Piece::getType()
	{
		return this->type;
	}
}
