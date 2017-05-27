#ifndef alphabeta_h
#define alphabeta_h
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// bestmove
int bestsrc;
int bestdest;
int maxdepth = 6;

int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta);
int abnegamax(Board &bb,int a,int b,int ply,int player);

int getpiecevalue(Board &bb, int loc){
	int Value = 1300;
	return Value;
}


int eval(Board &bb,Byte player){
	int whiteleft = 0;
	int blackleft = 0;
	int Value = 0;
	for(int i=0;i<8;++i){
		int blackpieceOnCol = 0;
		int whitepieceOnCol = 0;
		for(int j=0;j<8;++j){
			if(bb.board[16*i+j] == empty)
				continue;

			if(bb.board[16*i+j] == white){
				++whiteleft;
				++whitepieceOnCol;
				// PieceValue (this is a function not implemented)
				if(i == 6){
					bool threatA = false;
					bool threatB = false;
					if(j > 0){
						threatA = (bb.board[112+j-1] == empty);
					}
					if(j < 7){
						threatB = (bb.board[112+j+1] == empty);
					}
					if(!(threatA && threatB))
						Value -= 10000;
				}else if(i == 0){
					Value -= 10;
				}
			}else{
				//Black
				++blackleft;
				++blackpieceOnCol;
				// PieceValue (not implemented)
				if(i == 1){
					bool threatA = false;
					bool threatB = false;
					if(j > 0){
						threatA = (bb.board[j-1] == empty);
					}
					if(j < 7){
						threatB = (bb.board[j+1] == empty);
					}
					if(!(threatA && threatB))
						Value += 10000;
				}else if(i == 7){
					Value += 10;
				}
			}
		}

		if(whitepieceOnCol == 0)
			Value += 20;
		if(blackpieceOnCol == 0)
			Value -= 20;
	}

	bool BlackWins = (whiteleft==0);
	bool WhiteWins = (blackleft==0);

	if(WhiteWins)
		Value-=999999;
	if(BlackWins)
		Value+=999999;

	// if this round is for black
	if((!player) == white)
		Value = -1 * Value;

	
	return Value;
}


int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta){
	int value;
	// Terminal_test
	
	if(bb.gameOver()){
		Byte winner = bb.getWinner();
		if(winner == white){
			return -999999-depth;
		}
		else{
			return 999999+depth;
		}
	}

	if(depth <= 0){
		return 0;
		//return eval(bb,player);
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
		Byte winner = bb.getWinner();
		if(winner == white){
			return 10000;
		}
		else{
			return -10000;
		}
	}
	if(ply == 0){
		return eval(bb,player);
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




void bestmove(Board &bb, Byte player){
	int score = abnegamax_incrupdate_quisc(bb,player,maxdepth,-99999999,99999999);
	printf("%d -> %d    score:%d \n",bestsrc,bestdest,score);
	bb.setMove(bestdest,player);
	bb.setMove(bestsrc,empty);
}








#endif /* alphabeta_h */