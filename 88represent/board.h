
#ifndef board_h
#define board_h
#include <stdio.h>
#include <vector>

using namespace std;
typedef unsigned char Byte;

enum{
	white = 1, black, empty
};

class Board{
public:
	Byte board[120] = {0};
	int moves = 0;
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
		/*for(int i=0;i<8;++i){
			for(int j=0;j<16;++j)
				printf("%d ",this->board[16*i+j]);
			printf("\n");
		}*/
		
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
	// Generate next possible board(s)
	void possibleMoves(vector<int>& tmp){
		// 0  1  2  3  4  ... 15
		// 16 17 18 19 20 ... 32
		for(int i=0;i<7;++i){// !!!
			for(int j=0;j<8;++j){
				if(this->board[16*i+j] == white){
					// the position is reachable and the elment in it is empty
					for(int k=15;k<18;++k)
						if(this->board[16*i+j+k] == empty && (!(16*i+j+k & 0x88)))
							tmp.push_back(16*i+j+k);
					// the position is reachable and the element in it is black
					for(int k=15;k<18;k+=2)
						if((!(16*i+j+k & 0x88)) && this->board[16*i+j+k] == black)
							tmp.push_back(16*i+j+k);
				}
				//if(this->board[16*i+j] == empty)
				//	tmp.push_back(16*i+j);
				/*
				if(!(this->board[16*i+j] & 0x88)){// be true if the position is reachable
					tmp.push_back(16*i+j);
				}*/
			}
		}
	}

};

#endif /* board_h */