#include "Board.hpp"

/**
 * Board.cpp
 *
 * Created on 21 December, 2016
 *
 * @author	Allan Rodriguez <allanr1@uci.edu>
 *
 * What does an Australian say after a meal at a restaurant?
 *
 * Check, mate!
 */

namespace JellyAI
{
	Board::Board()
	{
		this->previousBoard = nullptr;
		this->numPieces = 0;
		this->moveCount = 0;
		this->colorTurn = White;
		this->enPassantRank = -1;
		this->enPassantFile = -1;
		this->kingLocations[Black][0] = 7;
		this->kingLocations[Black][1] = 4;
		this->kingLocations[White][0] = 0;
		this->kingLocations[White][1] = 4;
		for (int i = 0; i < 2; i++)
		{
			this->check[i] = false;
			this->castling[i] = true;
			for (int j = 0; j < 2; j++)
			{
				this->rookMoved[i][j] = false;
			}
		}
		for (int i = 0; i < 8; i++)
		{
			switch (i)
			{
				case 0:
				case 7:
					this->board[i][0] = setPiece(i == 0 ? White : Black, Rook);
					this->board[i][1] = setPiece(i == 0 ? White : Black, Knight);
					this->board[i][2] = setPiece(i == 0 ? White : Black, Bishop);
					this->board[i][3] = setPiece(i == 0 ? White : Black, Queen);
					this->board[i][4] = setPiece(i == 0 ? White : Black, King);
					this->board[i][5] = setPiece(i == 0 ? White : Black, Bishop);
					this->board[i][6] = setPiece(i == 0 ? White : Black, Knight);
					this->board[i][7] = setPiece(i == 0 ? White : Black, Rook);
					break;
				case 1:
				case 6:
					for (int j = 0; j < 8; j++)
					{
						this->board[i][j] = setPiece(i == 1 ? White : Black, Pawn);
					}
					break;
				default:
					for (int j = 0; j < 8; j++)
					{
						this->board[i][j] = nullptr;
					}
			}
		}
	}

