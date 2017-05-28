#ifndef board_h
#define board_h
#include <stdio.h>
#include <vector>

using namespace std;

typedef unsigned long long U64;
typedef char Byte;

enum{
	white = 0, black, empty
};

enum{
	rank0 = 0, rank1, rank2, rank3, rank4, rank5, rank6, rank7
};

enum{
	filea = 0, fileb, filec, filed, filee, filef, fileg, fileh
};

U64 mask[64];
U64 rank[8] = {0x00000000000000FF,
	0x000000000000FF00,
	0x0000000000FF0000,
	0x00000000FF000000,
	0x000000FF00000000,
	0x0000FF0000000000,
	0x00FF000000000000,
	0xFF00000000000000
};

U64 file[8] = {
	0x0101010101010101,
	0x0202020202020202,
	0x0404040404040404,
	0x0808080808080808,
	0x1010101010101010,
	0x2020202020202020,
	0x4040404040404040,
	0x8080808080808080
};


class Board{
public:
	U64 whitepawn = 0xFFFF000000000000;
	U64 blackpawn = 0x000000000000FFFF;
	U64 emptypawn = 0x0000FFFFFFFF0000;
	int white_piece = 16;
	int black_piece = 16;
	Board(){
		whitepawn = 0x8000000000000000;
		blackpawn = 0x0000000000000001;
		whitepawn = 0xFFFF000000000000;
		blackpawn = 0x000000000000FFFF;
		emptypawn = 0x0000FFFFFFFF0000;

		
		U64 tmp = 0x0000000000000001;
		for(int i=0;i<64;++i){
			mask[i] = (tmp << i);
			//printf("%llu\n",mask[i]);
		}
	}
	// Display Board
	void showBoard(){
		printf("  A B C D E F G H\n");
		int val = 0;
		for(int i=0;i<8;++i){
			printf("%d ",i);
			val = 8*i;
			for(int j=0;j<8;++j){
				if(this->blackpawn & mask[val])
					printf("B ");
				//if(this->emptypawn & mask[val])
				//	printf("_ ");
				else if(this->whitepawn & mask[val])
					printf("W ");
				else
					printf("_ ");
				val += 1;
			}
			printf("\n");
		}
	}


	// Whether the game is over or not
	bool gameOver(){
		if(whitepawn & 0x00000000000000FF)
			return true;
		if(blackpawn & 0xFF00000000000000)
			return true;
		if(black_piece == 0 || white_piece == 0)
			return true;
		return false;
	}

	// get winner of this game
	Byte getWinner(){
		if(this->white_piece == 0)
			return black;
		if(this->black_piece == 0)
			return white;
		
		if(whitepawn & 0x00000000000000FF)
			return white;
		if(blackpawn & 0xFF00000000000000)
			return black;
		return empty;
	}


};

#endif /* board_h */