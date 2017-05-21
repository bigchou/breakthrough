#ifndef utility_h
#define utility_h
#include "board.h"
#include <stdio.h>

int inputSrc(){
	char x, y;
	scanf(" %c %c",&x,&y);
	return 16*(x-'0') + (y-'A');
}

int inputDest(){
	char x, y;
	scanf(" %c %c",&x,&y);
	return 16*(x-'0') + (y-'A');
}




#endif /* utility_h */