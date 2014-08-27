#include <iostream>
#include "stroom.h"
#include "graaf.h"
#include <vector>
using namespace std;



int main(int argc,char *argv[]){
	
	//graaf maken
	GraafMetTakdata<GERICHT,int> g(8);
	g.voegVerbindingToe(0,1,7);
	g.voegVerbindingToe(0,2,10);
	
	g.voegVerbindingToe(1,4,4);
	
	g.voegVerbindingToe(2,3,6);
	g.voegVerbindingToe(2,5,5);
	
	g.voegVerbindingToe(3,0,2);
	g.voegVerbindingToe(3,6,6);
	
	g.voegVerbindingToe(4,5,6);
	
	g.voegVerbindingToe(5,7,8);
	
	g.voegVerbindingToe(6,5,2);
	g.voegVerbindingToe(6,7,7);
	
	g.voegVerbindingToe(7,4,1);
	
	//pad
	Pad p;
		
	//padzoeker maken
	Vergrotendpadzoeker<String> v(g,0,7,p);
	
	
	//oplossen
	cout << "hoi" << endl;
	cout << p << endl;


	return 0;
}

