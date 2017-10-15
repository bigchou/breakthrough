#ifndef alphabeta_h
#define alphabeta_h
#include "board.h"
#include "transposition_table.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <string>
#include "utility.h"

#define PieceAttackValue 1000
int loc[120] = {100,100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,
					10,20,30,40,40,30,20,10,0,0,0,0,0,0,0,0,
					10,30,40,50,50,40,30,10,0,0,0,0,0,0,0,0,
					20,40,50,100,100,50,40,20,0,0,0,0,0,0,0,0,
					20,40,50,100,100,50,40,20,0,0,0,0,0,0,0,0,
					10,30,40,50,50,40,30,10,0,0,0,0,0,0,0,0,
					10,20,30,40,40,30,20,10,0,0,0,0,0,0,0,0,
					100,100,100,100,100,100,100,100};

double TimeLimited = 2.0;
#define TimeUpSignal 999999999

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
int maxdepth = 5;
int bestsrc = -1;
int bestdest = -1;

// Time
time_t start;
bool timeUp = false;

int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta);

double setTimeLimited(){
	double tmp;
	if(FILE *fp = fopen("timeControl.txt","r")){
		fscanf(fp,"%lf",&tmp);
		TimeLimited = tmp;
		printf("TimeLimited: %f\n",TimeLimited);
		TimeLimited = tmp * (double)CLOCKS_PER_SEC;
		fclose(fp);
		return tmp;
	}else{
		tmp = TimeLimited;
		printf("timeControl.txt not exists, so the TimeLimited is %f by default\n",TimeLimited);
		return tmp;
	}
}

int atk(Board &bb,int& dest,int& player,int& depth){
	int value = 0;
	if(player == black){
		// Black
		if(dest+15 < 120 && bb.board[dest+15] == !player)
			value -= PieceAttackValue + ((loc[dest] + depth)<<2);
		else if(dest+17 < 120 && bb.board[dest+17] == !player)
			value -= PieceAttackValue + ((loc[dest] + depth)<<2);
		else
			value += (PieceAttackValue + ((loc[dest] + depth)<<2));
		/*
		if(dest >= 96 && dest <= 103)
			if(dest == 96 && bb.board[113] == empty)
				value += TimeUpSignal;
			else if(dest == 103 && bb.board[118] == empty)
				value += TimeUpSignal;
			else if( bb.board[dest+17] == empty && bb.board[dest+15] == empty )
				value += TimeUpSignal;
		*/
	}else{
		// White
		if(dest-15 >= 0 && bb.board[dest-15] == !player){
			value -= PieceAttackValue + ((loc[dest] + depth)<<2);
		}
		else if(dest-17 >= 0 && bb.board[dest-17] == !player){
			value -= PieceAttackValue + ((loc[dest] + depth)<<2);
		}else{
			value += (PieceAttackValue + ((loc[dest] + depth)<<2));
		}
		/*
		if(dest >= 16 && dest <= 23)
			if(dest == 16 && bb.board[1] == empty)
				value += TimeUpSignal;
			else if(dest == 23 && bb.board[6] == empty)
				value += TimeUpSignal;
			else if( bb.board[dest-17] == empty && bb.board[dest-15] == empty )
				value += TimeUpSignal;
		*/
	}
	return value;
}

int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta){
	int value;
	// Terminal_test
	if(bb.gameOver()){
		Byte winner = bb.getWinner();
		if(winner == player){
			return 999999+(depth<<5);
		}
		else{
			return -999999-(depth<<5);
		}
	}else if(depth <= 0){
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

		//printf("...\n");
		if(  (double)(clock() - start)  > TimeLimited ){
			timeUp = true;
			value = TimeUpSignal;
		}else{
			// a-b with Negamax and Incremental Updates and quiescence search
			if(capturable == true){
				value = atk(bb,dest,player,depth);
				if(depth == 1){// do one more search if someone capture the piece at the bottom layer
					value -= abnegamax_incrupdate_quisc(bb,!player,depth,-1*beta+value,-1*alpha+value);
				}else{
					value -= abnegamax_incrupdate_quisc(bb,!player,depth-1,-1*beta+value,-1*alpha+value);
				}
			}else{
				int PieceMoveValue = loc[dest] - loc[src];
				value = PieceMoveValue - abnegamax_incrupdate_quisc(bb,!player,depth-1,-1*beta+PieceMoveValue,-1*alpha+PieceMoveValue);
			}
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
				bestsrc = src;
				bestdest = dest;
			}
		}
	}
	return alpha;
}


void bestmove(Board &bb, Byte player, vector<char> &recorder){
	maxdepth = 2;
	int score;
	int history_src, history_dest;
	do{
		start = clock();
		timeUp = false;
		score = abnegamax_incrupdate_quisc(bb,player,maxdepth,-99999999,99999999);
		// TimeUp
		if(timeUp == true){
			bestdest = history_dest;
			bestsrc = history_src;
			printf("time: %f\n",((double)(clock() - start) / (double)CLOCKS_PER_SEC));
			break;
		}else{
			// Record history
			history_dest = bestdest;
			history_src = bestsrc;
		}
		printf("maxdepth: %d\n",maxdepth);
		printf("time: %f\n",((double)(clock() - start) / (double)CLOCKS_PER_SEC));
		// Update
		maxdepth += 1;
	}while(  (double)(clock() - start)  < TimeLimited );
	printf("AI's turn:\n%d %c -> %d %c    score:%d\n",8-bestsrc/16,'A'+bestsrc%16,8-bestdest/16,'A'+bestdest%16,score);
	
	if(bb.board[bestdest] == !player){
		if(player == white)
			bb.black_piece--;
		else
			bb.white_piece--;
	}
	bb.setMove(bestdest,player);
	bb.setMove(bestsrc,empty);

	addRecord(recorder,bestsrc,bestdest);
}
#endif /* alphabeta_h */