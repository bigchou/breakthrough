#ifndef alphabetaLGV2_h
#define alphabetaLGV2_h
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <string>
#include "utility.h"

#define WinValueLGV2 100000
#define PieceAlmostWinValueLGV2 10000
#define PieceValueLGV2 2000
#define PieceAttackValueLGV2 1000
#define PieceConnectionHValueLGV2 100
#define PieceConnectionVValueLGV2 50
#define PieceHomeGroundValueLGV2 100
#define WhiteRank3ValueLGV2 50
#define WhiteRank2ValueLGV2 150
#define WhiteRank1ValueLGV2 450
#define BlackRank4ValueLGV2 50
#define BlackRank5ValueLGV2 150
#define BlackRank6ValueLGV2 450

int loctable[120] = {100,100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,
					10,20,30,40,40,30,20,10,0,0,0,0,0,0,0,0,
					10,30,40,50,50,40,30,10,0,0,0,0,0,0,0,0,
					20,40,50,100,100,50,40,20,0,0,0,0,0,0,0,0,
					20,40,50,100,100,50,40,20,0,0,0,0,0,0,0,0,
					10,30,40,50,50,40,30,10,0,0,0,0,0,0,0,0,
					10,20,30,40,40,30,20,10,0,0,0,0,0,0,0,0,
					100,100,100,100,100,100,100,100};

double TimeLimitedLGV2 = 2.0;
#define TimeUpSignalLGV2 999999999

// the structure is usage for move-ordering
struct PairLGV2 {
	int eval_score;
    int src_score;
    int dest_score;
    bool operator<( const PairLGV2& rhs ) const{
    	return eval_score > rhs.eval_score;
    }
};

// bestmove
int maxdepthLGV2 = 5;
int bestsrcLGV2 = -1;
int bestdestLGV2 = -1;
vector<int> possiblebestsrcLGV2;
vector<int> possiblebestdestLGV2;

// Time
time_t startLGV2;
bool timeUpLGV2 = false;
bool EnableRandomLGV2 = true;
int randomnessLGV2 = 5;// choosing the best move in 50%


int abnegamax_incrupdate_quiscLGV2(Board &bb, int player, int depth,int alpha,int beta);

double setTimeLimitedLGV2(){
	double tmp;
	printf("=== LGV2 ===\n");
	if(FILE *fp = fopen("timeControl.txt","r")){
		fscanf(fp,"%lf",&tmp);
		TimeLimitedLGV2 = tmp;
		printf("TimeLimitedLGV2: %f\n",TimeLimitedLGV2);
		TimeLimitedLGV2 = tmp * (double)CLOCKS_PER_SEC;
		fclose(fp);
		return tmp;
	}else{
		tmp = TimeLimitedLGV2;
		printf("timeControl.txt not exists, so the TimeLimitedLGV2 is %f by default\n",TimeLimitedLGV2);
		TimeLimitedLGV2 *= (double)CLOCKS_PER_SEC;
		return tmp;
	}
}


int getPieceValueLGV2(Board &bb, int loc,int i,int j){
	int Value = PieceValueLGV2;
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
		Value += PieceConnectionVValueLGV2;
	if(ConnectedH)
		Value += PieceConnectionHValueLGV2;
	Value += loctable[loc];
	return Value;
}

int evalLGV2(Board &bb,Byte player){
	int Value = 0;
	if(player == white){
		// White
		for(int i=0;i<8;++i){
			for(int j=0;j<8;++j){
				if(bb.board[16*i+j] == empty)
					continue;
				if(bb.board[16*i+j] == white){
					Value += getPieceValueLGV2(bb,16*i+j,i,j);
					if(i == 0){
						Value += WinValueLGV2;
					}else if(i == 1){
						bool threatA = false;
						bool threatB = false;
						if(j > 0)
							threatA = (bb.board[j-1] == empty);
						if(j < 7)
							threatB = (bb.board[j+1] == empty);
						if(threatA && threatB)
							Value += PieceAlmostWinValueLGV2;
						Value += WhiteRank1ValueLGV2;
					}else if(i == 2){
						Value += WhiteRank2ValueLGV2;
					}else if(i == 3){
						Value += WhiteRank3ValueLGV2;
					}else if(i == 7){
						Value += PieceHomeGroundValueLGV2;
					}
				}else{
					Value -= getPieceValueLGV2(bb,16*i+j,i,j);
					if(i == 7){
						Value -= WinValueLGV2;
					}else if(i == 6){
						bool threatA = false;
						bool threatB = false;
						if(j > 0)
							threatA = (bb.board[112+j-1] == empty);
						if(j < 7)
							threatB = (bb.board[112+j+1] == empty);
						if(threatA && threatB){
							Value -= PieceAlmostWinValueLGV2;
						}
						Value -= BlackRank6ValueLGV2;
					}else if(i == 5){
						Value -= BlackRank5ValueLGV2;
					}else if(i == 4){
						Value -= BlackRank4ValueLGV2;
					}else if(i == 0){
						Value -= PieceHomeGroundValueLGV2;
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
					Value -= getPieceValueLGV2(bb,16*i+j,i,j);
					if(i == 0){
						Value -= WinValueLGV2;
					}else if(i == 1){
						bool threatA = false;
						bool threatB = false;
						if(j > 0)
							threatA = (bb.board[j-1] == empty);
						if(j < 7)
							threatB = (bb.board[j+1] == empty);
						if(threatA && threatB)
							Value -= PieceAlmostWinValueLGV2;
						Value -= WhiteRank1ValueLGV2;
					}else if(i == 2){
						Value -= WhiteRank2ValueLGV2;
					}else if(i == 3){
						Value -= WhiteRank3ValueLGV2;
					}else if(i == 7){
						Value -= PieceHomeGroundValueLGV2;
					}
				}else{
					Value += getPieceValueLGV2(bb,16*i+j,i,j);
					if(i == 7){
						Value += WinValueLGV2;
					}else if(i == 6){
						bool threatA = false;
						bool threatB = false;
						if(j > 0)
							threatA = (bb.board[112+j-1] == empty);
						if(j < 7)
							threatB = (bb.board[112+j+1] == empty);
						if(threatA && threatB){
							Value += PieceAlmostWinValueLGV2;
						}
						Value += BlackRank6ValueLGV2;
					}else if(i == 5){
						Value += BlackRank5ValueLGV2;
					}else if(i == 4){
						Value += BlackRank4ValueLGV2;
					}else if(i == 0){
						Value += PieceHomeGroundValueLGV2;
					}
				}
			}
			
		}
	}
	return Value;
}

void moveOrderingLGV2(Board& bb,Byte player,vector<int>& possiblemoves, vector<int>& invertedlist){
	vector<PairLGV2> tmp;
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
		int eval_score = evalLGV2(bb,player);

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
		PairLGV2 obj = {eval_score,src_score,dest_score};
		tmp.push_back(obj);
	}
	stable_sort(tmp.begin(),tmp.end());
	// Re-Ordering
	for(int i=0;i<tmp.size();++i){
		possiblemoves[i] = tmp[i].dest_score;
		invertedlist[i] = tmp[i].src_score;
	}
}

