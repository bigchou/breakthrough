//
//  board.h
//  breakthrough
//
//  Created by WAN SHR-TZE on 07/05/2017.
//
//

#ifndef board_h
#define board_h
#define BRD_SQ_NUM 120

typedef unsigned long long US64;

enum {
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};

enum{
    RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};

enum{
    WHITE, BLACK, BOTH
};

enum{
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};


struct Board{
    US64 pawns[3];
};



int FR2SQ(int f,int r){
    return 21+f + r*10;
}

void InitSq120To64(int* Sq120ToSq64, int* Sq64ToSq120){
    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;
    for(index =0;index<BRD_SQ_NUM;++index)
        Sq120ToSq64[index] = 65;
    
    for(index =0;index<64;++index)
        Sq64ToSq120[index] = 120;
    
    for(rank=RANK_1;rank<=RANK_8;++rank)
        for(file=FILE_A;file<=FILE_H;++file){
            sq = FR2SQ(file,rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++;
        }
}



#endif /* board_h */
