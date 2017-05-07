//
//  bitboard.h
//  breakthrough
//
//  Created by WAN SHR-TZE on 07/05/2017.
//
//

#ifndef bitboard_h
#define bitboard_h
#include "board.h"

void PrintBitBoard(US64 bb,int* Sq120ToSq64){
    US64 shiftMe = 1ULL;
    int rank = 0;
    int file = 0;
    int sq = 0;
    int sq64 = 0;
    
    printf("\n");
    printf("   A B C D E F G H\n");
    for(rank =RANK_8; rank>=RANK_1;--rank){
        printf("%d ",rank+1);//noice it plus 1
        for(file = FILE_A;file<= FILE_H;++file){
            sq = FR2SQ(file, rank);
            //printf("..sq:%d..",sq);  91
            sq64 = SQ64(Sq120ToSq64,sq);
            //printf("..sq64:%d..",sq64); 56
            if((shiftMe << sq64) & bb)
                printf(" X");
            else
                printf(" -");
        }
        printf("\n");
    }
    printf("\n\n");
    
}

#endif /* bitboard_h */
