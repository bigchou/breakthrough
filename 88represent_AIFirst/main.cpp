
#include "board.h"
#include "alphabeta.h"
#include "utility.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

int main(){
	srand((unsigned)time(NULL));
	Board b;
	// case 1
	//b.board[16] = white;
	// case 2
	//b.board[16] = white;
	// case 3
	/*b.board[80] = black;
	b.board[112] = empty;
	b.board[114] = empty;*/
	// case 4
	/*b.board[64] = black;
	b.board[112] = empty;
	b.board[114] = empty;*/

	// case 5
	/*
	for(int i = 0;i<8;++i){
		b.board[16+i] = empty;
		b.board[32+i] = black;
		b.board[64+i] = white;
		b.board[96+i] = empty;
	}*/
	// 27.56 -> no ordering
	// 23.82 -> ordering
	// case 6
	
	/*b.board[71] = black;
	b.board[117] = empty;
	b.board[119] = empty;*/
	// 11.59 -> no ordering
	// 3.36 -> no ordering
	int src;
	int dest;
	Byte player = white;
	bool err_flag = false;
	while(!b.gameOver()){
		if(!err_flag){
			b.showBoard();
			bestmove(b,player);
			b.showBoard();
		}
		if(b.gameOver())
			break;
		// Read user input
		printf("Please input the source and destination (e.g. 6 B 5 C)...> ");
		src = inputSrc();
		//printf("Please input the destination (e.g. 5 C)...> ");
		dest = inputDest();
		if(!b.isMoveable(!player,src,dest)){
			printf("Invalid move!\n");
			err_flag = true;
			continue;
		}else{
			err_flag = false;
		}
		b.setMove(dest,!player);
		b.setMove(src,empty);

		
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