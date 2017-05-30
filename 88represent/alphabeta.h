#ifndef alphabeta_h
#define alphabeta_h
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


#define WinValue 700000
#define PieceAlmostWinValue 20000
#define PieceValue 2600
#define PieceDangerValue 20
#define PieceHighDangerValue 200
#define PieceAttackValue 100
#define PieceProtectionValue 130
#define PieceConnectionHValue 70
#define PieceConnectionVValue 30
#define PieceColumnHoleValue 40
#define PieceHomeGroundValue 20
#define PieceMoveValue 50

#define WhiteRank3Value 40
#define WhiteRank2Value 160
#define WhiteRank1Value 640
#define BlackRank4Value 40
#define BlackRank5Value 160
#define BlackRank6Value 640

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
#define maxdepth 4
int bestsrc = -1;
int bestdest = -1;


int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta);
int abnegamax(Board &bb,int player,int depth,int a,int b);

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


	// Predict Next Move
	int AttckedValue = 0;
	int ProtectedValue = 0;
	int t = (bb.board[loc]==black)?1:-1;
	for(int i=0;i < 1;++i){
		if(bb.board[loc+15*t] != empty){
			if(bb.board[loc+15*t] == (!bb.board[loc])){
				// attack enemy
				AttckedValue += PieceAttackValue;
			}else{
				// protected by someone
				ProtectedValue += PieceProtectionValue;
			}
		}	
		if(bb.board[loc+17*t] != empty){
			if(bb.board[loc+17*t] == (!bb.board[loc])){
				// attack enemy
				AttckedValue += PieceAttackValue;
			}else{
				// protected by someone
				ProtectedValue += PieceProtectionValue;
			}
		}	
	}
	// Add Protected Value
	Value += PieceProtectionValue;
	// Evaluate Attack
	if(AttckedValue > 0){
		Value -= AttckedValue;
		if(ProtectedValue == 0)
			Value -= AttckedValue;
	}else{
		if(ProtectedValue != 0){
			// Pawns at the end that are not attacked are worth more points
			if(bb.board[loc] == white){
				if(i == 2)
					Value += PieceDangerValue;
				else if(i == 1)
					Value += PieceHighDangerValue;
			}else{
				if(i == 5)
					Value += PieceDangerValue;
				else if(i == 6)
					Value += PieceHighDangerValue;
			}
		}
	}
	// danger value
	if(bb.board[loc] == white)
		Value += i * PieceDangerValue;
	else
		Value += (8-i) * PieceDangerValue;
	return Value;
}


