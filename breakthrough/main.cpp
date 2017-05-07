//
//  main.cpp
//  breakthrough
//
//  Created by WAN SHR-TZE on 02/05/2017.
//
//

#include <stdio.h>
#include "board.h"
#include "bitboard.h"



int main(){
    int Sq120ToSq64[BRD_SQ_NUM];
    int Sq64ToSq120[64];
    
    struct Board board;
    InitSq120To64(Sq120ToSq64,Sq64ToSq120);
    PrintBoard(Sq120ToSq64,Sq64ToSq120);
    
    
    
    US64 playBitBoard = 0ULL;
    PrintBitBoard(playBitBoard, Sq120ToSq64);
    
    playBitBoard |= (1ULL << SQ64(Sq120ToSq64, D2));
    printf("D2 Added!\n");
    PrintBitBoard(playBitBoard, Sq120ToSq64);
    
    
    return 0;
}
