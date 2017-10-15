// AI = Black;
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
	setTimeLimited();
	vector<char> recorder;
	vector<Board> history;
	history.push_back(b);
	/*b.board[84]= black;
	b.board[97] = empty;
	b.board[98] = empty;
	b.board[99] = empty;
	b.board[114] = empty;
	b.board[115] = empty;
	b.board[116] = empty;*/



	/*for(int i=96;i<118;++i)
		b.board[i] = empty;
	b.board[96] = black;*/


	/*b.board[52] = black;
	b.board[69] = white;
	b.board[86] = white;*/

	//b.board[96]= black;


	/*b.board[48] = black;
	b.board[113] = empty;
	b.board[115] = empty;
	b.board[96] = empty;
	b.board[97] = empty;
	b.board[98] = empty;
	b.board[114] = empty;*/
	//b.board[112] = empty;


	/*b.board[98] = empty;
	b.board[99] = empty;
	b.board[115] = empty;
	b.board[116] = empty;
	b.board[101] = empty;
	b.board[83] = black;
	b.board[68] = black;
	b.board[70] = white;
	b.board[86] = white;
	b.board[119] = empty;*/

	/*b.board[98] = empty;
	b.board[99] = empty;
	b.board[101] = empty;
	b.board[102] = empty;
	b.board[103] = empty;
	b.board[114] = empty;
	b.board[115] = empty;
	b.board[116] = empty;
	b.board[68] = black;
	b.board[53] = black;
	b.board[84] = white;
	b.board[83] = white;
	for(int i=0;i<8;++i)
		b.board[16+i] = empty;*/

	while(!b.gameOver()){
		b.showBoard();
		Byte player = white; // AI = Black;
		// Read user input
		printf("Please input the source and destination (e.g. 2 B 3 C)...> ");
		int src = inputSrc();
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
	}
	// Result
	Byte winner = b.getWinner();
	if(winner == black)
		printf("Black wins\n");
	else
		printf("White wins\n");
	// Store Result
	writeRecord(recorder);
	printf("white_piece: %d\n",b.white_piece);
	printf("black_piece: %d\n",b.black_piece);
	// END
	int byebye;
	printf("Press \'E\' to continue...\n");
	scanf("%d",&byebye);
	return 0;
	
}