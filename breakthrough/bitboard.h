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

const int BitTable[64] = {
    63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
    51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
    26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
    58, 20, 37, 17, 36, 8
};

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

int countBits(US64 b){
    int r;
    for(r=0;b;r++,b&= (b-1)){
        //printf("%llu\n",b&(b-1));
        //printf("%d\n",r);
    }
    //b = 200ULL;
    //b&=(b-1);
    //printf("%llu\n",b);
    return r;
}

int PopBit(US64 &bb){
    US64 b = bb ^ (bb-1);
    unsigned int fold = (unsigned) ((b&0xffffffff)^(b >> 32));
    bb &= (bb-1);
    return BitTable[(fold * 0x783a9b23) >> 26];
}

void ClearBit(US64 &bb,int sq,US64 *ClearMask){
    bb &= ClearMask[sq];
}

void SetBit(US64 &bb,int sq,US64 *SetMask){
    bb |= SetMask[sq];
}


void InitBitMasks(US64* SetMask,US64* ClearMask){
    int index = 0;
    for(index = 0;index<64;++index){
        SetMask[index] = 0ULL;
        ClearMask[index] = 0ULL;
    }
    for(index = 0;index<64;++index){
        SetMask[index] |= (1ULL << index);
        ClearMask[index] = ~SetMask[index];
    }
}


#endif /* bitboard_h */
