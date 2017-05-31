
#include "board.h"
#include "alphabeta.h"
#include "utility.h"
#include "randmove.h"
#include "transposition_table.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

int main(){
	srand((unsigned)time(NULL));
	Board b;
	/*TT t;
	U64 tmp = t.getZobristKey(b);
	int xxx = t.lookUp(tmp);
	if(xxx == -1)
		printf("not exist!\n");
	t.Insert(tmp,10);
	xxx = t.lookUp(tmp);
	if(xxx == -1)
		printf("not exist!\n");*/
	// case 1
	//b.board[96] = black;
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
	}
	b.board[64] = empty;
	b.board[80] = white;
	// 27.56 -> no ordering
	// 23.82 -> ordering*/
	// case 6
	/*b.board[71] = black;
	b.board[117] = empty;
	b.board[119] = empty;*/
	// 11.59 -> no ordering
	// 3.36 -> no ordering
	// case 7
	/*
	for(int i = 0;i<8;++i){
		b.board[16+i] = empty;
		b.board[32+i] = black;
		b.board[64+i] = white;
		b.board[96+i] = empty;
	}
	b.board[64] = empty;
	b.board[80] = white;*/
	/*for(int i = 0;i<8;++i){
		b.board[96+i] = empty;
		b.board[112+i] = empty;
	}
	b.board[64] = white;
	b.board[65] = white;*/
	while(!b.gameOver()){
		b.showBoard();
		Byte player = white; // AI = Black;
		// Read user input
		printf("Please input the source and destination (e.g. 6 B 5 C)...> ");
		int src = inputSrc();
		//printf("Please input the destination (e.g. 5 C)...> ");
		int dest = inputDest();
		if(!b.isMoveable(player,src,dest)){
			printf("Invalid move!\n");
			continue;
		}
		b.setMove(dest,player);
		b.setMove(src,empty);

		b.showBoard();
		if(b.gameOver())
			break;
		bestmove(b,!player);
		//b.showBoard();
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