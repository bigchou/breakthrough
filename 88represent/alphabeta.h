#ifndef alphabeta_h
#define alphabeta_h
#include "board.h"
#define alphamax -99999 
#define betamax 99999
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// bestmove
int bestsrc;
int bestdest;
int maxdepth = 5;

int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta);
int abnegamax(Board &bb,int a,int b,int ply,int player);
int eval(Board &b){
	int tmp = rand() % betamax;
	return tmp;
}


int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta){
	int value;
	// Terminal_test
	if(depth <= 0){
		return 0;
	}
	if(bb.gameOver()){
		Byte winner = bb.getWinner();
		if(winner == white){
			return -999999-depth;
		}
		else{
			return 99999+depth;
		}
	}

	// =========
	vector<int> possiblemoves;
	vector<int> invertedlist;
	int dest, src;
	bb.possibleMoves(player,possiblemoves,invertedlist); // Generate Moves
	bool capturable = false;
	for(int i=0;i<possiblemoves.size();++i){
		// Exec move on b
		capturable = false;
		dest = possiblemoves[i];
		src = invertedlist[i];
		if(bb.board[dest] == !player)
			capturable = true;
		bb.setMove(dest,player);
		bb.setMove(src,empty);

		if(capturable == true){
			value = 100 + depth;
			if(depth == 1){
				value -= abnegamax_incrupdate_quisc(bb,!player,depth,-1*beta+value,-1*alpha+value);
			}else{
				value -= abnegamax_incrupdate_quisc(bb,!player,depth-1,-1*beta+value,-1*alpha+value);
			}
		}else{
			value  = 50 - abnegamax_incrupdate_quisc(bb,!player,depth-1,-1*beta+50,-1*alpha+50);
		}

		// Undo move
		if(capturable == true)
			bb.setMove(dest,!player);
		else
			bb.setMove(dest,empty);
		bb.setMove(src,player);

		if(value >= beta){
			return value;
		}

		if(value > alpha){
			alpha = value;
			if(depth == maxdepth){
				bestsrc = src;
				bestdest = dest;
			}
		}
	}

	return alpha;
}









int abnegamax(Board &bb,int a,int b,int ply,int player){
	// Terminal_test
	if(bb.gameOver()){
		printf("here\n");
		Byte winner = bb.getWinner();
		if(winner == white){
			return 10000;
		}
		else{
			return -10000;
		}
	}
	if(ply == 0){
		return eval(bb);
	}

	// =========
	vector<int> possiblemoves;
	vector<int> invertedlist;
	int dest, src;
	bb.possibleMoves(player,possiblemoves,invertedlist);
	bool capturable = false;
	int v = -10000;


	for(int i=0;i<possiblemoves.size();++i){
		// Exec move on b
		capturable = false;
		dest = possiblemoves[i];
		src = invertedlist[i];
		if(bb.board[dest] == !player)
			capturable = true;
		bb.setMove(dest,player);
		bb.setMove(src,empty);


		// AlphaBeta
		int tmp = -1 * abnegamax(bb, -1*b ,-1*a,ply-1,!player);
		v = (tmp>v)?tmp:v;

		// Undo move
		if(capturable == true)
			bb.setMove(dest,!player);
		else
			bb.setMove(dest,empty);
		bb.setMove(src,player);
		
		if(v >= b){
			return v;
		}
		if(v > a){
			a = v;
			bestsrc = src;
			bestdest = dest;
		}
	}
	return v;
}




void bestmove(Board &bb, Byte player, Byte opponent){
	
	int score = abnegamax_incrupdate_quisc(bb,player,maxdepth,-99999999,99999999);
	printf("%d -> %d    score:%d \n",bestsrc,bestdest,score);
	bb.setMove(bestdest,player);
	bb.setMove(bestsrc,empty);
}








#endif /* alphabeta_h */