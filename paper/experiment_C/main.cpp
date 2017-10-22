
#include "board.h"
#include "alphabeta_LGV2.h"
#include "alphabeta_LGV1.h"
#include "utility.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string>





int main(){
	srand((unsigned)time(NULL));
	setTimeLimitedLGV1();// Black = LGV1
	setTimeLimitedLGV2();// White = LGV2
	int round = 3;
	int white_win = 0;
	int black_win = 0;
	for(int i=0;i<round;++i){
		// Init
		Board b;
		vector<char> recorder;
		while(!b.gameOver()){
			// White
			Byte player = white;
			bestmoveLGV2(b,player,recorder);
			if(b.gameOver())
				break;
			// Black
			bestmoveLGV1(b,!player,recorder);			
		}
		Byte winner = b.getWinner();
		if(winner == black){
			black_win++;
			printf("Black[LGV1] wins # %d\n",black_win);
		}else if(winner == white){
			white_win++;
			printf("White[LGV2] wins # %d\n",white_win);
		}
		writeRecord(recorder);
	}
	printf("Final Result\nWhite[LGV2](%d) vs Black[LGV1](%d)",white_win,black_win);
	FILE *fp = fopen("LGV2vsLGV1_result.txt","w");
	if(fp){
		fprintf(fp,"Final Result\nWhite[LGV2](%d) vs Black[LGV1](%d)",white_win,black_win);
		fclose(fp);	
	}else{
		printf("Error in Writing Files\n");
	}
	return 0;
}