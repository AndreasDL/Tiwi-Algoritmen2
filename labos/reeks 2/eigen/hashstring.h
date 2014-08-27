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
		//string str(s);
		return str;
	}

    int geefExhashindex() const{
		    std::string st=str;
		    unsigned int even=0;
		    const char* s = st.c_str();
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

    bool operator==(const hashstring &hash1, const hashstring &hash2) const{
		return hash1.geefString() == hash2.geefString();
	}

    bool operator<(const hashstring &hash1, const hashstring &hash2) const{
		return hash1.geefString() < hash2.geefString();
	}

    bool operator>(const hashstring &hash1, const hashstring &hash2) const {
		return hash1.geefString() > hash2.geefString();
	}
}
