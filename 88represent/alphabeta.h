#ifndef alphabeta_h
#define alphabeta_h
#include "board.h"
#define alphamax -99999 
#define betamax 99999
#include <stdlib.h>
#include <time.h>
#include <stdio.h>



int abnegamax(Board &bb,int a,int b,int ply,int player);
int eval(Board &b){
	int tmp = rand() % betamax;
	tmp = 10;
	return tmp;
}



int bestsrc;
int bestdest;

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


void bestmove(Board &b, Byte player, Byte opponent){
	int ply = 3;
	int score = abnegamax(b,-10000,10000,3,player);
	printf("%d -> %d    score:%d \n",bestsrc,bestdest,score);
	b.setMove(bestdest,player);
	b.setMove(bestsrc,empty);
}








#endif /* alphabeta_h */