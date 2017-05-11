#ifndef utility_h
#define utility_h
#include "board.h"
#include <stdio.h>

int InputSrc(){
	char x, y;
	scanf(" %c %c",&x,&y);
	return 16*(x-'0') + (y-'A');
}

int InputDest(){
	char x, y;
	scanf(" %c %c",&x,&y);
	return 16*(x-'0') + (y-'A');
}


#endif /* utility_h */