#include "exhash.h"
#include <iostream>
#include <ctime>
#include <random>

using namespace std;

const int AANTAL	 = 45;
const int MAX_LENGTE = 5;
const int MIN_LENGTE = 1;

void main() {
	srand(time(0));
	Exhash<ExhashString,int>* exhash = new Exhash<ExhashString,int>();
	for (int i = 0; i < AANTAL; i++) {
		ExhashString exhashstr;
		int aantalChars = rand() % (MAX_LENGTE-MIN_LENGTE+1) + MIN_LENGTE;
		for (int j = 0; j < aantalChars; j++) {
			char ch = 'a' + (rand() % 26);
			
			exhashstr.setString(exhashstr.getString().append(1, ch)); //voeg een random char toe
		}
		exhash->voegtoe(exhashstr, 0);
	}
	delete exhash;

	cin.get();
}