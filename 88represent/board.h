
#ifndef board_h
#define board_h
#include <stdio.h>
#include <vector>

using namespace std;
typedef unsigned char Byte;

enum{
	white = 0, black, empty
};

class Board{
public:
	Byte board[120] = {0};
	int moves = 0;
	int white_piece = 16;
	int black_piece = 16;
	Board(){// Constructor
		// Init Black
		for(int i=0;i<2;++i)
			for(int j=0;j<8;++j)
				this->board[16*i+j] = black;
		// Init Empty
		for(int i=2;i<6;++i)
			for(int j=0;j<8;++j)
				this->board[16*i+j] = empty;
		// Init White
		for(int i=6;i<8;++i)
			for(int j=0;j<8;++j)
				this->board[16*i+j] = white;
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
		printf("\n");
	}
	// Modify the element in the board
	void setMove(int pos,Byte player){
		this->board[pos] = player;
	}
	// Generate next possible board(s)
	void possibleMoves(Byte player,vector<int>& tmp, vector<int>& invertedlist){
		// tmp is a destination vector
		// invertedlist is a source vector
		// 0  1  2  3  4  ... 15
		// 16 17 18 19 20 ... 32
		for(int i=0;i<8;++i){
			for(int j=0;j<8;++j){
				if(this->board[16*i+j] == player){
					int t = 0;
					// the position is reachable and the elment in it is empty
					for(int k=15;k<18;++k){
						t = (player==black) ? k : -1*k;
						if((!(16*i+j+t & 0x88)) && this->board[16*i+j+t] == empty){
							tmp.push_back(16*i+j+t);
							invertedlist.push_back(16*i+j);
						}
					}
					// the position is reachable and the element in it is opponent piece
					for(int k=15;k<18;k+=2){
						t = (player==black) ? k : -1*k;
						if((!(16*i+j+t & 0x88)) && (this->board[16*i+j+t] == (!player))){
							tmp.push_back(16*i+j+t);
							invertedlist.push_back(16*i+j);
						}
					}
				}
			}
		}
	}

	// Whether the game is over or not
	bool gameOver(){
		if(black_piece == 0 || white_piece == 0)
			return true;
		for(int i=0;i<8;++i){
			if(this->board[i] == white)
				return true;
			if(this->board[i+112] == black)
				return true;
		}
		return false;
	}

	// whether the move is valid or not
	bool isMoveable(Byte player, int src,int dest){
		// Check source
		if(this->board[src] == !player || this->board[src] == empty)
			return false;
		if(!(src & 0x88) && !(dest & 0x88)){
			int t = (player == black) ? 1 : -1;

			if(src+t*15 == dest || src+t*17 == dest){
				//diagonal
				if( this->board[dest] == player){
					return false;
				}else{
					return true;
				}
			}else if(src+t*16 == dest){
				//straight
				if( this->board[dest] == empty){
					return true;
				}else{
					return false;
				}
			}else{
				return false;
			}
		}else{
			return false;
		}
	}

	// get winner of this game
	Byte getWinner(){
		if(this->white_piece == 0)
			return black;
		if(this->black_piece == 0)
			return white;
		for(int i=0;i<8;++i){
			if(this->board[i] == white)
				return white;
			if(this->board[i+112] == black)
				return black;
		}
		return empty;
	}

};

#endif /* board_h */