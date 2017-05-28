
#include "board.h"
#include "alphabeta.h"
#include "utility.h"
#include "randmove.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

int main(){
	srand((unsigned)time(NULL));
	Board b;
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
	while(!b.gameOver()){
		b.showBoard();
		vector<int> possiblemoves;
		vector<int> invertedlist;
		Byte player = white; // AI = Black;
		// Read user input
		printf("Please input the source and destination (e.g. 6 B 5 C)...> ");
		int src = inputSrc();
		//printf("Please input the destination (e.g. 5 C)...> ");
		int dest = inputDest();
		b.possibleMoves(player,possiblemoves,invertedlist);
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