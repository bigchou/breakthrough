#ifndef alphabeta_h
#define alphabeta_h
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>

#define PieceMoveValue 50
#define PieceAttackValue 100

// the structure is usage for move-ordering
struct Pair {
	int eval_score;
    int src_score;
    int dest_score;
    bool operator<( const Pair& rhs ) const{
    	return eval_score > rhs.eval_score;
    }
};

// bestmove
int maxdepth = 7;
int bestsrc = -1;
int bestdest = -1;

int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta);

int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta){
	int value;
	// Terminal_test
	if(bb.gameOver()){
		Byte winner = bb.getWinner();
		if(winner == player)
			return 999999+depth;
		else
			return -999999-depth;
	}
	if(depth <= 0){
		return 0;
	}
	// =========
	vector<int> possiblemoves;
	vector<int> invertedlist;
	int dest, src;
	bb.possibleMoves(player,possiblemoves,invertedlist); // Generate Moves

	bool capturable = false;
	for(int i=0;i<possiblemoves.size();++i){
		// Exec move on board
		capturable = false;
		dest = possiblemoves[i];
		src = invertedlist[i];
		if(bb.board[dest] == !player){
			if(player == white)
				bb.black_piece--;
			else
				bb.white_piece--;
			capturable = true;
		}
		bb.setMove(dest,player);
		bb.setMove(src,empty);

		// a-b with Negamax and Incremental Updates and quiescence search
		if(capturable == true){
			value = PieceAttackValue + depth;// Capturing opponent piece earlier would be better
			if(depth == 1){// do one more search if someone capture the piece at the bottom layer
				value -= abnegamax_incrupdate_quisc(bb,!player,depth,-1*beta+value,-1*alpha+value);
			}else{
				value -= abnegamax_incrupdate_quisc(bb,!player,depth-1,-1*beta+value,-1*alpha+value);
			}
		}else{
			value  = PieceMoveValue - abnegamax_incrupdate_quisc(bb,!player,depth-1,-1*beta+PieceMoveValue,-1*alpha+PieceMoveValue);
		}

		// Undo move
		if(capturable == true){
			bb.setMove(dest,!player);
			if(player == white)
				bb.black_piece++;
			else
				bb.white_piece++;
		}else{
			bb.setMove(dest,empty);
		}
		bb.setMove(src,player);
		
		if(value >= beta){
			return value;
		}

		if(value > alpha){
			alpha = value;
			// Keep the best move on first layer
			if(depth == maxdepth){
				//printf("%d -> %d score:%d\n",src,dest,value);
				bestsrc = src;
				bestdest = dest;
			}
		}
	}
	return alpha;
}

void bestmove(Board &bb, Byte player){
	int maxdepth = 7;
	printf("# of layers...> ");
	scanf("%d",&maxdepth);
	time_t start = clock();
	int score = abnegamax_incrupdate_quisc(bb,player,maxdepth,-99999999,99999999);
	printf("time: %f\n",((double)(clock() - start) / (double)CLOCKS_PER_SEC));
	printf("AI's turn:\n%d %c -> %d %c\n",bestsrc/16,'A'+bestsrc%16,bestdest/16,'A'+bestdest%16);
	if(bb.board[bestdest] == !player){
		if(player == white)
			bb.black_piece--;
		else
			bb.white_piece--;
	}
	bb.setMove(bestdest,player);
	bb.setMove(bestsrc,empty);
}

#endif /* alphabeta_h */