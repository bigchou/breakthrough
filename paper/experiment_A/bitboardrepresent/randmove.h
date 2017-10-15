#ifndef randmove_h
#define randmove_h
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void randMove(Board &b,Byte player){
	vector<int> possiblemoves;
	vector<int> invertedlist;
	b.possibleMoves(player,possiblemoves,invertedlist);
	int pos = rand() % (int)(possiblemoves.size());
	int src = invertedlist[pos];
	int dest = possiblemoves[pos];
	//b.setMove(dest,player);
	bool capturable = false;
	printf("src:%d dest:%d\n",src,dest);
	if(player == black){
		capturable = false;
		if(  !(b.blackpawn & mask[dest])   ){// reachable
			printf("Movable\n");
			U64 MoveStep = (((b.blackpawn & mask[src]) & ~file[7]) << 9) | (((b.blackpawn & mask[src]) & ~file[0]) << 7)  | ((b.blackpawn & mask[src]) << 8);
			//printf("%llu\n",MoveStep);
			U64 AttackStep = (((b.blackpawn & mask[src]) & ~file[7]) << 9) | (((b.blackpawn & mask[src]) & ~file[0]) << 7);
			//printf("%llu\n",AttackStep);
			printf("%llu\n",MoveStep);
			printf("%llu\n",MoveStep & mask[dest]);
			if(MoveStep & mask[dest]){//
				
				if(b.emptypawn & mask[dest]){
					printf("hi\n");
					//printf("Movable step\n");
					capturable = false;
					b.blackpawn ^= mask[src];
					b.blackpawn |= mask[dest];
					b.emptypawn = ~(b.whitepawn | b.blackpawn);
				}else if(AttackStep & mask[dest]){
					
					if(b.whitepawn & mask[dest]){
						printf("li ho\n");
						//printf("Attackable step\n");
						capturable = true;
						b.whitepawn ^= mask[dest];
						b.blackpawn ^= mask[src];
						b.blackpawn |= mask[dest];
						b.emptypawn = ~(b.whitepawn | b.blackpawn);
					}
				}
			}
			
		}
	}


	//b.setMove(src,empty);
}


#endif /* randmove_h */