	/**
	 * isMainBoard determines whether the linked list of previous boards is
	 * copied over to this new board. It's mostly false, except for when a move
	 * has been made, and a previous board needs to be saved.
	 */
	Board::Board(Board * board, bool isMainBoard)
	{
		if (board != nullptr)
		{
			this->colorTurn = board->colorTurn;
			this->numPieces = board->numPieces;
			this->moveCount = board->moveCount;
			// Linked lists of previous boards for easy undoing of moves
			this->previousBoard = isMainBoard ? board->previousBoard : nullptr;
			this->enPassantRank = board->enPassantRank;
			this->enPassantFile = board->enPassantFile;
			for (int i = 0; i < 2; i++)
			{
				this->check[i] = board->check[i];
				this->castling[i] = board->castling[i];
				for (int j = 0; j < 2; j++)
				{
					this->rookMoved[i][j] = board->rookMoved[i][j];
					this->kingLocations[i][j] = board->kingLocations[i][j];
				}
			}
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (board->board[i][j] != nullptr)
					{
						this->board[i][j] = new Piece(board->board[i][j]);
					}
					else
					{
						this->board[i][j] = nullptr;
					}
				}
			}
		}
		else
		{
			std::cerr << "[Board] Board is null" << std::endl;
		}
	}

	Board::Board(std::string fen)
	{
		// Keeps track of the dividing character in the string, either a slash or a space
		unsigned int first = 0;
		char all[10];
		int charToInt = 0, l = 0;
		std::string thePartIWant;	// Holds the substring in between the dividing characters
		this->numPieces = 0;
		this->moveCount = 0;
		this->check[0] = this->check[1] = false;
		this->previousBoard = nullptr;

		for (int i = 0; i < 8; i++)
		{
			if (i < 7)
			{
				thePartIWant = fen.substr(first, fen.find_first_of("/", first + 1) - first);
				first = fen.find_first_of("/", first + 1) + 1;
			}
			else
			{
				thePartIWant = fen.substr(first, fen.find_first_of(" ") - first);
				first = fen.find_first_of(" ") + 1;
			}
			strncpy(all, thePartIWant.c_str(), 10);

			l = 0;
			for (int j = 0; j < 8 && all[j] != '\0'; j++)
			{
				charToInt = all[j] - '0';	// To differentiate empty spaces from pieces
				if (charToInt > 0 && charToInt <= 8)
				{
					for (int k = 0; k < charToInt; k++)
					{
						this->board[7 - i][l++] = nullptr;
					}
				}
				else
				{
					switch (all[j])
					{
						case 'B':
							this->board[7 - i][l] = setPiece(White, Bishop);
							break;
						case 'b':
							this->board[7 - i][l] = setPiece(Black, Bishop);
							break;
						case 'K':
							this->board[7 - i][l] = setPiece(White, King);
							this->kingLocations[White][0] = 7 - i;
							this->kingLocations[White][1] = l;
							break;
						case 'k':
							this->board[7 - i][l] = setPiece(Black, King);
							this->kingLocations[Black][0] = 7 - i;
							this->kingLocations[Black][1] = l;
							break;
						case 'N':
							this->board[7 - i][l] = setPiece(White, Knight);
							break;
						case 'n':
							this->board[7 - i][l] = setPiece(Black, Knight);
							break;
						case 'P':
							this->board[7 - i][l] = setPiece(White, Pawn);
							break;
						case 'p':
							this->board[7 - i][l] = setPiece(Black, Pawn);
							break;
						case 'Q':
							this->board[7 - i][l] = setPiece(White, Queen);
							break;
						case 'q':
							this->board[7 - i][l] = setPiece(Black, Queen);
							break;
						case 'R':
							this->board[7 - i][l] = setPiece(White, Rook);
							break;
						case 'r':
							this->board[7 - i][l] = setPiece(Black, Rook);
							break;
						default:
							--this->numPieces;
							std::cerr << "[Board] Unexpected condition in Board(std::string fen) constructor." << std::endl;
							this->board[7 - i][l] = nullptr;
							break;
					}
					l++;
				}
			}
		}

		this->castling[0] = this->castling[1] = true;

		for (int i = 8; i < 11; i++)	// Getting turn to move, states of castling options, and en passant options
		{
			thePartIWant = fen.substr(first, fen.find_first_of(" ", first + 1) - first);
			strncpy(all, thePartIWant.c_str(), 10);

			switch(i)
			{
				case 8:		// Determining who is next to move
					this->colorTurn = (all[0] == 'b') ? Black : White;
					break;
				case 9:		// Castling options
					for (int j = 0; j < 4; j++)
					{
						this->rookMoved[j / 2][j % 2] = true;
					}
					for (int j = 0; j < 4; j++)
					{
						switch(all[j])
						{
							case 'K':
								this->rookMoved[White][Kingside] = false;
								break;
							case 'k':
								this->rookMoved[Black][Kingside] = false;
								break;
							case 'Q':
								this->rookMoved[White][Queenside] = false;
								break;
							case 'q':
								this->rookMoved[Black][Queenside] = false;
								break;
							default:
								break;
						}
					}
					break;
				case 10:
					if (all[0] != '-')
					{
						this->enPassantRank = (int) (all[1] - '0') - 1;
						this->enPassantFile = (int) (all[0] - 'a') - 1;
					}
					else
					{
						this->enPassantRank = -1;
						this->enPassantFile = -1;
					}
					break;
				default:
					break;
			}
			first = fen.find_first_of(" ", first + 1) + 1;
		}

		if ( (this->rookMoved[White][Kingside] && this->rookMoved[White][Queenside]) ||
			(this->getPiece(0, 4) == nullptr || ( this->getPiece(0, 4) != nullptr &&
			(this->getPiece(0, 4)->getType() != King || this->getPiece(0, 4)->getColor() != White) ) ) )
		{
			this->castling[White] = false;
		}
		if ( (this->rookMoved[Black][Kingside] && this->rookMoved[Black][Queenside]) ||
			(this->getPiece(7, 4) == nullptr || ( this->getPiece(7, 4) != nullptr &&
			(this->getPiece(7, 4)->getType() != King || this->getPiece(7, 4)->getColor() != Black) ) ) )
		{
			this->castling[Black] = false;
		}

		if (kingLocations[Black][0] == -1 || kingLocations[White][0] == -1)
		{
			std::cerr << "[Board] Something in the FEN constructor has gone terribly wrong." << std::endl;
		}
		else
		{
			this->isInCheck(kingLocations[Black][0], kingLocations[Black][1], Black);
			this->isInCheck(kingLocations[White][0], kingLocations[White][1], White);
		}
	}

	Board::~Board()
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (this->board[i][j] != nullptr)
				{
					delete this->board[i][j];
					this->board[i][j] = nullptr;
				}
			}
		}
		if (this->previousBoard != nullptr)
		{
			delete this->previousBoard;
			this->previousBoard = nullptr;
		}
	}

	Piece * Board::getPiece(int rank, int file)
	{
		if (rank >= 0 && rank < 8 && file >= 0 && file < 8)
		{
			return this->board[rank][file];
		}
		return nullptr;
	}

	Piece * Board::setPiece(PieceColor color, PieceType type)
	{
		this->numPieces++;
		return new Piece(color, type);
	}

	bool Board::movePiece(int startRank, int startFile, int endRank, int endFile)
	{
		if (this->getPiece(startRank, startFile) == nullptr)
		{
			return false;
		}

		PieceColor oppositeColor = (this->getPiece(startRank, startFile)->getColor() == Black) ? White : Black;
		char file[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		bool result = false;

		if ( !(result = this->_movePiece(startRank, startFile, endRank, endFile) ) )
		{
			std::cout << "[Board] Illegal move: " << file[startFile] << startRank + 1 << " -> " <<
				file[endFile] << endRank + 1 << " - " << (this->colorTurn == Black ? "Black's " : "White's ") <<
				"turn\n" << std::endl;
			this->print();
		}
		else if ( result && this->isCheckmate(oppositeColor) )
		{
			std::cout << "[Board] CHECKMATE! " <<
				(this->getPiece(endRank, endFile)->getColor() == Black ? "Black " : "White ") <<
				"wins!" << std::endl;
		}

		return result;
	}

	bool Board::_movePiece(int startRank, int startFile, int endRank, int endFile)
	{
		Piece * startPiece = this->getPiece(startRank, startFile);
		char fileArray[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

		if (startPiece == nullptr)
		{
			std::cout << "[Board] Illegal move: No starting piece found! " << fileArray[startFile] << startRank + 1 << " -> " <<
				fileArray[endFile] << endRank + 1 << " - " << (this->colorTurn == Black ? "Black's " : "White's ") <<
				"turn\n" << std::endl;
			return false;
		}
		int rank = -1, file = -1;

		/* Checking to see if kings are in check */
		this->isInCheck(kingLocations[Black][0], kingLocations[Black][1], Black);
		this->isInCheck(kingLocations[White][0], kingLocations[White][1], White);

		rank = kingLocations[startPiece->getColor()][0];
		file = kingLocations[startPiece->getColor()][1];

		if (this->check[startPiece->getColor()])
		{
			if (startPiece->getType() == King)
			{
				rank = endRank;
				file = endFile;
			}
			Board temp(this, false);
			if (temp.move(startRank, startFile, endRank, endFile) &&
				!temp.isInCheck(rank, file, startPiece->getColor() ) )
			{
				return this->move(startRank, startFile, endRank, endFile);
			}
			else if (temp.check[startPiece->getColor()])
			{
				return false;
			}
		}
		else
		{
			return this->move(startRank, startFile, endRank, endFile);
		}

		return false;
	}

	bool Board::move(int startRank, int startFile, int endRank, int endFile)
	{
		Piece * startPiece = this->board[startRank][startFile];
		Piece * endPiece = this->board[endRank][endFile];
		PieceColor pc;
		Result result;

		if (startRank >= 0 && startRank < 8 && startFile >= 0 && startFile < 8 &&
				startPiece != nullptr && startPiece->getColor() == this->colorTurn &&
				endRank >= 0 && endRank < 8 && endFile >= 0 && endFile < 8 &&
				!this->isCheckmate( startPiece->getColor() ) &&
				!this->isCheckmate( (startPiece->getColor() == Black) ? White : Black ) &&
				( (result = this->checkLegalMove(startRank, startFile, endRank, endFile) ) == Legal ||
						result == Castling || result == EnPassant) )
		{
			this->previousBoard = new Board(this, true);
			this->enPassantRank = -1;
			this->enPassantFile = -1;
										/* Saving board before the piece move in a backwards linked list */
			if (endPiece != nullptr)	/* If a piece is captured, decrease # of pieces on board */
			{
				delete endPiece;
				this->numPieces--;
			}
			this->board[endRank][endFile] = board[startRank][startFile];	/* Moving the piece */
			this->board[startRank][startFile] = nullptr;
			this->colorTurn = (this->colorTurn == Black) ? White : Black;	/* Changing the turn */
			this->moveCount++;

			pc = startPiece->getColor();

			switch (startPiece->getType())
			{
				case King:
					this->kingLocations[pc][0] = endRank;
					this->kingLocations[pc][1] = endFile;

					if (result == Castling && endFile == 2)
					{
						this->board[endRank][3] = board[endRank][0];	/* Moving the queenside rook */
						this->board[endRank][0] = nullptr;
					}
					else if (result == Castling && endFile == 6)
					{
						this->board[endRank][5] = board[endRank][7];	/* Moving the kingside rook */
						this->board[endRank][7] = nullptr;
					}
					this->castling[pc] = false;	/* Castling cannot be performed if king moves */
					break;
				case Pawn:
					if (result == EnPassant)	/* Deleting the enemy pawn in an en passant play */
					{
						delete this->board[(pc == Black) ? endRank + 1 : endRank - 1][endFile];
						this->board[(pc == Black) ? endRank + 1 : endRank - 1][endFile] = nullptr;
					}
					else if (endRank - startRank == 2)		/* Sets the square for opposite side to capture in an */
					{										/* en passant move. Gets reset after a move is made */
						this->enPassantRank = startRank + 1;
						this->enPassantFile = startFile;
					}
					else if (endRank - startRank == -2)
					{
						this->enPassantRank = endRank + 1;
						this->enPassantFile = startFile;
					}
					else if (endRank == 7 || endRank == 0)
					{
						delete this->board[endRank][endFile];
						this->board[endRank][endFile] = new Piece(pc, Queen);
						startPiece = this->board[endRank][endFile];
					}
					break;
				case Rook:
					if (startRank == 0)
					{
						if (startFile == 0)
						{
							this->rookMoved[White][Queenside] = true;
						}
						else if (startFile == 7)
						{
							this->rookMoved[White][Kingside] = true;
						}
					}
					else if (startRank == 7)
					{
						if (startFile == 0)
						{
							this->rookMoved[Black][Queenside] = true;
						}
						else if (startFile == 7)
						{
							this->rookMoved[Black][Kingside] = true;
						}
					}

					if (this->rookMoved[startPiece->getColor()][Queenside] &&
							this->rookMoved[startPiece->getColor()][Kingside])
					{	// Castling cannot be performed if both rooks have moved
						this->castling[startPiece->getColor()] = false;
					}
					break;
				default:
					break;
			}

			if (startPiece->getColor() == Black)
			{
				this->isInCheck(kingLocations[White][0], kingLocations[White][1], White);
			}
			else
			{
				this->isInCheck(kingLocations[Black][0], kingLocations[Black][1], Black);
			}

			return true;
		}

		return false;
	}

	void Board::print()
	{
		std::string color[] = { "b", "w" };
		std::string type[] = { "B", "K", "N", "P", "Q", "R" };

		std::cerr << "\n";
		for (int i = 7; i >= 0; i--)
		{
			std::cerr << i + 1 << "  ";
			for (int j = 0; j < 8; j++)
			{
				if (this->board[i][j] != nullptr)
				{
					std::cerr << color[this->board[i][j]->getColor()] <<
							  type[this->board[i][j]->getType()] << "  ";
				}
				else
				{
					std::cerr << "--  ";
				}
			}
			std::cerr << std::endl;
		}
		std::cerr << "   a   b   c   d   e   f   g   h\n" << std::endl;
	}

	/**
	 * Returns the number of pieces on the board
	 */
	int Board::getNumPieces()
	{
		return this->numPieces;
	}

	/**
	 * Check if move is legal and returns equivalent enum value for
	 * legal, illegal, castling, en passant, and check moves
	 */
	Result Board::checkLegalMove(int startRank, int startFile, int endRank, int endFile)
	{
		int x = endFile - startFile;	/* distance between positions on x-axis */
		int y = endRank - startRank;	/* distance between positions on y-axis */
		Result result = Illegal;
		Piece * startPiece = this->board[startRank][startFile];
		Piece * endPiece = this->board[endRank][endFile];

		if (startPiece == nullptr || (x == 0 && y == 0))
		{
			return Illegal;
		}

		switch (startPiece->getType())
		{
			case Pawn:
				if (endPiece == nullptr)	/* moving a piece to empty space */
				{
					if (startPiece->getColor() == White)	/* white pawn */
					{
						if (startRank == 1 && x == 0 && board[startRank + 1][startFile] == nullptr &&
								(y == 1 || (y == 2 && board[endRank][endFile] == nullptr) ) )
						{	/* in starting position and can't move sideways and piece is not blocking path */
							result = Legal;
						}
						else if (startRank > 1 && x == 0 && y == 1) /* not in starting position, moving forward */
						{
							result = Legal;
						}
						else if (startRank == 4 && y == 1 && (x == 1 || x == -1) )	/* check en passant conditions */
						{
							Piece * temp = this->board[startRank][endFile];

							if ( temp != nullptr && temp->getColor() == Black &&
									temp->getType() == Pawn &&
									endRank == this->enPassantRank && endFile == this->enPassantFile)
							{
								return EnPassant;
							}
						}
					}
					else    /* black pawn */
					{
						if (startRank == 6 && x == 0 && board[startRank - 1][startFile] == nullptr &&
								(y == -1 || (y == -2 && board[endRank][endFile] == nullptr) ) )
						{			/* in starting position and can't move sideways and piece is not blocking path */
							result = Legal;
						}
						else if (startRank < 6 && x == 0 && y == -1) 	/* not in starting position, moving forward */
						{
							result = Legal;
						}
						else if (startRank == 3 && y == -1 && (x == 1 || x == -1) )	/* check en passant conditions */
						{
							Piece * temp = this->board[startRank][endFile];

							if ( (x == 1 || x == -1) && temp != nullptr && temp->getColor() == White &&
									temp->getType() == Pawn &&
									endRank == this->enPassantRank && endFile == this->enPassantFile)
							{
								return EnPassant;
							}
						}
					}
				}
				else if (endPiece != nullptr && endPiece->getColor() != startPiece->getColor() && (x == 1 || x == -1) &&
						( (startPiece->getColor() == White && y == 1) || (startPiece->getColor() == Black && y == -1) ) )
				{									/* moving piece to occupied space, pieces are opposite color,
 													 * and white pawn capturing black or black pawn capturing white */
						result = Legal;
				}
				break;

			case Rook:
				if ( ( (x != 0 && y == 0) || (x == 0 && y != 0) ) && ( (endPiece == nullptr) ||
						(endPiece != nullptr && endPiece->getColor() != startPiece->getColor() ) ) )
				{											/* check legal movement and moving to occupied space and
 															 * piece colors are different or moving to empty space */
					if (x != 0 && y == 0)    /* Checking for blocked path when moving horizontally */
					{
						int i = x;
						while (abs(i) != 1)
						{
							(x > 0) ? i-- : i++;
							if (this->board[endRank][endFile - i] != nullptr)
							{
								return Illegal;
							}
						}
					}
					else    /* Checking for blocked path when moving vertically */
					{
						int i = y;
						while (abs(i) != 1)
						{
							(y > 0) ? i-- : i++;
							if (this->board[endRank - i][endFile] != nullptr)
							{
								return Illegal;
							}
						}
					}

					result = Legal;
				}
				break;

			case Knight:
				if ( ( (abs(x) == 1 && abs(y) == 2) || (abs(x) == 2 && abs(y) == 1) ) &&
						( (endPiece == nullptr) || (endPiece != nullptr &&
								endPiece->getColor() != startPiece->getColor() ) ) )
				{											/* check legal movement and moving to occupied space and
 															 * piece colors are different or moving to empty space */
					result = Legal;
				}
				break;

			case Bishop:
				if (abs(x) == abs(y) && (endPiece == nullptr ||
						(endPiece != nullptr && endPiece->getColor() != startPiece->getColor() ) ) )
				{											/* check legal movement, and if moving to unoccupied space
															 * or space occupied by different colored piece */
					int i = x;
					int j = y;
					while (abs(i) != 1 && abs(j) != 1)    /* Checking for blocked path */
					{
						(x > 0) ? i-- : i++;
						(y > 0) ? j-- : j++;
						if (this->board[endRank - j][endFile - i] != nullptr)
						{
							return Illegal;
						}
					}

					result = Legal;
				}
				break;

			case Queen:
				if ( (abs(x) == abs(y) || (x == 0 && y != 0) || (x != 0 && y == 0) ) && (endPiece == nullptr ||
						(endPiece != nullptr && endPiece->getColor() != startPiece->getColor() ) ) )
				{											/* check legal movement, and if moving to unoccupied space
															 * or space occupied by different colored piece */
					int i = x;
					int j = y;
					if (x != 0 && y == 0)
					{
						while (abs(i) != 1)    /* Checking for blocked path when moving horizontally */
						{
							(x > 0) ? i-- : i++;
							if (this->board[endRank][endFile - i] != nullptr)
							{
								return Illegal;
							}
						}
					}
					else if (x == 0 && y != 0)
					{
						while (abs(j) != 1)    /* Checking for blocked path when moving vertically */
						{
							(y > 0) ? j-- : j++;
							if (this->board[endRank - j][endFile] != nullptr)
							{
								return Illegal;
							}
						}
					}
					else if (abs(x) == abs(y))
					{
						while (abs(i) != 1 && abs(j) != 1)    /* Checking for blocked path when moving diagonally */
						{
							(x > 0) ? i-- : i++;
							(y > 0) ? j-- : j++;
							if (this->board[endRank - j][endFile - i] != nullptr)
							{
								return Illegal;
							}
						}
					}

					result = Legal;
				}
				break;

			case King:
				this->isInCheck(startRank, startFile, startPiece->getColor());

				if (x <= 1 && x >= -1 && y <= 1 && y >= -1 && ( endPiece == nullptr ||
						( endPiece != nullptr && endPiece->getColor() != startPiece->getColor() ) ) &&
						!this->isInCheck(endRank, endFile, startPiece->getColor() ) )
				{									/* check legal conditions, and moving piece to unoccupied space or
 													 * moving a piece to occupied space with different colored piece and
 													 * king will not be in check */
						result = Legal;
				}
				else if (castling[startPiece->getColor()])
				{
					bool whiteCastle = startPiece->getColor() == White && endRank == 0 &&
						( (endFile == 6 && !this->rookMoved[White][Kingside] && this->getPiece(0, 5) == nullptr &&
						this->getPiece(0, 6) == nullptr && !this->isInCheck(0, 5, White) && !this->isInCheck(0, 6, White) ) ||
						(endFile == 2 && !this->rookMoved[White][Queenside] && this->getPiece(0, 3) == nullptr &&
						this->getPiece(0, 2) == nullptr && this->getPiece(0, 1) == nullptr && !this->isInCheck(0, 3, White) &&
						!this->isInCheck(0, 2, White) ) );
					bool castleBlack = startPiece->getColor() == Black && endRank == 7 &&
						( (endFile == 6 && !this->rookMoved[Black][Kingside] && this->getPiece(7, 5) == nullptr &&
						this->getPiece(7, 6) == nullptr && !this->isInCheck(7, 5, Black) && !this->isInCheck(7, 6, Black) ) ||
						(endFile == 2 && !this->rookMoved[Black][Queenside] && this->getPiece(7, 3) == nullptr &&
						this->getPiece(7, 2) == nullptr && this->getPiece(7, 1) == nullptr && !this->isInCheck(7, 3, Black) &&
						!this->isInCheck(7, 2, Black) ) );

					if ( (whiteCastle || castleBlack) && !this->check[startPiece->getColor()] )
					{
						return Castling;
					}
				}
				break;
			default:
				break;
		} /* end switch */

		if (result == Legal)
		{
			if (endPiece != nullptr && endPiece->getType() == King)
			{
				result = Check;
			}
			return result;
		}

		return Illegal;
	}

	bool Board::isInCheck(int rank, int file, PieceColor color)
	{
		Result result;
		int decide;
		int x = 0;	/* distance between positions on x-axis */
		int y = 0;
		bool isCheck = false, isOtherKingNear = false;
		Piece * left = nullptr, * right = nullptr;
		Piece * piece = this->getPiece(rank, file), * sample = nullptr;
		PieceColor oppositeColor = (color == Black) ? White : Black;
		Board temp(this, false);

		if (piece != nullptr && piece->getType() == King && piece->getColor() == color)
		{
			this->check[color] = false;
		}
		else if (piece != nullptr && piece->getType() != King)
		{
			delete temp.board[rank][file];
			temp.board[rank][file] = nullptr;
		}

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				sample = this->getPiece(i, j);

				if (sample != nullptr && sample->getColor() != color)	// Checking for enemy pieces
				{
					if (sample->getType() == King)
					{
						x = file - j;
						y = rank - i;

						isOtherKingNear = abs(kingLocations[oppositeColor][0] - kingLocations[color][0]) <= 1 &&
							abs(kingLocations[oppositeColor][1] - kingLocations[color][1]);

						if (abs(x) <= 1 && abs(y) <= 1 && (!this->check[sample->getColor()] || isOtherKingNear) )
						{									/* Checking the other king separately, else there's infinite
 															 * recursion */
							result = Legal;
						}
						else
						{
							result = Illegal;
						}
					}
					else
					{
						result = temp.checkLegalMove(i, j, rank, file);
					}

					/* Piece can capture potential move space or check king at current space,
					 * and is not of the same color.
 					 * A pawn threat would show up as Check since it can't capture head-on (a Legal move)
					 */
					if (result == Check || (result == Legal && sample->getType() != Pawn ) )
					{
//						std::cerr << fileToChar[file] << rank + 1 << ((result == Check) ? " in check because of " :
//									  " will be in check because of ") << fileToChar[j] << i + 1 << std::endl;

						if (piece != nullptr && piece->getType() == King && piece->getColor() == color)
						{
							this->check[color] = true;
						}
						isCheck = true;
					}
				}
			}
		}

		decide = (color == Black) ? rank - 1 : rank + 1;
		left = this->getPiece(decide, file - 1);
		right = this->getPiece(decide, file + 1);

		if (left != nullptr && left->getColor() != color && left->getType() == Pawn)
		{
		/*	std::cerr << fileToChar[file] << rank + 1 << " pawn check because of " << fileToChar[file - 1]
					  << decide + 1 << std::endl; */

			if (piece != nullptr && piece->getType() == King && piece->getColor() == color)
			{
				this->check[color] = true;
			}
			isCheck = true;
		}
		if (right != nullptr && right->getColor() != color && right->getType() == Pawn)
		{
			/* std::cerr << fileToChar[file] << rank + 1 << " pawn check because of " << fileToChar[file + 1]
					  << decide + 1 << std::endl; */

			if (piece != nullptr && piece->getType() == King && piece->getColor() == color)
			{
				this->check[color] = true;
			}
			isCheck = true;
		}

		return isCheck;
	}

	bool Board::isCheckmate(PieceColor color)
	{
		int rank = this->kingLocations[color][0], file = this->kingLocations[color][1];
		Piece * piece = nullptr;

		// Checking if king itself is in check, and if so, can the enemy piece be captured in the next move
		if ( this->isInCheck(kingLocations[color][0], kingLocations[color][1], color) )
		{
			for (int i = 0; i < 8; ++i)
			{
				for (int j = 0; j < 8; ++j)
				{
					if (this->checkLegalMove(i, j, rank, file) == Check &&
						(this->isPieceCapturable(i, j, color) ||
						this->canPathBeBlocked(i, j, rank, file, color) ) )
					{
						return false;
					}
				}
			}
		}
		else
		{
			return false;
		}

		Board temp(this, false);
		delete temp.board[kingLocations[color][0]][kingLocations[color][1]];
		temp.board[kingLocations[color][0]][kingLocations[color][1]] = nullptr;

		if (rank + 1 < 8)
		{
			if (file + 1 < 8 && !temp.isInCheck(rank + 1, file + 1, color) &&
				( ( piece = this->getPiece(rank + 1, file + 1) ) == nullptr ||
				piece->getColor() != color) )
			{
				return false;
			}
			if (file - 1 >= 0 && !temp.isInCheck(rank + 1, file - 1, color) &&
				( ( piece = this->getPiece(rank + 1, file - 1) ) == nullptr ||
				piece->getColor() != color) )
			{
				return false;
			}
			if (!temp.isInCheck(rank + 1, file, color) &&
				( ( piece = this->getPiece(rank + 1, file) ) == nullptr ||
				piece->getColor() != color) )
			{
				return false;
			}
		}

		if (rank - 1 >= 0)
		{
			if (file + 1 < 8 && !temp.isInCheck(rank - 1, file + 1, color) &&
				( ( piece = this->getPiece(rank - 1, file + 1) ) == nullptr ||
				piece->getColor() != color) )
			{
				return false;
			}
			if (file - 1 >= 0 && !temp.isInCheck(rank - 1, file - 1, color) &&
				( ( piece = this->getPiece(rank - 1, file - 1) ) == nullptr ||
				piece->getColor() != color) )
			{
				return false;
			}
			if (!temp.isInCheck(rank - 1, file, color) &&
				( ( piece = this->getPiece(rank - 1, file) ) == nullptr ||
				piece->getColor() != color) )
			{
				return false;
			}
		}

		if ( file + 1 < 8 && !temp.isInCheck(rank, file + 1, color) &&
			( ( piece = this->getPiece(rank, file + 1) ) == nullptr ||
			piece->getColor() != color) )
		{
			return false;
		}

		if (file - 1 >= 0 && !temp.isInCheck(rank, file - 1, color) &&
			( ( piece = this->getPiece(rank, file - 1) ) == nullptr ||
			piece->getColor() != color) )
		{
			return false;
		}

		return true;
	}

	bool Board::isStalemate(PieceColor color)
	{
		Piece * piece = nullptr;

		if ( this->isInCheck(kingLocations[color][0], kingLocations[color][1], color) )
		{
			return false;
		}

		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if ( ( piece = this->getPiece(i, j) ) != nullptr && piece->getColor() == color)
				{
					for (int k = 0; k < 8; ++k)
					{
						for (int m = 0; m < 8; ++m)
						{
							if ( this->checkLegalMove(i, j, k, m) != Illegal &&
								( piece->getType() != King ||
								( piece->getType() == King &&
								!this->isInCheck(k, m, color) ) ) )
							{
								return false;
							}
						}
					}
				}
			}
		}

		return true;
	}

	bool Board::isPieceCapturable(int rank, int file, PieceColor color)
	{
		Result result = Illegal;
		int kr = this->kingLocations[color][0], kf = this->kingLocations[color][1];

		for (int k = 0; k < 8; ++k)
		{
			for (int m = 0; m < 8; ++m)
			{
				if ( ( (result = this->checkLegalMove(k, m, rank, file) ) == Legal || result == EnPassant) &&
					( (k != kr || m != kf) || (k == kr && m == kf && !this->isInCheck(rank, file, color) ) ) )
				{
					return true;
				}
			}
		}

		return false;
	}

	Board * Board::getPreviousBoard()
	{
		return this->previousBoard;
	}

	bool Board::getCheck(PieceColor color)
	{
		return this->isInCheck(kingLocations[color][0], kingLocations[color][1], color);
	}

	bool Board::canCastle(PieceColor color)
	{
		return this->castling[color];
	}

	bool Board::canPathBeBlocked(int startRank, int startFile, int endRank, int endFile, PieceColor color)
	{
		int x = endFile - startFile, y = endRank - startRank, a = abs(y), b = abs(x);

		if (this->getPiece(startRank, startFile)->getType() != Knight)
		{
			while (a > 1 || b > 1)    /* Checking for blocked path */
			{
				if (b > 0)
				{
					(x > 0) ? x-- : x++;
					--b;
				}
				if (a > 0)
				{
					(y > 0) ? y-- : y++;
					--a;
				}

				for (int i = 0; i < 8; ++i)
				{
					for (int j = 0; j < 8; ++j)
					{
						if (this->checkLegalMove(i, j, startRank + y, startFile + x) == Legal &&
							this->getPiece(i, j)->getType() != King &&
							this->getPiece(i, j)->getColor() == color)
						{
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	bool Board::hasRookMoved(PieceColor color, Direction side)
	{
		return this->rookMoved[color][side];
	}

	int Board::getKingLocations(PieceColor color, int place)
	{
		return this->kingLocations[color][place];
	}
}
