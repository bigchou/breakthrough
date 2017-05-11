
#include "board.h"
#include "alphabeta.h"
#include "utility.h"
#include <stdio.h>
#include <ctype.h>

int main(){


	Board b;
	//b.setMove(0,empty);
	//bestMove(b);
	//return 0;
	//b.black_piece = 0;


	while(!b.gameOver()){
		Byte player = black;
		char playermove[4];
		// Read user input
		
		printf("Please input the source (e.g. 6 B)...> ");
		//scanf(" %c %c %c %c",&playermove[0],&playermove[1],&playermove[2],&playermove[3]);
		int src = InputSrc();
		//int src = 16*(playermove[0]-'0') + (playermove[1]-'A');
		printf("Please input the destination (e.g. 5 C)...> ");
		int dest = InputDest();
		//int dest = 16*(playermove[2]-'0') + (playermove[3]-'A');
		b.setMove(dest,black);
		b.setMove(src,empty);
		//printf("%d\n",playermove);
		b.showBoard();
	}
	return 0;




	printf("Hello Wolrd\n");
	Byte M[120];

	for(int i=0;i<120;++i)
		M[i] = i;

	/*
	for(int i=0;i<120;++i){
		printf("%d   ",i);
		if(!(M[i] & 0x88)){
			printf("reachable\n");
		}else{
			printf("unreachable\n");
		}
	}*/

	Board board;
	board.setMove(1,black);
	board.showBoard();


	vector<int> possible_moves;
	board.possibleMoves(possible_moves);
	for(int i=0;i<possible_moves.size();++i)
		printf("%d ",possible_moves[i]);
	possible_moves.clear();

	board.setMove(119,empty);
	board.setMove(118,empty);
	board.setMove(103,white);
	board.showBoard();
	board.possibleMoves(possible_moves);
	for(int i=0;i<possible_moves.size();++i)
		printf("%d ",possible_moves[i]);
	return 0;
}