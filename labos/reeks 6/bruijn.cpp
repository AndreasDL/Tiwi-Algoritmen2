#include <iostream>
#include "graaf.h"
using namespace std;



int main(int argc,char *argv[]){	
	//M => macht, n => aantal knopen
	const int m = 2;//(int)argv[0];
	const int n = 2;//(int)argv[1];
	
	//graafaanmaken
	GraafMetTakdata<GERICHT,int> g(n);
	//verbindingen
	for (int j = 0 ; j < n ; j++){		
		for (int i = 0 ; i < n ; i++){
			g.voegVerbindingToe(i,j,j);
		}
	}
	std::cerr<<g<<'\n';
	
	//eulerreeks bepalen
	 vector<int> v = g.geefEulerReeks(m);
	 for (int i = 0 ; i < v.size() ; i++)
	 	cout << v[i];// << endl;

	return 0;
}

