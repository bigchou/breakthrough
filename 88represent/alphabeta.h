#ifndef alphabeta_h
#define alphabeta_h
#include "board.h"
#define alphamax -99999 
#define betamax 99999
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


int eval(Board &b){
	int tmp = rand() % betamax;
	//printf("%d\n",tmp);
	return tmp;
}

int alphabetaNegamax(Board &b, Byte player,int alpha, int beta, int depth){
	if(b.gameOver())
		return 2*betamax;
	if(depth == 0)
		return eval(b);
	
	int v = alphamax;//negative infinity
	vector<int> possiblemoves;
	vector<int> invertedlist;
	b.possibleMoves(player,possiblemoves,invertedlist);
	//printf("depth = %d\n",depth);
	//printf("moves = %d\n",possiblemoves.size());
	
	int dest, src, tmp;
	bool capturable = false;
	for(int i=0;i<possiblemoves.size();++i){
		capturable = false;
		//printf("here\n");
		dest = possiblemoves[i];
		src = invertedlist[i];
		if(b.board[dest] == !player)
			capturable = true;
		b.setMove(dest,player);
		b.setMove(src,empty);
		//printf("i = %d  dest = %d   src = %d\n",i,dest, src);
		
		tmp = -1 * alphabetaNegamax(b,!player,-1 * beta,-1 * alpha,depth-1);
		if(capturable == true)
			b.setMove(dest,!player);
		else
			b.setMove(dest,empty);
		b.setMove(src,player);

		v = (v>tmp)?v:tmp;
		if(v>=beta)
			return v;
		alpha = (alpha>v)?alpha:v;
	}
	//printf("hello\n");
	return v;
}


int alphabeta(Board &b, Byte player, int alpha, int beta,int depth){
	if(b.gameOver()){
		//printf("here\n");
		Byte winner = b.getWinner();
		if(winner == white)
			return 2*betamax;
		else
			return 2*alphamax;
	}
	if(depth == 0)
		return eval(b);

	int v = alphamax;//negative infinity
	vector<int> possiblemoves;
	vector<int> invertedlist;
	b.possibleMoves(player,possiblemoves,invertedlist);
	int dest, src, tmp;
	bool capturable = false;
	for(int i=0;i<possiblemoves.size();++i){
		capturable = false;
		dest = possiblemoves[i];
		src = invertedlist[i];
		if(b.board[dest] == !player)
			capturable = true;
		b.setMove(dest,player);
		b.setMove(src,empty);
		tmp = alphabeta(b,!player,alpha,beta,depth-1);

		// uodo
		if(capturable == true)
			b.setMove(dest,!player);
		else
			b.setMove(dest,empty);
		b.setMove(src,player);

		// black player and white ai
		if(player == white){ // MAX(white) update alpha
			if(tmp > alpha)
				alpha = tmp;
			if(alpha >= beta)
				return beta;
		}else{// MIN(black) update beta
			if(tmp < beta)
				beta = tmp;
			if(alpha >= beta)
				return alpha;
		}
	}
	if(player == white){// MAX(white)
		return alpha;
	}else{// MIN(black)
		return beta;
	}
}


void bestMove(Board &b, Byte player){
	//b.showBoard();
	//int worstscore = alphamax;
	int bestscore;
	vector<int> possiblemoves;
	vector<int> invertedlist;
	int bestdest, bestsrc;
	b.possibleMoves(player,possiblemoves,invertedlist);
	int dest, src;
	int depth = 6;
	//printf("move = %d\n",possiblemoves.size());
	bool capturable = false;
	for(int i=0;i<possiblemoves.size();++i){
		capturable = false;
		dest = possiblemoves[i];
		src = invertedlist[i];
		if(b.board[dest] == !player)
			capturable = true;
		b.setMove(dest,player);
		b.setMove(src,empty);
		//printf("here\n");
		//printf("No\n");
		//int score = alphabetaNegamax(b,!player,alphamax,betamax,depth-1);
		int score = alphabeta(b,!player,alphamax,betamax,depth-1);
		printf("%d -> %d   score = %d\n",src,dest,score);
		if(capturable == true)
			b.setMove(dest,!player);
		else
			b.setMove(dest,empty);
		b.setMove(src,player);
		
		if(i == 0){
			bestscore = score;
			bestdest = dest;
			bestsrc = src;
		}else if(score > bestscore){
			bestscore = score;
			bestdest = dest;
			bestsrc = src;
		}
	}
	printf("AI's turn: %d %c --> %d %c\n",bestsrc/16,'A'+bestsrc%16,bestdest/16,'A'+bestdest%16);
	b.setMove(bestdest,player);
	b.setMove(bestsrc,empty);
	//printf("here\n");
}






#endif /* alphabeta_h */