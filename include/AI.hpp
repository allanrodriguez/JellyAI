/**
 * AI.hpp
 *
 * Created on 13 January, 2017
 *
 * @author	Allan Rodriguez <allanr1@uci.edu>
 */

#ifndef AI_HPP
#define AI_HPP

#include <iostream>
#include <climits>
#include <ctime>
#include <vector>
#include "Board.hpp"

namespace JellyAI
{
	class AI
	{
		private:
			int startRank;	/* Start and end squares of the move being made */
			int startFile;	//
			int endRank;	//
			int endFile;	//
			int value;		/* Heuristic value of the move in this node */
			int nodeCount;	/* Number of nodes in the tree (to be used only by root node) */
			int startPieceType;		/* Type of the start piece for Mario's code */
			bool isMoveSuccessful;	/* False if move is unsuccessful, mostly in check cases */
			PieceColor color;		/* Color of Jelly's pieces */
			AI * parent;			/* Parent of this node */
			AI * root;				/* Root node for maintaining node count */
			Board * try1board;		/* Board state after move above is made */
			std::vector<AI *> choice;	/* Vector holding a node's children */

			AI(int, int, int, int, AI *);
			void getChoices(int, int);
			int getPiecePoints(PieceColor);
			int evaluate(int);
			int alphaBeta(int, int, int);
			int getNodeCount();

		public:
			AI(Board *, PieceColor);
			~AI();
			int getStartPieceType();
			void getMove(int [2][2]);
			bool jellysMove();
	};
}

#endif // AI_HPP
