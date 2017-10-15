#include "board.h"
#include "utility.h"
#include "alphabeta.h"
#include "randmove.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <vector>


int main(){
	srand((unsigned)time(NULL));
	Board b;
	while(!b.gameOver()){
		b.showBoard();
		Byte player = white; // AI = Black;
		printf("Please input the source and destination (e.g. 6 B 5 C)...> ");
		int src = inputSrc();
		int dest = inputDest();
		bool capturable = false;
		if(player == white){
			if(  !(b.whitepawn & mask[src])){
				printf("Invalid Move\n");
				continue;
			}
			if(  !(b.whitepawn & mask[dest])){// if the destination is reachable
				U64 MoveStep = (((b.whitepawn & mask[src]) & ~file[fileh]) >> 7) | (((b.whitepawn & mask[src]) & ~file[filea]) >> 9)  | ((b.whitepawn & mask[src]) >> 8);
				U64 AttackStep = (((b.whitepawn & mask[src]) & ~file[fileh]) >> 7) | (((b.whitepawn & mask[src]) & ~file[filea]) >> 9);
				if(MoveStep & mask[dest]){
					if(b.emptypawn & mask[dest]){
						capturable = false;
						b.whitepawn ^= mask[src];
						b.whitepawn |= mask[dest];
						b.emptypawn = ~(b.whitepawn | b.blackpawn);
					}else if(AttackStep & mask[dest]){
						if(b.blackpawn & mask[dest]){
							capturable = true;
							b.blackpawn ^= mask[dest];
							b.whitepawn ^= mask[src];
							b.whitepawn |= mask[dest];
							b.emptypawn = ~(b.whitepawn | b.blackpawn);
						}
					}else{
						printf("Invalid Move\n");
						continue;
					}
				}else{
					printf("Invlaid Move\n");
					continue;
				}
			}
		}
		b.showBoard();
		// AI part
		if(b.gameOver())
			break;
		bestmove(b,!player);
	}
	// Result
	Byte winner = b.getWinner();
	if(winner == black)
		printf("Black wins\n");
	else if(winner == white)
		printf("White wins\n");
	else
		printf("Draw\n");
	return 0;
}