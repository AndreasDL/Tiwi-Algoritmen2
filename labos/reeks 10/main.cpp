#include <iostream>
#include "ternaireboom.h"
using namespace std;


int main(int argc,char *argv[]){
	
	TernaireBoom* b = new TernaireBoom();
	b->voegtoe("hunnen");
	b->voegtoe("hunebed");
	
	b->voegtoe("aarden");
	b->voegtoe("aardvarken");
	
	b->print();
	
	return 0;
}

