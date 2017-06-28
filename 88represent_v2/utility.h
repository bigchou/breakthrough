#ifndef utility_h
#define utility_h
#include "board.h"
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <fstream>

#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#include <iostream>

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
	myfile.open(patch::to_string(count)+".txt");
	//myfile << recorder;
	for(int i=0;i<recorder.size();i+=4){
		myfile << recorder[i] << " " << recorder[i+1] << " - " << recorder[i+2] << " " << recorder[i+3] << endl;
	}
	myfile.close();
	// update info.txt
	ofstream myfile2;
	myfile2.open("info.txt");
	myfile2 << patch::to_string(count+1);
	myfile2.close();
}


#endif /* utility_h */