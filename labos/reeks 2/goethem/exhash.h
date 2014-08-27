#include <cstdlib>
#include <iostream>
#include <queue>

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
    int aantalKnopen;//aantal dat nu in pagina zit;
    int k;//aantal bits in hashwaarde;
    unsigned int index;//laatste k bits zijn belangrijk, die daarvoor zijn nul
    struct knoop{
        T sleutel;
        D data;
    };
    //we werken hier rechtstreeks; andere oplossing zou zijn een tabel
    //met pointers naar de eigenlijke waarden bij te houden.
    knoop kn[PAGINAGROOTTE];
    int geefk(){
        return k;
    }
    Schijfpagina():index(0),k(0),aantalKnopen(0){
    }

//de huidige Schijfpagina wordt in twee gesplitst. De nieuwe hashindex is
//dus een bit langer; *this krijgt de sleutelwaarden met hashindex
//eindigend op nul. De nieuwe pagina, met hashindex eindigend op 1,
//wordt aan eenbit gehangen.

    void splits(Schijfpagina<T,D>*& eenbit) {
		//alle sleutels met een 1 op plaats index verplaatsen naar nieuwe pagina
		k++;
		eenbit->k = k;
		index *= 2;
		eenbit->index = index + 1;
		int i = 0;
		while (i < aantalKnopen) {
			if ((kn[i].sleutel.geefExhashindex()>>(AANTBITS-k)) & 1) { //eindigt op 1 => verplaatsen naar nieuwe pagina
				eenbit->voegtoe(kn[i].sleutel, kn[i].data);
				verwijder(kn[i].sleutel);
			}
			else {
				i++;
			}
		}
	}

    bool isVol() const{
        return (aantalKnopen==PAGINAGROOTTE);
    }
 
    void zoek(const T& sleutel,int& plaats, bool& gevonden) const{
        plaats=0;
        while (plaats<aantalKnopen && kn[plaats].sleutel < sleutel)
            plaats++;
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
   // dit is de taak van Exhash

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

	void schrijf() {
		cout << endl;
		for (int i = 0; i < aantalKnopen; i++) {
			cout << kn[i].sleutel.getString().c_str() << "\t";
		}
		cout << endl;
	}
};

template <class T,class D>
class Exhash{
    friend class Schijfpagina<T,D>;
    private: Schijfpagina<T,D>** tab;
    private: int maxk;
    private: int grootte;//grootte tabel; volgt uit maxk, maar alla
    private: int aantalSleutels;
	private: int aantalPag;
public: Exhash():maxk(0),grootte(1),aantalSleutels(0),aantalPag(1){
        tab=new Schijfpagina<T,D>*[1];
        tab[0]=new Schijfpagina<T,D>;
    };

    public: ~Exhash();

//index geeft de index in de tabel. kan als voorbeeld dienen
//voor schuifoperaties
    private: unsigned int index(const T& sleutel) const{
        if (maxk==0)//computer weigert 32 plaatsen shift
            return 0;
        else{
            unsigned int q=sleutel.geefExhashindex();
            return (q>>(AANTBITS-maxk));
        }
    };

    private: void verdubbel();
    
    public: void voegtoe(const T& sleutel,const D& data);

// verwijder: lege pagina's blijven bestaan.
    public: void verwijder(const T& sleutel);

    public: D* zoekData(const T& sleutel);
};
  
template <class T,class D>
Exhash<T,D>::~Exhash() {
	//monitoring
	cout << "Aantal pagina's: " << aantalPag << endl
		 << "Aantal sleutels: " << aantalSleutels << endl;

	/*
	for (int i = 0; i < grootte; i++) {
		tab[i]->schrijf();
	}*/
	delete[] tab;
}

template <class T,class D>
void Exhash<T,D>::verdubbel() {
	//dubbel aantal pointers voorzien in tab
	//maxk++
	//de nieuwe pointers verwijzen naar bestaande pagina's
	maxk++;
	grootte *= 2;
	Schijfpagina<T,D>** nieuweTab = new Schijfpagina<T,D>*[grootte];
	for (int i = 0; i < grootte; i += 2) {
		nieuweTab[i] = tab[i / 2];
		nieuweTab[i + 1] = tab[i / 2];
	}
	delete tab;
	tab = nieuweTab;
}

template <class T,class D>
void Exhash<T,D>::voegtoe(const T& sleutel,const D& data) {
	//vergelijk het gedefinieerd aantal bits
	//indien er niet genoeg plaats is in de pagina => verdubbel
	//en splits de volle pagina
	unsigned int i = index(sleutel);
	Schijfpagina<T,D>* pagina = tab[i];
	while (pagina->isVol()) {
		/*
		cout << "VOOR" << endl;
		for (int j = 0; j < grootte; j++) {
			tab[j]->schrijf();
		}
		cout  << "----------------" << endl;*/
		verdubbel();
		Schijfpagina<T,D>* nieuwePagina = new Schijfpagina<T,D>();
		pagina->splits(nieuwePagina);
		
		int aantal_pointers = pow(2, maxk - pagina->geefk());
		for (int j = 1; j <= aantal_pointers; j++) {
			tab[2*i + j] = nieuwePagina;
		}
		/*
		cout << "NA" << endl;
		for (int j = 0; j < grootte; j++) {
			tab[j]->schrijf();
		}
		cout  << "----------------" << endl;*/
		i = index(sleutel);
		pagina = tab[i];
		aantalPag++;
	}

	pagina->voegtoe(sleutel, data);
	aantalSleutels++;
	/*
	cout << "EIND" << endl;
	for (int j = 0; j < grootte; j++) {
		tab[j]->schrijf();
	}
	cout  << "----------------" << endl;*/
	
}

template <class T,class D>
void Exhash<T,D>::verwijder(const T& sleutel) {
	tab[index(sleutel)]->verwijder(sleutel);
	aantalSleutels--;
}

template <class T,class D>
D* Exhash<T,D>::zoekData(const T& sleutel) {
	return tab[index(sleutel)]->zoekData(sleutel);
}

class ExhashString {
public:
	bool operator<(const ExhashString& exhashstr) const {
		return (str.compare(exhashstr.str) < 0);
	}
	bool operator==(const ExhashString& exhashstr) const {
		return (str.compare(exhashstr.str) == 0);
	}
	bool operator>(const ExhashString& exhashstr) const {
		return (str.compare(exhashstr.str) > 0);
	}
	std::string getString() const { return str; }
	void setString(std::string _str) { str = _str; }
	unsigned int geefExhashindex() const {
		const char* s = str.c_str();
		unsigned int even=0;
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
private:
	std::string str;
};