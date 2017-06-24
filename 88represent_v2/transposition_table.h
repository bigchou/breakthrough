#ifndef transposition_table_h
#define transposition_table_h
#include <map>
typedef unsigned long long U64;

class TT{
public:
	U64 piece_pattern[2][64];
	map<U64, int> ttable;

	TT(){
		U64 tmp = 1;
		// Init
		for(int i=0;i<2;++i)
			for(int j=0;j<64;++j)
				this->piece_pattern[i][j] = tmp++;
	}

	// Get Zobrist Hashing Key
	U64 getZobristKey(Board& bb){
		U64 val;
		for(int i=0;i<8;++i){
			for(int j=0;j<8;++j){
				if(bb.board[8*i+j] == white)
					val ^= this->piece_pattern[white][8*i+j];
				else if(bb.board[16*i+j] == empty)
					val ^= this->piece_pattern[black][8*i+j];
			}
		}
		return val;
	}

	// Do table lookup to get evaluation score (Return -1 if the element not exists)
	int lookUp(U64 boardhashkey){
		map<U64,int>::iterator iter;
		iter = this->ttable.find(boardhashkey);
		if(iter != this->ttable.end()){
			// If the element exists...
			return iter->second;
		}
		return -1;
	}

	// add new element into table if the target not exists
	void Insert(U64 boardhashkey,int eval_score){
		this->ttable[boardhashkey] = eval_score;
	}

};


#endif /* transposition_table_h */