int eval(Board &bb,Byte player){
	int Value = 0;
	for(int i=0;i<8;++i){
		int blackOnRow = 0;
		int whiteOnRow = 0;
		for(int j=0;j<8;++j){
			if(bb.board[16*i+j] == empty)
				continue;
			if(bb.board[16*i+j] == white){
				// White
				// addition could be regarded as eliminating black's power
				++whiteOnRow;
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
				// Black
				// subtraction could be regarded as enhancing black's power
				++blackOnRow;
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
		if(whiteOnRow == 0)
			Value -= PieceColumnHoleValue;//enhance black's power
		if(blackOnRow == 0)
			Value += PieceColumnHoleValue;//eliminate black's power
	}
	// Invert Value for Negamax
	if(player == black)
		Value *= -1;
	
	return Value;
}

void moveOrdering(Board& bb,Byte player,vector<int>& possiblemoves, vector<int>& invertedlist){
	vector<Pair> tmp;
	for(int i=0;i<invertedlist.size();++i){
		// Exec move on board
		bool capturable = false;
		int dest = possiblemoves[i];
		int src = invertedlist[i];
		if(bb.board[dest] == !player)
			capturable = true;
		bb.setMove(dest,player);
		bb.setMove(src,empty);

		// Get Evaluation Score
		int eval_score = eval(bb,player);

		// Undo Move
		if(capturable == true)
			bb.setMove(dest,!player);
		else
			bb.setMove(dest,empty);
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

int abnegamax_incrupdate_quisc(Board &bb, int player, int depth,int alpha,int beta){
	int value;
	// Terminal_test
	if(bb.gameOver()){
		Byte winner = bb.getWinner();
		if(winner == player)
			return 999999+depth;
		else
			return -999999-depth;
		//same result
		/*if(winner == black)
			return -999999-depth;
		else
			return 999999+depth;*/
	}
	if(depth <= 0){
		//return 0;
		return eval(bb,player);
	}
	// =========
	vector<int> possiblemoves;
	vector<int> invertedlist;
	int dest, src;
	bb.possibleMoves(player,possiblemoves,invertedlist); // Generate Moves
	if(possiblemoves.size() > 34)
		moveOrdering(bb,player,possiblemoves,invertedlist);

	bool capturable = false;
	for(int i=0;i<possiblemoves.size();++i){
		// Exec move on board
		capturable = false;
		dest = possiblemoves[i];
		src = invertedlist[i];
		if(bb.board[dest] == !player)
			capturable = true;
		bb.setMove(dest,player);
		bb.setMove(src,empty);

		// a-b with Negamax and Incremental Updates and quiescence search
		if(capturable == true){
			value = PieceAttackValue + depth;// Capturing opponent piece earlier would be better
			if(depth == 1){// do one more search if someone capture the piece at the bottom layer
				value -= abnegamax_incrupdate_quisc(bb,!player,depth,-1*beta+value,-1*alpha+value);
			}else{
				value -= abnegamax_incrupdate_quisc(bb,!player,depth-1,-1*beta+value,-1*alpha+value);
			}
		}else{
			value  = PieceMoveValue - abnegamax_incrupdate_quisc(bb,!player,depth-1,-1*beta+PieceMoveValue,-1*alpha+PieceMoveValue);
		}

		// Undo move
		if(capturable == true)
			bb.setMove(dest,!player);
		else
			bb.setMove(dest,empty);
		bb.setMove(src,player);
		//printf("%d -> %d      score = %d\n",src,dest,value);
		
		if(value >= beta){
			return value;
		}

		if(value > alpha){
			alpha = value;
			// Keep the best move on first layer
			if(depth == maxdepth){
				//printf("%d -> %d score:%d\n",src,dest,value);
				bestsrc = src;
				bestdest = dest;
			}
		}
	}
	return alpha;
}

int abnegamax(Board &bb,int player,int depth,int a,int b){
	// Terminal_test
	if(bb.gameOver()){
		Byte winner = bb.getWinner();
		if(winner == player){
			return 999999+depth;
		}
		else{
			return -999999-depth;
		}
	}
	if(depth <= 0){
		return eval(bb,player);
	}
	// =========
	vector<int> possiblemoves;
	vector<int> invertedlist;
	int dest, src;
	bb.possibleMoves(player,possiblemoves,invertedlist);
	bool capturable = false;
	int v = -99999999;
	for(int i=0;i<possiblemoves.size();++i){
		// Exec move on board
		capturable = false;
		dest = possiblemoves[i];
		src = invertedlist[i];
		if(bb.board[dest] == !player)
			capturable = true;
		bb.setMove(dest,player);
		bb.setMove(src,empty);

		// AlphaBetaWithNegaMax
		int tmp = -1 * abnegamax(bb, !player, depth-1,-1*b ,-1*a);

		// Undo move
		if(capturable == true)
			bb.setMove(dest,!player);
		else
			bb.setMove(dest,empty);
		bb.setMove(src,player);
		
		v = (tmp>v)?tmp:v;
		if(v >= b){
			return v;
		}
		if(v > a){
			a = v;
			// Keep the best move on first layer
			if(depth == maxdepth){
				bestsrc = src;
				bestdest = dest;
			}
		}
	}
	return v;
}




void bestmove(Board &bb, Byte player){
	//int score = abnegamax(bb,player,maxdepth,-99999999,99999999);
	int score = abnegamax_incrupdate_quisc(bb,player,maxdepth,-99999999,99999999);
	printf("AI's turn:\n%d %c -> %d %c    score:%d\n",bestsrc/16,'A'+bestsrc%16,bestdest/16,'A'+bestdest%16,score);
	//printf("%d -> %d    score:%d \n",bestsrc,bestdest,score);
	bb.setMove(bestdest,player);
	bb.setMove(bestsrc,empty);
}








#endif /* alphabeta_h */