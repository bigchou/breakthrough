#ifndef alphabeta_h
#define alphabeta_h
#include "board.h"
#include "transposition_table.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <string>
#include "utility.h"


#define WinValue 100000
#define PieceAlmostWinValue 10000
#define PieceValue 2000
#define PieceAttackValue 1000
#define PieceConnectionHValue 100
#define PieceConnectionVValue 50
#define PieceHomeGroundValue 100
#define WhiteRank3Value 50
#define WhiteRank2Value 150
#define WhiteRank1Value 450
#define BlackRank4Value 50
#define BlackRank5Value 150
#define BlackRank6Value 450

int loctable[120] = {100,100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,
					10,20,30,40,40,30,20,10,0,0,0,0,0,0,0,0,
					10,30,40,50,50,40,30,10,0,0,0,0,0,0,0,0,
					20,40,50,100,100,50,40,20,0,0,0,0,0,0,0,0,
					20,40,50,100,100,50,40,20,0,0,0,0,0,0,0,0,
					10,30,40,50,50,40,30,10,0,0,0,0,0,0,0,0,
					10,20,30,40,40,30,20,10,0,0,0,0,0,0,0,0,
					100,100,100,100,100,100,100,100};

double TimeLimited = 2.0;
#define TimeUpSignal 999999999

// the structure is usage for move-ordering
struct Pair {
	int eval_score;
    int src_score;
    int dest_score;
    bool operator<( const Pair& rhs ) const{
    	return eval_score > rhs.eval_score;
    }
};

// bestmove
int maxdepth = 5;
int bestsrc = -1;
int bestdest = -1;

// Time
time_t start;
bool timeUp = false;


int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta);

double setTimeLimited(){
	double tmp;
	if(FILE *fp = fopen("timeControl.txt","r")){
		fscanf(fp,"%lf",&tmp);
		TimeLimited = tmp;
		printf("TimeLimited: %f\n",TimeLimited);
		TimeLimited = tmp * (double)CLOCKS_PER_SEC;
		fclose(fp);
		return tmp;
	}else{
		tmp = TimeLimited;
		printf("timeControl.txt not exists, so the TimeLimited is %f by default\n",TimeLimited);
		TimeLimited *= (double)CLOCKS_PER_SEC;
		return tmp;
	}
}


int getPieceValue(Board &bb, int loc,int i,int j){
	int Value = PieceValue;
	// Evaluation Connection
	bool ConnectedH = false;
	bool ConnectedV = false;
	if(j > 0)
		if(bb.board[loc-1] == bb.board[loc])
			ConnectedH = true;
	if(j < 7)
		if(bb.board[loc+1] == bb.board[loc])
			ConnectedH = true;
	if(i > 0)
		if(bb.board[loc-16] == bb.board[loc])
			ConnectedV = true;
	if(i < 7)
		if(bb.board[loc+16] == bb.board[loc])
			ConnectedV = true;
	// Add Connection Value
	if(ConnectedV)
		Value += PieceConnectionVValue;
	if(ConnectedH)
		Value += PieceConnectionHValue;
	Value += loctable[loc];
	return Value;
}


