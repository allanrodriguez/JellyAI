#include "AI.hpp"

/**
 * AI.cpp
 *
 * Created on 13 January, 2017
 *
 * @author	Allan Rodriguez <allanr1@uci.edu>
 *
 * "I fear all I have done is to awaken a sleeping giant and fill him with a
 * terrible resolve."
 *
 * - The 1986 chess grandmaster after challenging Jelly to a match. He went on
 *   to lose after the 10th full move following a stunning Giuoco Piano opening,
 *   followed by a glaring hole in the grandmaster's play, resulting in a
 *   checkmate amid his incessant sobbing.
 *
 *   To this day, it remains a controversial loss for the erstwhile grandmaster,
 *   with some conspiracy theorists speculating that a combination of negging
 *   and possibly even blackmail on Jelly's behalf were the culprits.
 *
 *   Such theories remain to be proven. Below, you will find NO code that would
 *   lead to such a play from Jelly. None at all. You have been warned.
 */

namespace JellyAI
{
	/**
	 * Creates the root AI node using a copy of the board it is given. The
	 * result of the AI move tree will be returned to this node. Finds optimal
	 * move for the color specified.
	 */
	AI::AI(Board * try1board, PieceColor jellysColor)
	{
		this->startRank = -1;
		this->startFile = -1;
		this->endRank = -1;
		this->endFile = -1;
		this->nodeCount = 1;
		this->isMoveSuccessful = true;
		this->parent = nullptr;
		this->root = this;
		this->color = jellysColor;
		this->startPieceType = -1;
		this->try1board = try1board;
		// this->choice = new std::vector<AI *>();
		this->value = 0;
		// std::cerr << "Initial " << (this->color == Black ? "black" : "white") <<
		// 	" points: " << this->value << std::endl;
	}

	/**
	 * Creates branch AI nodes where moves will be evaluated.
	 *
	 * May be deprecated if the information can be manipulated without creating
	 * a new object.
	 */
	AI::AI(int startRank, int startFile, int endRank, int endFile, AI * parent)
	{
		this->startRank = startRank;
		this->startFile = startFile;
		this->endRank = endRank;
		this->endFile = endFile;
		this->nodeCount = 0;
		this->parent = parent;
		this->root = parent->root;
		this->color = parent->color;
		this->startPieceType = -1;
		this->try1board = new Board(parent->try1board, false);
		this->isMoveSuccessful = this->try1board->_movePiece(startRank, startFile, endRank, endFile);
		// this->choice = new std::vector<AI *>();
		this->value = 0;
	}

	/**
	 * AI destructor
	 *
	 * Disappointed near the end of its life, the AI breaks its chessboard,
	 * kills its children, and sets its house on fire.
	 */
	AI::~AI()
	{
		if (this->parent != nullptr)
		{
			delete this->try1board;
		}
		this->try1board = nullptr;
		this->parent = nullptr;

		int size = this->choice.size();

		for (int i = 0; i < size; ++i)
		{
			delete choice.at(i);
		}

		// delete this->choice;
		// this->choice = nullptr;
	}

