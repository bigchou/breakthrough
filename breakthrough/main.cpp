//
//  main.cpp
//  breakthrough
//
//  Created by WAN SHR-TZE on 02/05/2017.
//
//

#include <stdio.h>
#include "board.h"

int Sq120ToSq64[BRD_SQ_NUM];
int Sq64ToSq120[64];

int main(){
    //printf("Hello World");
    struct Board board;
    InitSq120To64(Sq120ToSq64,Sq64ToSq120);
    
    printf("%d\n",BRD_SQ_NUM);
    // Print
    for(int i=0;i<BRD_SQ_NUM;++i){
        if(i%10==0)
            printf("\n");
        printf("%4d",Sq120ToSq64[i]);
    }
    
    printf("\n\n");
    
    for(int i=0;i<64;++i){
        if(i%8==0)
            printf("\n");
        printf("%4d",Sq64ToSq120[i]);
    }
    printf("\n");
    return 0;
}
