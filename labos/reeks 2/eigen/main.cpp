#include <iostream>
#include <string>
#include <string.h>

#include "exhash.h"

class hashstring{
private: 
    std::string str;
public: 
	hashstring(){
		str = "";
	}

    hashstring(std::string s){
		str= s;
	}

	const std::string geefString()const {
		return str;
	}

    int geefExhashindex() const{
		    unsigned int even=0;
		    const char* s = str.c_str();
    int len=strlen(s);
    for (int i=0;i<len;i++){
        unsigned int pl=s[i];
        pl=pl*8+pl/32;//eerste  4 bits achteraan;
        even=even*253+pl;
        //pl*((unsigned int)1 << 24)+even/37;
    }
    unsigned int even2=0;
    int aantbits=sizeof(int)*8;
    for (int i=0;i < aantbits/2;i++)
        even2+=(even>>i)*((unsigned int)1 << (aantbits-1-i));
    return even2^even;

	}
	
	bool operator==(const hashstring & hash1) const{
		return (str == hash1.geefString());
	}
	
	bool operator<(const hashstring & hash1) const{
		return (str < hash1.geefString());
	}

};

int main(){
	Exhash<hashstring,int> h;

/*	hashstring hs("testqsdfghjklm");
	hashstring ts("testing1234567");
	std::cout << ts.geefString() << std::endl;
	std::cout << hs.geefString() << std::endl;
	h.voegtoe(hs, 1);
	h.write();
*/	

	for (int i = 1 ; i <= 8 ; i++){
		std::string s;
		std::cin >> s;
		hashstring hs(s);
		//cout << hs.geefString() << ":" ;
		//cout << (unsigned int) hs.geefExhashindex() << endl;
		h.voegtoe(hs,i);
		h.write();
	}
    
    return 0;
}
