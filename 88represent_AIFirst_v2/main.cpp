
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
	
	int src;
	int dest;
	
	bool err_flag = false;
	//string recorder = "";
	
	vector<char> recorder;
	vector<Board> history;

	history.push_back(b);

	while(!b.gameOver()){
		Byte player = black;
		if(!err_flag){
			b.showBoard();
			bestmove(b,!player,recorder);
			b.showBoard();
			history.push_back(b);
		}else{
			//printf("start\n");
			b.showBoard();
			//printf("end\n");
		}
		if(b.gameOver())
			break;
		// Read user input
		printf("Please input the source and destination (e.g. 6 B 5 C)...> ");
		src = inputSrc();
		//printf("Please input the destination (e.g. 5 C)...> ");
		dest = inputDest();
		//Undo
		if(src == -1000 && dest == -1000){
			printf("Undo\n");
			err_flag = true;
			Undo(history,recorder,b);
			continue;
		}else{
			err_flag = false;
		}

		if(!b.isMoveable(player,src,dest)){
			printf("Invalid move!\n");
			err_flag = true;
			continue;
		}else{
			err_flag = false;
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