int abnegamax_incrupdate_quiscLGV2(Board &bb, int player, int depth,int alpha,int beta){
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
		return evalLGV2(bb,player);
	}
	// timeUpLGV2
	if(  (double)(clock() - startLGV2) > TimeLimitedLGV2 ){
		timeUpLGV2 = true;
		return TimeUpSignalLGV2;
	}


	// Generate Moves
	vector<int> possiblemoves;
	vector<int> invertedlist;
	int dest, src;
	bb.possibleMoves(player,possiblemoves,invertedlist);
	// Move Ordering
	if(possiblemoves.size() > 36){
		moveOrderingLGV2(bb,player,possiblemoves,invertedlist);
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
			value = PieceAttackValueLGV2 + ((loctable[dest] + depth)<<2);// Capturing opponent piece earlier would be better (Shallow Depth)
			if(depth == 1){// do one more search if someone capture the piece at the bottom layer
				value -= abnegamax_incrupdate_quiscLGV2(bb,!player,depth,-1*beta+value,-1*alpha+value);
			}else{
				value -= abnegamax_incrupdate_quiscLGV2(bb,!player,depth-1,-1*beta+value,-1*alpha+value);
			}
		}else{
			int PieceMoveValue = loctable[dest] - loctable[src];
			value  = PieceMoveValue - abnegamax_incrupdate_quiscLGV2(bb,!player,depth-1,-1*beta+PieceMoveValue,-1*alpha+PieceMoveValue);
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
			if(depth == maxdepthLGV2){
				bestsrcLGV2 = src;
				bestdestLGV2 = dest;
				possiblebestsrcLGV2.push_back(bestsrcLGV2);
				possiblebestdestLGV2.push_back(bestdestLGV2);
			}
		}
	}
	return alpha;
}




void bestmoveLGV2(Board &bb, Byte player, vector<char> &recorder){
	maxdepthLGV2 = 2;// always reset the mexdepth
	int score;
	int history_src, history_dest;
	do{
		startLGV2 = clock();
		timeUpLGV2 = false;
		score = abnegamax_incrupdate_quiscLGV2(bb,player,maxdepthLGV2,-99999999,99999999);
		// timeUpLGV2
		if(timeUpLGV2 == true){
			bestdestLGV2 = history_dest;
			bestsrcLGV2 = history_src;
			//printf("time(sec): %f\n",((double)(clock() - startLGV2) / (double)CLOCKS_PER_SEC));
			//printf("timeUpLGV2!\n");
			possiblebestsrcLGV2.clear();
			possiblebestdestLGV2.clear();
			break;
		}else{
			// Record history
			if(EnableRandomLGV2 && possiblebestdestLGV2.size() >= 2){
				if( rand() % 10 < randomnessLGV2 ){
					history_dest = possiblebestdestLGV2[possiblebestdestLGV2.size()-2];
					history_src = possiblebestsrcLGV2[possiblebestsrcLGV2.size()-2];
				}else{
					history_dest = possiblebestdestLGV2[possiblebestdestLGV2.size()-1];
					history_src = possiblebestsrcLGV2[possiblebestsrcLGV2.size()-1];
				}
			}else{
				history_dest = bestdestLGV2;
				history_src = bestsrcLGV2;
			}
			possiblebestsrcLGV2.clear();
			possiblebestdestLGV2.clear();
		}
		//printf("Depth_%d\t\ttime(sec): %f\n",maxdepthLGV2,((double)(clock() - startLGV2) / (double)CLOCKS_PER_SEC));
		// Update
		maxdepthLGV2 += 1;
	}while(  (double)(clock() - startLGV2)  < TimeLimitedLGV2 );
	//printf("AI's turn:\t\t%d %c -> %d %c\n",8-bestsrcLGV2/16,'A'+bestsrcLGV2%16,8-bestdestLGV2/16,'A'+bestdestLGV2%16);
	
	// Put piece on the board actually
	if(bb.board[bestdestLGV2] == !player){
		if(player == white)
			bb.black_piece--;
		else
			bb.white_piece--;
	}
	bb.setMove(bestdestLGV2,player);
	bb.setMove(bestsrcLGV2,empty);
	// Record the move
	addRecord(recorder,bestsrcLGV2,bestdestLGV2);
}

#endif /* alphabeta_h */