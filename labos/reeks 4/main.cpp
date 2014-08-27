#include <iostream>
#include "prquadtree.h"
#include <math.h>
using namespace std;



int main(int argc,char *argv[]){
	const int aantalPunten= 10;
	const int MAX= 1w0;
	
	PRquadtree boompje(MAX);
	cout << "planted a tree" << endl;
	
	for (int i = 0 ; i < aantalPunten ; i++){
		//boompje.voegtoe( (1000000-5*i)* cos(i*0.23*M_PI) , (1000000-5*i)* sin(i*0.23*M_PI) );	
		boompje.voegtoe(i,i);
		cout << endl << endl;
		//cout << i << "added to tree" << endl;
	}
	
	//boompje.write();
	
	return 0;
}

