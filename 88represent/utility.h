#ifndef utility_h
#define utility_h
#include "board.h"
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <fstream>

int inputSrc(){
	char x, y;
	scanf(" %c %c",&x,&y);
	x = toupper(x);
	y = toupper(y);
	return 16*('8'-x) + (y-'A');
}

int inputDest(){
	char x, y;
	scanf(" %c %c",&x,&y);
	x = toupper(x);
	y = toupper(y);
	return 16*('8'-x) + (y-'A');
}

void addRecord(string &recorder,int src, int dest){
	recorder += ('0'+8-src/16);
	recorder += ' ';
	recorder += ('A'+src%16);
	recorder += " -> ";
	recorder += ('0'+8-dest/16);
	recorder += ' ';
	recorder += ('A'+dest%16);
	recorder += '\n';
}

void writeRecord(string &recorder){
	int count = 0;
	FILE *fp = fopen("info.txt","r");
	if(!fp){// file not found, and new a file called info.txt
		FILE *fp2 = fopen("info.txt","w");
		fprintf(fp2, "%d",count);
		fclose(fp2);	
	}else{// file found
		fscanf(fp,"%d",&count);
	}
	fclose(fp);
	// write move record
	ofstream myfile;
	myfile.open(std::to_string(count)+".txt");
	myfile << recorder;
	myfile.close();
	// update info.txt
	ofstream myfile2;
	myfile2.open("info.txt");
	myfile2 << std::to_string(count+1);
	myfile2.close();
}


#endif /* utility_h */