	void AI::getChoices(int rank, int file)
	{
		AI * potential = nullptr;
		Result result;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				/* Check is not a valid condition here because we don't want the
				 * king to get captured!
				 */
				if ( (result = this->try1board->checkLegalMove(rank, file, i, j) ) == Legal ||
						result == Castling || result == EnPassant)
				{
					potential = new AI(rank, file, i, j, this);
					if (potential->isMoveSuccessful)
					{
						this->choice.push_back(potential);
						this->root->nodeCount++;
					}
					else
					{
						delete potential;
					}
					potential = nullptr;
				}
			}
		}
	}

	int AI::getPiecePoints(PieceColor color)
	{
		Piece * piece = nullptr;
		int piecePoints[] = { 0, 0 };
		int pawnCount[] = { 0, 0 };
		int knightCount[] = { 0, 0 };
		int rookCount[] = { 0, 0 };
		int difference = 0;
		PieceColor pc;
		PieceColor oppositeColor = (color == Black) ? White : Black;

		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if ( (piece = this->try1board->getPiece(i, j) ) != nullptr)
				{
					pc = piece->getColor();
					switch ( piece->getType() )
					{
						case Pawn:
							piecePoints[pc] += 100;
							pawnCount[pc]++;
							break;
						case Knight:
							knightCount[pc]++;
						case Bishop:
							piecePoints[pc] += 300;
							break;
						case Rook:
							piecePoints[pc] += 500;
							rookCount[pc]++;
							break;
						case Queen:
							piecePoints[pc] += 900;
							break;
						case King:
							piecePoints[pc] += 10000;
							break;
						default:
							break;
					}

					if (pc == this->color && i < 6 && i > 1 && j < 6 && j > 1)
					{
						piecePoints[pc] += 25;
						if ( ( (this->color == White && i < 5) ||
							(this->color == Black && i > 2) ) &&
							(j == 3 || j == 4) )
						{
							piecePoints[pc] += 32;
						}
					}
				}
			}
		}

		/* Knight's value decreases, rook's value increases as pawns are
		 * captured
		 * If all the pawns are on the board, knights will gain 160 points,
		 * and rooks will gain nothing */
		piecePoints[Black] += ( (pawnCount[Black] - 4) * 20 * knightCount[Black]);
		piecePoints[White] += ( (pawnCount[White] - 4) * 20 * knightCount[White]);
		piecePoints[Black] += ( (4 - pawnCount[Black]) * 20 * rookCount[Black]);
		piecePoints[White] += ( (4 - pawnCount[White]) * 20 * rookCount[White]);

		/* Subtracting the difference in points between sides from the side being
		 * evaluated */
		 difference = piecePoints[oppositeColor] - piecePoints[color];
		 piecePoints[color] -= difference;

		return piecePoints[color];
	}

	int AI::evaluate(int depth)
	{
		AI * prevAI = this->parent;
		// AI * temp = this;
		Board * board = this->try1board;
		Piece * startPiece = nullptr;
		// Piece * sTemp = nullptr, * eTemp = nullptr;
		PieceColor oppositeColor = (this->color == Black) ? White : Black;
		int pieceValues[6], pawnCount = 0;

		this->value = this->getPiecePoints(this->color);

		pieceValues[Pawn] = 100;
		pieceValues[Bishop] = pieceValues[Knight] = 300;
		pieceValues[Rook] = 500;
		pieceValues[Queen] = 900;
		pieceValues[King] = 10000;

		/* Assigning a value to checkmate. The highest is the maximum integer value,
		 * while the lowest is the lowest integer value */
		if (board->isCheckmate(oppositeColor))
		{
			this->value = INT_MAX;
			return this->value;
		}
		else if (board->isCheckmate(this->color))
		{
			this->value = INT_MIN;
			return this->value;
		}

		startPiece = board->getPiece(this->endRank, this->endFile);

		/* Penalty for having incurred a prior check */
		// if (prevAI != nullptr)
		// {
		// 	startPiece = prevAI->try1board->getPiece(this->startRank, this->startFile);
		//
		// 	if ( prevAI->try1board->getCheck(this->color) )
		// 	{
		// 		this->value -= 500;
		// 	}
		// 	else if ( prevAI->try1board->getCheck(oppositeColor) )
		// 	{
		// 		this->value += 500;
		// 	}
		// }

		/* Assigning values for check. Value equivalent to double the queen's value */
		if ( board->getCheck(oppositeColor) )
		{
			this->value += 10;
			if ( board->isPieceCapturable(this->endRank, this->endFile, oppositeColor) )
			{
				this->value -= pieceValues[startPiece->getColor()];
			}
			for (int i = 0; i < 8; ++i)
			{
				for (int j = 0; j < 8; ++j)
				{
					if ( board->checkLegalMove(this->endRank, this->endFile, i, j) == Legal &&
						board->getPiece(i, j) != nullptr && board->getPiece(i, j)->getColor() != this->color)
					{
						this->value += (int) ( (double) pieceValues[board->getPiece(i, j)->getType()] /
							pieceValues[startPiece->getType()] * 10.0);
					}
				}
			}
		}
		else if ( board->getCheck(color) )
		{
			this->value -= 100;
			if ( board->isPieceCapturable(this->endRank, this->endFile, oppositeColor) )
			{
				this->value += pieceValues[startPiece->getColor()];
			}
			for (int i = 0; i < 8; ++i)
			{
				for (int j = 0; j < 8; ++j)
				{
					if ( board->checkLegalMove(this->endRank, this->endFile, i, j) == Legal &&
						board->getPiece(i, j) != nullptr && board->getPiece(i, j)->getColor() == this->color)
					{
						this->value -= (int) ( (double) pieceValues[board->getPiece(i, j)->getType()] /
							pieceValues[startPiece->getType()] * 10.0);
					}
				}
			}
		}

		/* Giving captures a value proportional to value of both capturing and captured pieces.
		 * The highest value possible is the full + half the value of the captured piece, while
		 * the lowest value is 100.
		 */
		// while (temp->parent != nullptr)
		// {
		// 	if (temp->try1board->getNumPieces() < temp->parent->try1board->getNumPieces())
		// 	{
		// 		sTemp = temp->parent->try1board->getPiece(temp->startRank, temp->startFile);
		// 		eTemp = temp->parent->try1board->getPiece(temp->endRank, temp->endFile);
		//
		// 		// Assigning value to en passant capture
		// 		if (sTemp->getType() == Pawn && eTemp == nullptr)
		// 		{
		// 			eTemp = temp->parent->try1board->getPiece(temp->startRank, temp->endFile);
		// 		}
		//
		// 		if (temp->color == sTemp->getColor())
		// 		{
		// 			// std::cerr << "Old value: " << this->value << std::endl;
		// 			this->value += (int)
		// 				( (double) pieceValues[eTemp->getType()] / pieceValues[sTemp->getType()] * 50.0 ) +
		// 				pieceValues[eTemp->getType()];
		// 			// std::cerr << "New value: " << this->value << std::endl;
		// 		}
		// 	}
		// 	temp = temp->parent;
		// }

		switch ( startPiece->getType() )
		{
			/* Penalty for developing the queen too early
			 * Queen loses 90% of its value if moved when all pawns have not moved,
			 * at least 56% of its value when at least 2 pawns have not moved
			 */
			case Queen:
				for (int i = 0; i < 8; ++i)
				{
					Piece * p = board->getPiece( (startPiece->getColor() == Black) ? 6 : 1, i);
					if (p != nullptr &&
						p->getType() == Pawn && p->getColor() == startPiece->getColor() )
					{
						pawnCount++;
					}
				}

				if (pawnCount > 2)
				{
					this->value = (startPiece->getColor() == this->color) ? this->value - 500 : this->value + 500;
				}
				this->value = (startPiece->getColor() == this->color) ?
					this->value - ( (pawnCount - 2) * 200) : this->value + ( (pawnCount - 2) * 200);
				break;

			/* Incentive for king to perform castling if it is possible, since
			 * it makes the king safer */
			case King:
				if (!prevAI->try1board->canCastle( startPiece->getColor() ) &&
					( (prevAI->endFile == 6 && !prevAI->try1board->hasRookMoved(startPiece->getColor(), Kingside) ) ||
						(prevAI->endFile == 2 && !prevAI->try1board->hasRookMoved(startPiece->getColor(), Queenside) ) ) )
				{
					this->value = (startPiece->getColor() == this->color) ?
						this->value + pieceValues[King] + 1 : this->value - pieceValues[King] - 1;
				}
				break;
			default:
				break;
		}

		return this->value;
	}

	int AI::alphaBeta(int depth, int alpha, int beta)
	{
	    int returnValue = 0, size = -1;
	    int newAlpha = alpha, newBeta = beta;
	    AI * child = nullptr;
		Piece * piece = nullptr;

	    if (depth > 2 || this->try1board->isCheckmate(White) || this->try1board->isCheckmate(Black) )
	    {
			return this->evaluate(depth);
	    }

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				piece = this->try1board->getPiece(i, j);
				if (piece != nullptr && ( (depth % 2 == 0 && piece->getColor() == this->color) ||
						(depth % 2 == 1 && piece->getColor() != this->color) ) )
				{
					this->getChoices(i, j);
				}
			}
		}

		size = this->choice.size();

	    /* Maximizing Jelly's points when it is its turn to move */
	    if (depth % 2 == 0)
	    {
	        returnValue = INT_MIN;
	        for (int i = 0; i < size; ++i)
	        {
	            child = this->choice.at(i);
	            returnValue = std::max(returnValue, child->alphaBeta(depth + 1, newAlpha, beta) );
	            newAlpha = std::max(newAlpha, returnValue);

	            if (beta <= newAlpha)
	            {
					for (int j = size - 1; j > i; --j)
					{
						delete this->choice.at(j);
						this->choice.pop_back();
					}
					i = size;
	            }
	        }
			this->value = returnValue;
	        return returnValue;
	    }
	    /* Minimizing Jelly's points when it is the opponent's turn to move */
	    else
	    {
	        returnValue = INT_MAX;
	        for (int i = 0; i < size; ++i)
	        {
	            child = this->choice.at(i);
	            returnValue = std::min(returnValue, child->alphaBeta(depth + 1, alpha, newBeta) );
	            newBeta = std::min(newBeta, returnValue);

	            if (newBeta <= alpha)
	            {
					for (int j = size - 1; j > i; --j)
					{
						delete this->choice.at(j);
						this->choice.pop_back();
					}
	                i = size;
	            }
	        }
			this->value = returnValue;
	        return returnValue;
	    }
	}

	bool AI::jellysMove()
	{
		int maxPoints = 0, randomIndex = 0;
		char files[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		std::vector<AI *> contenders;
		AI * finally = nullptr;
		int size = 0;

		/* Where the magic happens */
		std::cerr << "[AI] Return value: " << ( maxPoints = this->alphaBeta(0, INT_MIN, INT_MAX) ) << std::endl;

		for (unsigned int i = 0; i < this->choice.size(); ++i)
		{
			if (this->choice.at(i)->value == maxPoints)
			{
				std::cerr << files[this->choice.at(i)->startFile] << this->choice.at(i)->startRank + 1 <<
					" -> " << files[this->choice.at(i)->endFile] << this->choice.at(i)->endRank + 1 <<
					std::endl;
				contenders.push_back( this->choice.at(i) );
			}
		}

		if (contenders.size() > 0)
		{
			srand( time(NULL) );
			randomIndex = rand() % contenders.size();
			finally = contenders.at(randomIndex);
			this->startPieceType = this->try1board->getPiece(finally->startRank, finally->startFile)->getType();
			this->try1board->movePiece(finally->startRank, finally->startFile, finally->endRank, finally->endFile);

			std::cerr << "[AI] Final choice: " << files[contenders.at(randomIndex)->startFile] <<
				contenders.at(randomIndex)->startRank + 1 << " -> " << files[contenders.at(randomIndex)->endFile] <<
				contenders.at(randomIndex)->endRank + 1 << std::endl;
			this->startRank = contenders.at(randomIndex)->startRank;
			this->startFile = contenders.at(randomIndex)->startFile;
			this->endRank = contenders.at(randomIndex)->endRank;
			this->endFile = contenders.at(randomIndex)->endFile;

			size = this->choice.size();
			for (int i = 0; i < size; ++i)
			{
				if (this->choice.at(i)->startRank != contenders.at(randomIndex)->startRank ||
					this->choice.at(i)->startFile != contenders.at(randomIndex)->startFile ||
					this->choice.at(i)->endRank != contenders.at(randomIndex)->endRank ||
					this->choice.at(i)->endFile != contenders.at(randomIndex)->endFile)
					{
						delete this->choice.at(i);
						this->choice.at(i) = nullptr;
					}
			}

			std::cerr << "[AI] Number of AI nodes: " << this->getNodeCount() << std::endl;

			return true;
		}
		else
		{
			std::cerr << "[AI] No correct moves found. Most likely, the AI has "<<
				"been set to play a side during the wrong turn." << std::endl;
		}

		return false;
	}

	int AI::getNodeCount()
	{
		return this->nodeCount;
	}

	int AI::getStartPieceType()
	{
		int mariosTypes[] = { 3, 1, 4, 0, 2, 5 };

		if (this->startPieceType >= 0)
		{
			return mariosTypes[this->startPieceType];
		}

		std::cerr << "[AI] Wrong start piece type returned." << std::endl;
		return -1;
	}

	void AI::getMove(int holder[3][2])
	{
		Board * prevBoard = this->try1board->getPreviousBoard();
		Piece * piece = prevBoard->getPiece(this->startRank, this->startFile);

		for (int i = 0; i < 6; ++i)
		{
			holder[i / 2][i % 2] = -1;
		}

		holder[0][0] = this->startRank;
		holder[0][1] = this->startFile;
		holder[1][0] = this->endRank;
		holder[1][1] = this->endFile;
		holder[2][1] = this->getStartPieceType();

		if (prevBoard->getPiece(this->endRank, this->endFile) != nullptr)
		{
			holder[2][0] = 1;	// Capturing a piece
		}
		else if (prevBoard->checkLegalMove(this->startRank, this->startFile, this->endRank, this->endFile) == EnPassant)
		{
			holder[2][0] = 2;	// En passant
		}
		else if (prevBoard->checkLegalMove(this->startRank, this->startFile, this->endRank, this->endFile) == Castling)
		{
			if (piece->getColor() == Black)
			{
				holder[2][0] = 3;	// Black castling
			}
			else
			{
				holder[2][0] = 4;	// White castling
			}
		}
		else
		{
			holder[2][0] = 0;	// Regular move
		}
	}
}
