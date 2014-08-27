#include <iostream>
# include <stdio.h>
# include <limits.h>
using namespace std;
 
//max bepalen
int max(int a, int b) { return (a > b)? a: b; }


int main(int argc,char *argv[]){
		const int E = 20; 	//max eggs
		const int V = 20000; 	//max floors
		
		//init
		int eggFloor[E+1][V+1];
	    int res;
	 
	    //1 verdiep   => 1 poging
	    //0 verdiepen => 0 pogingen
	    for (int i = 1; i <= E; i++){
	        eggFloor[i][1] = 1;
	        eggFloor[i][0] = 0;
	    }
	    
	    //1 ei => altijd evenveel worpen als verdiepen
	    for (int j = 1; j <= V; j++){
	    	eggFloor[1][j] = j;
	    }
	 
	    //rest opvullen, gebaseerd op vorige waarden
	    for (int i = 2; i <= E; i++){
	        for (int j = 2; j <= V; j++){
	            
				eggFloor[i][j] = INT_MAX;
				// if in de for is altijd kleiner in eerste geval (anders zit er random waarde in)
	            
	            for (int x = 1; x <= j; x++){ //beginnen met op elk mogelijk verdiep te gooien
	            	
	                res = 1 + max(eggFloor[i-1][x-1], eggFloor[i][j-x]);
					//max worpen = 1worp (worp zelf)
					//+max van:
					//->(ei breekt)		-> met een ei minder, 1 verdiep minder oplossen.
					//->(ei breekt niet)-> met zelfde aantal eieren => j-x overige verdiepen oplossen.
	                
					if (res < eggFloor[i][j]){
	                	eggFloor[i][j] = res;
	                }//kleinste res behouden (min van de max)
	            }
	            
	        }
	    }
	    
	    
	    
	    //uitvoer
	    cout << "result: " << endl;
		cout << "\t";
		for (int ei = 1 ; ei <= E ; ei++){
			cout << ei << "\t";
		}
		cout << endl;
		
	    for (int ver=0 ; ver <= V ; ver++){
	    	cout << ver << "\t" ;
	    	for (int ei = 1 ; ei <= E ; ei++){
	    		cout << eggFloor[ei][ver] << "\t";
			}	
			cout << endl;
	    }
	return 0;
}