int eval(Board &bb,Byte player){
	int Value = 0;
	if(player == white){
		// White
		for(int i=0;i<8;++i){
			for(int j=0;j<8;++j){
				if(bb.board[16*i+j] == empty)
					continue;
				if(bb.board[16*i+j] == white){
					Value += getPieceValue(bb,16*i+j,i,j);
					if(i == 0){
						Value += WinValue;
					}else if(i == 1){
						bool threatA = false;
						bool threatB = false;
						if(j > 0)
							threatA = (bb.board[j-1] == empty);
						if(j < 7)
							threatB = (bb.board[j+1] == empty);
						if(threatA && threatB)
							Value += PieceAlmostWinValue;
						Value += WhiteRank1Value;
					}else if(i == 2){
						Value += WhiteRank2Value;
					}else if(i == 3){
						Value += WhiteRank3Value;
					}else if(i == 7){
						Value += PieceHomeGroundValue;
					}
				}else{
					Value -= getPieceValue(bb,16*i+j,i,j);
					if(i == 7){
						Value -= WinValue;
					}else if(i == 6){
						bool threatA = false;
						bool threatB = false;
						if(j > 0)
							threatA = (bb.board[112+j-1] == empty);
						if(j < 7)
							threatB = (bb.board[112+j+1] == empty);
						if(threatA && threatB){
							Value -= PieceAlmostWinValue;
						}
						Value -= BlackRank6Value;
					}else if(i == 5){
						Value -= BlackRank5Value;
					}else if(i == 4){
						Value -= BlackRank4Value;
					}else if(i == 0){
						Value -= PieceHomeGroundValue;
					}
				}
			}
			
		}
	}else{
		// Black
		for(int i=0;i<8;++i){
			for(int j=0;j<8;++j){
				if(bb.board[16*i+j] == empty)
					continue;
				if(bb.board[16*i+j] == white){
					Value -= getPieceValue(bb,16*i+j,i,j);
					if(i == 0){
						Value -= WinValue;
					}else if(i == 1){
						bool threatA = false;
						bool threatB = false;
						if(j > 0)
							threatA = (bb.board[j-1] == empty);
						if(j < 7)
							threatB = (bb.board[j+1] == empty);
						if(threatA && threatB)
							Value -= PieceAlmostWinValue;
						Value -= WhiteRank1Value;
					}else if(i == 2){
						Value -= WhiteRank2Value;
					}else if(i == 3){
						Value -= WhiteRank3Value;
					}else if(i == 7){
						Value -= PieceHomeGroundValue;
					}
				}else{
					Value += getPieceValue(bb,16*i+j,i,j);
					if(i == 7){
						Value += WinValue;
					}else if(i == 6){
						bool threatA = false;
						bool threatB = false;
						if(j > 0)
							threatA = (bb.board[112+j-1] == empty);
						if(j < 7)
							threatB = (bb.board[112+j+1] == empty);
						if(threatA && threatB){
							Value += PieceAlmostWinValue;
						}
						Value += BlackRank6Value;
					}else if(i == 5){
						Value += BlackRank5Value;
					}else if(i == 4){
						Value += BlackRank4Value;
					}else if(i == 0){
						Value += PieceHomeGroundValue;
					}
				}
			}
			
		}
	}
	return Value;
}

void moveOrdering(Board& bb,Byte player,vector<int>& possiblemoves, vector<int>& invertedlist){
	vector<Pair> tmp;
	for(int i=0;i<invertedlist.size();++i){
		// Exec move on board
		bool capturable = false;
		int dest = possiblemoves[i];
		int src = invertedlist[i];
		if(bb.board[dest] == !player){
			if(player == white)
				bb.black_piece--;
			else
				bb.white_piece--;
			capturable = true;
		}
		bb.setMove(dest,player);
		bb.setMove(src,empty);

		// Get Evaluation Score
		int eval_score = eval(bb,player);

		// Undo Move
		if(capturable == true){
			bb.setMove(dest,!player);
			if(player == white)
				bb.black_piece++;
			else
				bb.white_piece++;
		}else{
			bb.setMove(dest,empty);
		}
		bb.setMove(src,player);
		// ==========
		int src_score = invertedlist[i];
		int dest_score = possiblemoves[i];
		Pair obj = {eval_score,src_score,dest_score};
		tmp.push_back(obj);
	}
	stable_sort(tmp.begin(),tmp.end());
	// Re-Ordering
	for(int i=0;i<tmp.size();++i){
		possiblemoves[i] = tmp[i].dest_score;
		invertedlist[i] = tmp[i].src_score;
	}
}

int atk(Board &bb,int& dest,int& player,int& depth){
	int value = 0;
	// Capturing opponent piece earlier would be better (Shallow Depth)
	if(player == black){
		// Black
		if(dest+15 < 120 && bb.board[dest+15] == !player){
			value -= PieceAttackValue + ((loctable[dest] + depth)<<2);
		}else if(dest+17 < 120 && bb.board[dest+17] == !player){
			value -= PieceAttackValue + ((loctable[dest] + depth)<<2);
		}else{
			value += (PieceAttackValue + ((loctable[dest] + depth)<<2));
		}
	}else{
		// White
		if(dest-15 >= 0 && bb.board[dest-15] == !player){
			value -= PieceAttackValue + ((loctable[dest] + depth)<<2);
		}
		else if(dest-17 >= 0 && bb.board[dest-17] == !player){
			value -= PieceAttackValue + ((loctable[dest] + depth)<<2);
		}else{
			value += (PieceAttackValue + ((loctable[dest] + depth)<<2));
		}		
	}
	return value;
}

