#ifndef alphabeta_h
#define alphabeta_h
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// bestmove
#define maxdepth 6
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
	// ==================
	// Exec move on board
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
		U64 tmp_blackpawn = bb.blackpawn;
		U64 tmp_whitepawn = bb.whitepawn;
		U64 tmp_emptypawn = bb.emptypawn;
		if(player == black){
			if(  !(bb.blackpawn & mask[dest])   ){// reachable
				U64 MoveStep = (((bb.blackpawn & mask[src]) & ~file[fileh]) << 9) | (((bb.blackpawn & mask[src]) & ~file[filea]) << 7)  | ((bb.blackpawn & mask[src]) << 8);
				U64 AttackStep = (((bb.blackpawn & mask[src]) & ~file[fileh]) << 9) | (((bb.blackpawn & mask[src]) & ~file[filea]) << 7);
				if(MoveStep & mask[dest]){
					if(bb.emptypawn & mask[dest]){
						capturable = false;
						bb.blackpawn ^= mask[src];
						bb.blackpawn |= mask[dest];
						bb.emptypawn = ~(bb.whitepawn | bb.blackpawn);
					}else if(AttackStep & mask[dest]){
						if(bb.whitepawn & mask[dest]){
							//printf("capturable\n");
							capturable = true;
							bb.whitepawn ^= mask[dest];
							bb.blackpawn ^= mask[src];
							bb.blackpawn |= mask[dest];
							bb.emptypawn = ~(bb.whitepawn | bb.blackpawn);
						}
					}
				}
				
			}
		}else{
			// Black
			if(  !(bb.whitepawn & mask[dest])   ){// reachable
				U64 MoveStep = (((bb.whitepawn & mask[src]) & ~file[fileh]) >> 7) | (((bb.whitepawn & mask[src]) & ~file[filea]) >> 9)  | ((bb.whitepawn & mask[src]) >> 8);
				U64 AttackStep = (((bb.whitepawn & mask[src]) & ~file[fileh]) >> 7) | (((bb.whitepawn & mask[src]) & ~file[filea]) >> 9);
				if(MoveStep & mask[dest]){
					if(bb.emptypawn & mask[dest]){
						capturable = false;
						bb.whitepawn ^= mask[src];
						bb.whitepawn |= mask[dest];
						bb.emptypawn = ~(bb.whitepawn | bb.blackpawn);
					}else if(AttackStep & mask[dest]){
						if(bb.blackpawn & mask[dest]){
							capturable = true;
							bb.blackpawn ^= mask[dest];
							bb.whitepawn ^= mask[src];
							bb.whitepawn |= mask[dest];
							bb.emptypawn = ~(bb.whitepawn | bb.blackpawn);
						}
					}
				}
				
			}
		}

		// a-b with Negamax and Incremental Updates and quiescence search
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

		// Undo Move
		bb.blackpawn = tmp_blackpawn;
		bb.whitepawn = tmp_whitepawn;
		bb.emptypawn = tmp_emptypawn;


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

void bestmove(Board &bb, Byte player){
	int score = abnegamax_incrupdate_quisc(bb,player,maxdepth,-99999999,99999999);
	printf("AI's turn:\n%d %c -> %d %c    score:%d\n",bestsrc/8,'A'+bestsrc%8,bestdest/8,'A'+bestdest%8,score);
	if(player == black){	
		U64 MoveStep = (((bb.blackpawn & mask[bestsrc]) & ~file[fileh]) << 9) | (((bb.blackpawn & mask[bestsrc]) & ~file[filea]) << 7)  | ((bb.blackpawn & mask[bestsrc]) << 8);
		U64 AttackStep = (((bb.blackpawn & mask[bestsrc]) & ~file[fileh]) << 9) | (((bb.blackpawn & mask[bestsrc]) & ~file[filea]) << 7);
		if(MoveStep & mask[bestdest]){
			if(bb.emptypawn & mask[bestdest]){
				bb.blackpawn ^= mask[bestsrc];
				bb.blackpawn |= mask[bestdest];
				bb.emptypawn = ~(bb.whitepawn | bb.blackpawn);
			}else if(AttackStep & mask[bestdest]){
				if(bb.whitepawn & mask[bestdest]){
					bb.whitepawn ^= mask[bestdest];
					bb.blackpawn ^= mask[bestsrc];
					bb.blackpawn |= mask[bestdest];
					bb.emptypawn = ~(bb.whitepawn | bb.blackpawn);
				}
			}
		}
	}
}

#endif /* alphabeta_h */