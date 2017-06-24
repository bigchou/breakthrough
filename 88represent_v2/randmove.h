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
	b.setMove(dest,player);
	b.setMove(src,empty);
}

#endif /* randmove_h */