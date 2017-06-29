
#include "board.h"
#include "alphabeta.h"
#include "utility.h"
#include "randmove.h"
#include "transposition_table.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string>





int main(){
	srand((unsigned)time(NULL));
	Board b;

	/*
	for(int i=16;i<24;++i)
		b.board[i] = empty;
	b.board[50] = white;
	b.board[32] = white;
	b.board[16] = white;
	b.board[1] = empty;
	b.board[0] = empty;*/
	/*
	int src;
	int dest;
	Byte player = white;
	bool err_flag = false;
	string recorder = "";
	while(!b.gameOver()){
		if(!err_flag){
			b.showBoard();
			bestmove(b,player,recorder);
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
	*/



	
	/*b.board[55] = black;
	b.board[96] = black;
	b.board[113] = empty;
	b.board[112] = empty;
	for(int i=16;i<23;++i)
		b.board[i] = empty;
	b.board[100] = black;
	b.board[80] = black;*/
	//string recorder = "";
	vector<char> recorder;
	vector<Board> history;
	history.push_back(b);

	while(!b.gameOver()){
		b.showBoard();
		Byte player = white; // AI = Black;
		// Read user input
		printf("Please input the source and destination (e.g. 2 B 3 C)...> ");
		int src = inputSrc();
		//printf("Please input the destination (e.g. 5 C)...> ");
		int dest = inputDest();
		//Undo
		if(src == -1000 && dest == -1000){
			printf("Undo\n");
			Undo(history,recorder,b);
			continue;
		}

		if(!b.isMoveable(player,src,dest)){
			printf("Invalid move!\n");
			continue;
		}
		// Decrease # of Piece
		if(b.board[dest] == !player){
			if(player == white)
				b.black_piece--;
			else
				b.white_piece--;
		}
		b.setMove(dest,player);
		b.setMove(src,empty);
		
		addRecord(recorder,src,dest);
		history.push_back(b);
		
		
		// AI's turn 
		b.showBoard();
		if(b.gameOver())
			break;
		bestmove(b,!player,recorder);
		history.push_back(b);
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
	
	// Store Result
	writeRecord(recorder);	

	printf("white_piece: %d\n",b.white_piece);
	printf("black_piece: %d\n",b.black_piece);


	int byebye;
	printf("Press \'E\' to continue...\n");
	scanf("%d",&byebye);
	return 0;
	
}