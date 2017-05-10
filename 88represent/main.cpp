#include <cstdio>
#include "board.h"


int main(){
	printf("Hello Wolrd\n");
	Byte M[120];

	for(int i=0;i<120;++i)
		M[i] = i;

	
	for(int i=0;i<120;++i){
		printf("%d   ",i);
		if(!(M[i] & 0x88)){
			printf("reachable\n");
		}else{
			printf("unreachable\n");
		}
	}

	return 0;
}