#include "board.h"
#include "utility.h"
#include "alphabeta.h"
#include "randmove.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <vector>


int main(){
	// test start
	/*
	Board bb;
	vector<int> possiblemoves;
	vector<int> invertedlist;
	bb.possibleMoves(black,possiblemoves,invertedlist);
	for(int i=0;i<possiblemoves.size();++i){
		printf("src:%d -> dest:%d\n",invertedlist[i],possiblemoves[i]);
	}*/
	// test end
	//return 0;
	srand((unsigned)time(NULL));
	Board b;

	while(!b.gameOver()){
		//printf("????\n");
		b.showBoard();
		Byte player = white;
		// Read user input
		printf("Please input the source and destination (e.g. 6 B 5 C)...> ");
		int src = inputSrc();
		int dest = inputDest();
		
		//printf("src:%d dest:%d\n",src,dest);

		bool capturable = false;



		if(player == white){
			if(   !(b.whitepawn & mask[src])){
				printf("Invalid Move\n");
				continue;
			}
			if(  !(b.whitepawn & mask[dest])   ){// reachable
				//printf("Movable\n");
				U64 MoveStep = (((b.whitepawn & mask[src]) & ~file[7]) >> 7) | (((b.whitepawn & mask[src]) & ~file[0]) >> 9)  | ((b.whitepawn & mask[src]) >> 8);
				//printf("%llu\n",MoveStep);
				U64 AttackStep = (((b.whitepawn & mask[src]) & ~file[7]) >> 7) | (((b.whitepawn & mask[src]) & ~file[0]) >> 9);
				//printf("%llu\n",AttackStep);
				if(MoveStep & mask[dest]){
					if(b.emptypawn & mask[dest]){
						//printf("Movable step\n");
						capturable = false;
						b.whitepawn ^= mask[src];
						b.whitepawn |= mask[dest];
						b.emptypawn = ~(b.whitepawn | b.blackpawn);
					}else if(AttackStep & mask[dest]){
						if(b.blackpawn & mask[dest]){
							//printf("Attackable step\n");
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
		//randMove(b,!player);
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