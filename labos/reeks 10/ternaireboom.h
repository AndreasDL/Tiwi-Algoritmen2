#include <string>
#include <iostream>
using namespace std;

class TKnoop;
class TernaireBoom{
protected:
    TKnoop* k;
    const static char afsluiter;
public:
	TernaireBoom(){k=0;};
	void voegtoe(const string &s);
	void print();
};
const char TernaireBoom::afsluiter=char(127);
class TKnoop{
friend class TernaireBoom;
protected:
    char c;
    TernaireBoom kind[3];
};


void TernaireBoom::voegtoe(const string &s){
	if (k == 0){ //nog niets in boom
		//nieuwe knoop maken
		k = new TKnoop();
		TKnoop* h = k;
		
		//letters toevoegen
		for (int i = 0; i < s.size() ; i++){
			//char insteken
			h->c = s[i];
			
			//nieuwe knoop aanmaken
			h->kind[1].k = new TKnoop();
			
			//afdalen
			h = h->kind[1].k;
		}
		
		//afsluiter
		h->c = afsluiter;
	}else{
		if (s.size() > 0){
			if (k->c == s[0]){
				//een lager 
				//volgende letter beginnen
				k->kind[1].voegtoe(s.substr(1));
				
			}else if (k->c > s[0]){
				//links afdalen
				k->kind[0].voegtoe(s);
			}else{
				//rechts afdalen
				k->kind[2].voegtoe(s);
			}
			
		}
	}
}
void TernaireBoom::print(){
	TKnoop* h = k;
	cout << h->c << endl;
	if (h->c != afsluiter){
		
		if(h->kind[0].k != 0){
			cout << "links ";
			h->kind[0].print();
		}
		
		if(h->kind[1].k != 0){
			cout << "\tmidden: ";
			h->kind[1].print();
		}
		
		if(h->kind[2].k != 0){
			cout << "\t\trechts: ";
			h->kind[2].print();
		}
	}
}
