#include <cstdlib>
#include <iostream>
#include <queue>
#include <math.h>
#include <string.h>
#include <bitset>

using namespace std;

template <class T,class D>
class Exhash;
/*
Volgende code is correct, maar geeft problemen; daarom worden
de constanten buiten de klasse gedefinieerd.

template <class T,class D>
class Schijfpagina;

template <class T,class D>
const unsigned int Schijfpagina<T,D>::EEN=1;

template <class T,class D>
const int Schijfpagina<T,D>::AANTBITS=sizeof(unsigned int)*8;

template <class T,class D>
const int Schijfpagina<T,D>::PAGINAGROOTTE=8;
*/
const unsigned int EEN=1;
const int AANTBITS=sizeof(unsigned int)*8;
const int PAGINAGROOTTE=8;
template <class T,class D>
class Schijfpagina{
    //static const unsigned int EEN;
    //static const int PAGINAGROOTTE;
    //static const int AANTBITS;
    friend class Exhash<T,D>;
private:
    /*
        Vars
    */
    public: int aantalKnopen;//aantal dat nu in pagina zit;
    int k;//aantal bits in hashwaarde;
    unsigned int index;//laatste k bits zijn belangrijk, die daarvoor zijn nul
    struct knoop{
        T sleutel;
        D data;
    };
    //we werken hier rechtstreeks; andere oplossing zou zijn een tabel
    //met pointers naar de eigenlijke waarden bij te houden.
    public: knoop kn[PAGINAGROOTTE];
    
    /*
        Procedures/functions
    */
    Schijfpagina():index(0),k(0),aantalKnopen(0){  }

    int geefk(){ return k; }
    
    //de huidige Schijfpagina wordt in twee gesplitst. De nieuwe hashindex is
    //dus een bit langer; *this krijgt de sleutelwaarden met hashindex
    //eindigend op nul. De nieuwe pagina, met hasindex eindigend op 1,
    //wordt aan eenbit gehangen.s
    void splits(Schijfpagina<T,D>*& eenbit){
    	int i = 0;
    	while(i < aantalKnopen){
	        unsigned int sleutelIndex = getIndex(kn[i].sleutel);  
	        //If eenbit > voeg daar toe en delete uit nulbit
	        //kn[i] wordt nu een nieuw element
	        if((sleutelIndex>>(AANTBITS-k)) & 1){
	            eenbit->voegtoe(kn[i].sleutel, kn[i].data);
	            this->verwijder(kn[i].sleutel);
	        } 
	        //Else > kn[i] blijft in nulbit
	        //schuif i op naar het volgende element
	        else {
	            i++;
	        }
    	}         
    }
    
    unsigned int getIndex(const T& sleutel) const{
        if (k==0)//computer weigert 32 plaatsen shift
            return 0;
        else{
            unsigned int q=sleutel.geef_exhashindex();
            return (q>>(AANTBITS-k));
        }
    };

    bool isVol() const{ return (aantalKnopen==PAGINAGROOTTE); }
 
    void zoek(const T& sleutel,int& plaats, bool& gevonden) const{
        plaats=0;
        while (plaats<aantalKnopen && kn[plaats].sleutel < sleutel){
            plaats++;
		}
		cout<<"Search results: plaats = "<<plaats<<", sleutel = "<<sleutel.getString()<<", last found sleutel = "<<(plaats!=aantalKnopen? kn[plaats].sleutel.getString() : "out of bounds")<<endl;
        gevonden=(plaats!=aantalKnopen && kn[plaats].sleutel==sleutel);
    }

    D* zoekData(const T& sleutel){
        int plaats;
        bool gevonden;
        zoek(sleutel,plaats,gevonden);
        if (gevonden)
            return &(kn[plaats].data);
        else
            return 0;
    }

   //deze functie checkt *expliciet niet* of de pagina al vol is
   //dit is de taak van Exhash

    void voegtoe(const T& sleutel,const D& data){
        int plaats;
        bool gevonden;
        zoek(sleutel,plaats,gevonden);
        if (!gevonden){
            for (int i=aantalKnopen;i>plaats;i--)
                kn[i]=kn[i-1];
            kn[plaats].sleutel=sleutel;
            kn[plaats].data=data;
            aantalKnopen++;
            cout<<"Added: "<<sleutel.getString()<<"; #values: "<<aantalKnopen<<endl;
        }
    }

    void verwijder(const T& sleutel){
        int plaats;
        bool gevonden;
        zoek(sleutel,plaats,gevonden);
        if (gevonden){
            aantalKnopen--;
            for (int i=plaats;i<aantalKnopen;i++)
                kn[i]=kn[i+1];
        }
    }
};