int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta){
	int value;
	// Terminal_test
	if(bb.gameOver()){
		Byte winner = bb.getWinner();
		if(winner == player){
			return 999999+(depth<<5);
		}
		else{
			return -999999-(depth<<5);
		}
	}else if(depth <= 0){
		return eval(bb,player);
	}
	// TimeUp
	if(  (double)(clock() - start) > TimeLimited ){
		timeUp = true;
		return TimeUpSignal;
	}


	// Generate Moves
	vector<int> possiblemoves;
	vector<int> invertedlist;
	int dest, src;
	bb.possibleMoves(player,possiblemoves,invertedlist);
	// Move Ordering
	if(possiblemoves.size() > 36){
		moveOrdering(bb,player,possiblemoves,invertedlist);
	}


	bool capturable = false;
	for(int i=0;i<possiblemoves.size();++i){
		// Exec move on board
		capturable = false;
		dest = possiblemoves[i];
		src = invertedlist[i];
		if(bb.board[dest] == !player){
			if(player == white)
				bb.black_piece--;
			else
				bb.white_piece--;
			capturable = true;
		}
		bb.setMove(dest,player);
		bb.setMove(src,empty);

		// a-b with Negamax and Incremental Updates and quiescence search
		if(capturable == true){
			value = atk(bb,dest,player,depth);
			if(depth == 1){// do one more search if someone capture the piece at the bottom layer
				value -= abnegamax_incrupdate_quisc(bb,!player,depth,-1*beta+value,-1*alpha+value);
			}else{
				value -= abnegamax_incrupdate_quisc(bb,!player,depth-1,-1*beta+value,-1*alpha+value);
			}
		}else{
			int PieceMoveValue = loctable[dest] - loctable[src];
			value  = PieceMoveValue - abnegamax_incrupdate_quisc(bb,!player,depth-1,-1*beta+PieceMoveValue,-1*alpha+PieceMoveValue);
		}

		// Undo move
		if(capturable == true){
			bb.setMove(dest,!player);
			if(player == white)
				bb.black_piece++;
			else
				bb.white_piece++;
		}else{
			bb.setMove(dest,empty);
		}
		bb.setMove(src,player);
		
		
		if(value >= beta){
			return value;
		}
		if(value > alpha){
			alpha = value;
			// Keep the best move on first layer
			if(depth == maxdepth){
				bestsrc = src;
				bestdest = dest;
			}
		}
	}
	return alpha;
}

void bestmove(Board &bb, Byte player, vector<char> &recorder){
	maxdepth = 2;// always reset the mexdepth
	int score;
	int history_src, history_dest;
	do{
		start = clock();
		timeUp = false;
		score = abnegamax_incrupdate_quisc(bb,player,maxdepth,-99999999,99999999);
		// TimeUp
		if(timeUp == true){
			bestdest = history_dest;
			bestsrc = history_src;
			//printf("time(sec): %f\n",((double)(clock() - start) / (double)CLOCKS_PER_SEC));
			printf("TimeUp!\n");
			break;
		}else{
			// Record history
			history_dest = bestdest;
			history_src = bestsrc;
		}
		printf("Depth_%d\t\ttime(sec): %f\n",maxdepth,((double)(clock() - start) / (double)CLOCKS_PER_SEC));
		// Update
		maxdepth += 1;
	}while(  (double)(clock() - start)  < TimeLimited );
	printf("AI's turn:\t\t%d %c -> %d %c\n",8-bestsrc/16,'A'+bestsrc%16,8-bestdest/16,'A'+bestdest%16);
	
	// Put piece on the board actually
	if(bb.board[bestdest] == !player){
		if(player == white)
			bb.black_piece--;
		else
			bb.white_piece--;
	}
	bb.setMove(bestdest,player);
	bb.setMove(bestsrc,empty);
	// Record the move
	addRecord(recorder,bestsrc,bestdest);
}

#endif /* alphabeta_h */