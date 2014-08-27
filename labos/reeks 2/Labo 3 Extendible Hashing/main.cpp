#include <iostream>
#include "exhash.h"
#include <sstream>

using namespace std;

int main(){    
    Exhash<HashString, HashString> exhash;
    
    for (int i = 0; i < 10; i++) {  
        
        string a = "" + i;
        cout<<"String a: "<<a<<endl;
        
        cout<<"Pre toevoegen: "<<exhash.tab[0]->kn[0].sleutel.getString()<<endl;
        exhash.voegtoe(HashString(a.c_str()), HashString(a.c_str()));
        cout<<"Na toevoegen: "<<exhash.tab[0]->kn[0].sleutel.getString()<<endl;
    }
        
    for(unsigned int i = 0; i < (unsigned int)pow(2, exhash.maxk); i++){
		bitset<32> b((long) i);
		cout<<b.to_string()<<endl;
		for(int j = 0; j < exhash.tab[i]->aantalKnopen; j++){
			cout<<"\t>"<<exhash.tab[i]->kn[j].sleutel.getString()<<endl;
		}
	}
    
    return 0;
}
