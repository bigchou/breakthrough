
#ifndef board_h
#define board_h
#include <stdio.h>
typedef unsigned char Byte;

enum{
	white = 0, black, empty
};

class Board{
public:
	Byte board[120];
	int moves;
	Board(){// Constructor
		// Init White
		for(int i=0;i<2;++i)
			for(int j=0;j<8;++j)
				this->board[16*i+j] = white;
		// Init Empty
		for(int i=2;i<6;++i)
			for(int j=0;j<8;++j)
				this->board[16*i+j] = empty;
		// Init Black
		for(int i=6;i<8;++i)
			for(int j=0;j<8;++j)
				this->board[16*i+j] = black;
	}
	// Display Board
	void showBoard(){
		printf("  A B C D E F G H\n");
		for(int i=0;i<8;++i){
			printf("%d ",i);
			for(int j=0;j<8;++j){
				if(this->board[16*i+j] == white)
					printf("W ");
				else if(this->board[16*i+j] == empty)
					printf("_ ");
				else
					printf("B ");
			}
			printf("\n");
		}
	}
	// Modify the element in the board
	void setMove(int pos,Byte player){
		this->board[pos] = player;
	}

};

#endif /* board_h */