template <class T,class D>
class Exhash{
    friend class Schijfpagina<T,D>;
    /*
        Vars
    */
    public: Schijfpagina<T,D>** tab;
    public: int maxk;
    private: int grootte;//grootte tabel; volgt ook uit maxk
    private: int aantalSleutels;
    
    /*
        Procedures/functions
    */
    public: Exhash():maxk(0),grootte(1),aantalSleutels(0){
        tab=new Schijfpagina<T,D>*[1];
        tab[0]=new Schijfpagina<T,D>;
    };

    public: ~Exhash(){
    	delete[] tab;
    };

    //index geeft de index in de tabel. kan als voorbeeld dienen
    //voor schuifoperaties
    private: unsigned int index(const T& sleutel) const{
        if (maxk==0)//computer weigert 32 plaatsen shift
            return 0;
        else{
            unsigned int q=sleutel.geef_exhashindex();
            return (q>>(AANTBITS-maxk));
        }
    };

    private: void verdubbel(){
    	int kOud = maxk;
    	int kNieuw = maxk + 1;
    	
    	Schijfpagina<T,D>** tabNieuw = new Schijfpagina<T,D>*[(int) pow(2, kNieuw)];
		
		for(int i = 0; i < (int) pow(2, kOud); i++){
			tabNieuw[i*2] = tab[i];
			tabNieuw[i*2 + 1] = tab[i];
		} 
		
		maxk = kNieuw;
		
		delete[] tab;
		tab = tabNieuw;
    };
    
    public: void voegtoe(const T& sleutel,const D& data){
    	unsigned int sleutelIndex = index(sleutel);
    	//Schijfpagina vol? > splitsen/verdubbelen
    	while(tab[sleutelIndex]->isVol() && maxk < 32){
    		//If hashgrootte verdubbelen?
    		if(tab[sleutelIndex]->geefk() == maxk){
				verdubbel();
				//Sleutelindex updaten naar hogere maxk
	    		sleutelIndex = index(sleutel);
			} 
			
			//Schijfpagina splitsen
			Schijfpagina<T,D> * eenbit = new Schijfpagina<T,D>;
			//RR 1 && RL 1 = index van de vorige k + 0 (dus zeker de nieuwe nulbit index)
			tab[((sleutelIndex>>1)<<1) + 1] = eenbit;
			tab[(sleutelIndex>>1)<<1]->splits(eenbit);
    	}
    	if(maxk < 32){
    		tab[sleutelIndex]->voegtoe(sleutel, data);
    	}
    };

    // verwijder: lege pagina's blijven bestaan.
    public: void verwijder(const T& sleutel){
    	unsigned int sleutelIndex = index(sleutel);
    	tab[sleutelIndex]->verwijder(sleutel);
	};

    public: D* zoekData(const T& sleutel){
    	unsigned int sleutelIndex = index(sleutel);
    	return tab[sleutelIndex]->zoekData(sleutel);
    };
    
    //Does not work
    //Several variables have been set to public to allow a similar printing function in main.cpp
    public: ostream& operator<<(ostream& os){
    	for(unsigned int i = 0; i < (int)pow(2, maxk); i++){
    		bitset<32> b((long) i);
    		os<<b.to_string()<<endl;
    		for(int j = 0; j < tab[i]->aantalKnopen; j++){
    			os<<"\t"<<tab[i]->kn[j].sleutel.getString()<<endl;
    		}
    	}
    }
};
    
class HashString{
private:const char* cstring;
public: 
    HashString(){cstring="";};
    HashString(const char* _cstring){
        
       cstring=_cstring; 
    }
    unsigned int geef_exhashindex()const{
        unsigned int even = 0;
        int len = strlen(cstring);
        for (int i = 0; i < len; i++) {
            unsigned int pl = cstring[i];
            pl = pl * 8 + pl / 32; //eerste  4 bits achteraan;
            even = even * 253 + pl;
            //pl*((unsigned int)1 << 24)+even/37;
        }
        unsigned int even2 = 0;
        int aantbits = sizeof (int)*8;
        for (int i = 0; i < aantbits / 2; i++)
            even2 += (even >> i)*((unsigned int) 1 << (aantbits - 1 - i));
        return even2^even;
    }
    const char* getString()const{return cstring;}
    bool operator==(const HashString& hs)const{
         if(strcmp(hs.getString(),cstring)==0)return true;
         else false;
    }
    bool operator<(const HashString& hs)const{
         if(strcmp(cstring,hs.getString())<0)return true;
         else false;
   }
    
};


