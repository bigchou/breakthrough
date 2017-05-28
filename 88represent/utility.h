#ifndef utility_h
#define utility_h
#include "board.h"
#include <stdio.h>
#include <ctype.h>

int inputSrc(){
	char x, y;
	scanf(" %c %c",&x,&y);
	x = toupper(x);
	y = toupper(y);
	return 16*(x-'0') + (y-'A');
}

int inputDest(){
	char x, y;
	scanf(" %c %c",&x,&y);
	x = toupper(x);
	y = toupper(y);
	return 16*(x-'0') + (y-'A');
}

#endif /* utility_h */