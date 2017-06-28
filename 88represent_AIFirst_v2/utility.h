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
	if(x == 'R' && y == 'R')
		return -1000;
	else
		return 16*('8'-x) + (y-'A');
}

int inputDest(){
	char x, y;
	scanf(" %c %c",&x,&y);
	x = toupper(x);
	y = toupper(y);
	if(x == 'R' && y == 'R')
		return -1000;
	else
		return 16*('8'-x) + (y-'A');
}

void addRecord(vector<char> &recorder,int src, int dest){
	/*recorder += ('0'+8-src/16);
	recorder += ' ';
	recorder += ('A'+src%16);
	recorder += " -> ";
	recorder += ('0'+8-dest/16);
	recorder += ' ';
	recorder += ('A'+dest%16);
	recorder += '\n';*/

	recorder.push_back('0'+8-src/16);
	recorder.push_back('A'+src%16);
	recorder.push_back('0'+8-dest/16);
	recorder.push_back('A'+dest%16);
}

void writeRecord(vector<char> &recorder){
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
	//myfile << recorder;
	for(int i=0;i<recorder.size();i+=4){
		myfile << recorder[i] << " " << recorder[i+1] << " - " << recorder[i+2] << " " << recorder[i+3] << endl;
	}
	myfile.close();
	// update info.txt
	ofstream myfile2;
	myfile2.open("info.txt");
	myfile2 << std::to_string(count+1);
	myfile2.close();
}

/*
void Undo(vector<char> &recorder, Board &bb){
	char destx = recorder.back();// A
	recorder.pop_back();
	char desty = recorder.back();// 8
	recorder.pop_back();
	int dest = 16*('8'-desty) + (destx-'A');
	char srcx = recorder.back();
	recorder.pop_back();
	char srcy = recorder.back();
	recorder.pop_back();
	int src = 16*('8'-srcy) + (srcx-'A');


}*/


#endif /* utility_h */