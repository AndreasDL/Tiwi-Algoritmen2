#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stack>
#include <vector>
#include <iomanip>
using namespace std;
const int s = 2;	  //aantal sleetjes
const int p = 3;	  //rendieren per slee
const int d = 10;	  //aantal dagen
const int ptot = s*p; //totaal aantal rendieren

int main(){
	//init
	srand (time(NULL));
	stack<int> nietToegekend; //houdt bij welke dieren nog niet toegekent zijn
	int plaatsen[s];//plaatsen[sleenr] geeft aantal beschikbare plaatsen dat nog vrij is voor die slee
	int sleeNr[ptot]; //sleeNr[rendiernr] geeft het nummer van de slee waartoe het rendier behoord. -> niet perce nodig
	vector<vector<int> > dierenPerSlee(s);//geeft per slee nr een lijst van rendieren weer
	int dagenbijelkaar[ptot][ptot];
	for (int i =0; i < ptot ; i++){
		for (int j = 0 ; j < ptot ; j++){
			dagenbijelkaar[i][j] =0;
		}
	}

	for (int dag = 0 ; dag < d ; dag++){
		//setup
		for (int i = 0 ; i < ptot ; i++)
			nietToegekend.push(i);
		for (int i = 0 ; i < s ; i++){
			plaatsen[i] = p;
			dierenPerSlee[i].clear();
		}

		//random bij elkaar gooien
		while (!nietToegekend.empty()){
			int rendier = nietToegekend.top();
			nietToegekend.pop(); //van stack smijten

			int slee = rand() % s;
			while (plaatsen[slee] == 0){ //indien de slee al vol zit
				slee = rand() %s;
			}
			dierenPerSlee[slee].push_back(rendier);
			sleeNr[rendier] = slee;
			plaatsen[slee]--;
		}

		//uitschrijven
		cout << "------------- dag " << dag << " -------------" << endl;
		for (int i = 0 ; i < s ; i ++){
			cout << "slee: " << i << ": \t";
			//uitschrijven
			for (int j =0 ; j < p ; j++){
				cout << setw(3) << dierenPerSlee[i][j] << " ";
				dagenbijelkaar[dierenPerSlee[i][j]][dierenPerSlee[i][(j+1)%p]]++;
				dagenbijelkaar[dierenPerSlee[i][(j+1)%p]][dierenPerSlee[i][j]]++;
			}
			cout << endl;
		}
	}
	
	//dagenbijelkaar geven & stat berekenen
	double gemiddelde;
	int max = 0;

	cout << endl << endl;
	cout << "------------- dagen bij elkaar -------------" << endl;
	cout << "dier: |";
	for (int i = 0 ; i < ptot ; i++){
		cout << setw(2) << i+1 << " | ";
	}
	cout << endl;

	for (int i = 0 ; i < ptot ; i++){
		cout << setw(5) << i+1 << " |"; 
		for (int j = 0 ; j < ptot ; j++){
			cout << setw(2) << dagenbijelkaar[i][j] << " | ";

			//stat
			if (dagenbijelkaar[i][j] > max){
				max = dagenbijelkaar[i][j];
			}
			gemiddelde += dagenbijelkaar[i][j];
		}
		cout << endl;
	}
	gemiddelde /= (ptot*ptot)-ptot;

	cout << "Max dagen bij elkaar = " << max << endl;
	cout << "gemiddeld aantal dagen bij elkaar " << gemiddelde << endl;
}