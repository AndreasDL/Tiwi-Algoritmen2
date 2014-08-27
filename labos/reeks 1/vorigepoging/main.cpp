#include <iostream>
#include "zoekboom.h"
using namespace std;

int main(){
	const int AANTAL = 100;

	Zoekboom<int,int> boom;
	for (int i = 0 ; i < AANTAL; i++){
		boom.voegtoe(i,i);
	}
	//cout << boom << endl;

	cout << "de diepte is : " << endl;
	cout << boom.gemiddeldeDiepte() << endl;


	boom.verwijder(1);
	cout << boom << endl;

	return 0;
}