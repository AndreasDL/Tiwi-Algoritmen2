#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <map>
#include <stack>
#include <exception>


using namespace std;

enum RichtType { GERICHT, ONGERICHT };


class GraafExceptie : public std::logic_error {
public:
    GraafExceptie(const std::string &boodschap_) : std::logic_error(boodschap_) {}
};

std::ostream &operator<<(std::ostream &os, const GraafExceptie& exc){
    return os << exc.what();
}


//------------------------------------- PAD ------------------------------------------ #
//template< class Takdata >
template< class T >
class Pad:public std::vector< int >{ //Een Pad is een reeks van knoopnummers.
public:
    T geefCapaciteit() const{
        return capaciteit;
    }
    void zetCapaciteit(const T& _capaciteit){
        capaciteit=_capaciteit;
    }
	friend ostream& operator<<(ostream& os, const Pad& p){
		os<<"Capaciteit= "<<p.capaciteit<<" :: ";
		if (p.size() > 0){
			os<<p[0];
		}
		for (int i=1; i<p.size(); i++ ){
			os<<"->"<<p[i];
		}
		os<<"\n";
	}
protected:
    T capaciteit;
};


template<RichtType RT,class Takdata>
class GraafMetTakdata;

//----------------------- Vergrotendpadzoeker ----------------------------- #
template <class T>
class Vergrotendpadzoeker{
public:
	//Geef RestGraaf mee, van, naar knoopnummer en _pad is uitvoerparameter
	Vergrotendpadzoeker(const GraafMetTakdata<GERICHT,T>& _gr, int _van, int _naar, Pad<T>& _pad):
	pad(_pad), gr(_gr),van(_van),naar(_naar),
	voorganger(gr.aantalKnopen()), bezocht(gr.aantalKnopen(),false){
		pad.clear();
		verwerk(van,0);
		
		if (pad.size() > 1){
			T capaciteit=*gr.geefTakdata(pad[0],pad[1]);
			for (int i=2; i<pad.size(); i++ ){
				T nucapaciteit=*gr.geefTakdata(pad[i-1],pad[i]);
				if (nucapaciteit<capaciteit)
					capaciteit=nucapaciteit;
				//            assert(capaciteit > 0);
			}
			pad.zetCapaciteit(capaciteit);
		}
	}
	
	void verwerk(int knoopnr, int diepte){ 					//Diepte eerst zoeken naar vergrotend pad
		//    std::cerr <<" knoopnr "<<knoopnr  <<" <? "<< gr.aantalKnopen()<<"\n";
		//    assert(knoopnr < gr.aantalKnopen());
		bezocht[knoopnr]=true;
		const typename GraafMetTakdata<GERICHT,T>::Knoop& kn=gr[knoopnr];
		int nudiepte=diepte+1;
		for (typename GraafMetTakdata<GERICHT,T>::Knoop::const_iterator it=kn.begin(); it!=kn.end();it++){
			int kind=it->first;
			if (*gr.geefTakdata(knoopnr,kind)> 0){ // er is nog een weg
				if (it->first==naar && nudiepte+1 > pad.size()){
					voorganger[naar]=knoopnr;
					pad.resize(nudiepte+1);
					int nunr=naar;
					int i=nudiepte;
					while (nunr!=van){
						pad[i--]=nunr;
						nunr=voorganger[nunr];
					}
					//  assert(i==0);
					//  assert(nunr==van);
					pad[0]=nunr;
				}
				else if(!bezocht[kind]){ //niet bezocht
					//  assert(*gr.geefTakdata(knoopnr,kind)> 0);
					voorganger[kind]=knoopnr;
					verwerk(kind,nudiepte);   // Recursief
				}
			}//if takdata> 0
		}//for
	}
	
	const GraafMetTakdata<GERICHT,T>& gr;
	Pad<T>& pad;
	vector<int> voorganger;
	vector<bool> bezocht;
	int van,naar;
};



// ---------------------------- GRAAF ----------------------------------//

template<RichtType RT>
class Graaf{
public:
	typedef std::map<int, int>  Knoop;      // beeldt knoopnummer (van buur) af op verbindingsnummer
    // Construeert een graaf met gegeven RichtType en aantal knopen (default 0), zonder verbindingen.
    Graaf(int n=0);
    
    // Geeft true indien het richttype GERICHT is, false indien het ONGERICHT is.
    bool isGericht() const;
	
    // Voegt een nieuwe 'lege' knoop toe, d.w.z. zonder verbindingen.
    // Geeft knoopnummer van toegevoegde knoop terug (begint bij 0).
    virtual int voegKnoopToe();
    
    // Voegt verbinding toe tussen knoopnummers 'van' en 'naar'.
    // Gooit GraafExceptie indien verbinding al bestaat of knoopnummers ongeldig zijn.
    // Geeft verbindingsnummer van toegevoegde verbinding terug (begint bij 0).
    // Bij ongerichte graaf wordt terugverbinding ook toegevoegd (met zelfde verbindingnummer!)
    virtual int voegVerbindingToe(int van, int naar);
	
    // Verwijdert de verbinding tussen knoopnummers 'van' en 'naar', incl. de terugverbinding indien ongericht.
    // Gooit GraafExceptie indien knoopnummers ongeldig zijn.
    // Geen exceptie indien de verbinding niet bestond.
    // Andere verbindingen worden niet 'hernummerd'. Er komen dus mogelijks "gaten" in de verbindingsnummers.
    virtual void verwijderVerbinding(int van, int naar);
	
    // Geeft het aantal knopen van de graaf.
    int aantalKnopen() const;
	
    // Geeft het aantal verbindingen van de graaf.
    // Bij ongerichte graaf worden verbindingen NIET dubbel geteld!
    int aantalVerbindingen() const;
	
    // Geeft het verbindingsnummer van de verbinding tussen 'van' en 'naar'.
    // Geeft -1 indien die verbinding niet bestaat.
    // Gooit een GraafExceptie indien knoopnummers ongeldig zijn.
    // Opgelet: performantie is O(log(v)) waarbij v aantal verbindingen vanuit 'van' is.
    int verbindingsnummer(int van, int naar) const;
	
    // Verwijdert alle knopen en verbindingen en herstart de verbindingsnummer
    virtual void wis();
	
    // Toegang tot de knopen:
    const Knoop &operator[](int i) const { return knopen[i]; }
	Knoop &operator[](int i)       { return knopen[i]; }  // deze kan als lvalue gebruikt worden
	
	
    // Schrijft de gegevens van de volledige graaf naar outputstream os.
    virtual void schrijf(std::ostream &os) const;
	
    // Schrijft de gegevens van de knoop met knoopnummer k naar outputstream os.
    virtual void schrijfKnoop(std::ostream &os, int k) const;
    
    // Schrijft de gegevens van de verbinding met verbindingsnummer v naar outputstream os.
    virtual void schrijfVerbinding(std::ostream &os, int v) const;
	
    
protected:
    // hulpfuncties
    void controleerKnoopnummer(int k) const;   // throw indien k ongeldig
    void voegVerbindingToeInDatastructuur(int van, int naar,int verbindingsnummer);
    void verwijderVerbindingUitDatastructuur(int van, int naar);
protected:
    //datavelden
    std::vector<Knoop>  knopen;
    int                 hoogsteVerbindingsnummer;
    RichtType           richttype;
    std::stack<int>     vrijgekomenVerbindingsnummers;
};


template<RichtType RT>
std::ostream &operator<<(std::ostream& os, const Graaf<RT>& g);


// --- implementatie ---


template<RichtType RT>
void Graaf<RT>::controleerKnoopnummer(int k) const{
    if (k<0 || (size_t)k>=knopen.size())
        throw GraafExceptie("ongeldig knoopnummer");
}

template<RichtType RT>
Graaf<RT>::Graaf(int n) : knopen(n), hoogsteVerbindingsnummer(0){}

template<RichtType RT>
bool Graaf<RT>::isGericht() const { return true; }//voor gerichte graaf

template<>
bool Graaf<ONGERICHT>::isGericht() const { return false; }//voor ongerichte graaf


template<RichtType RT>
int Graaf<RT>::voegKnoopToe(){
    int n = knopen.size();
    knopen.resize(n+1); // default constructor voor nieuwe knoop wordt opgeroepen (hier lege map)
    return n;
}


template<RichtType RT>
int Graaf<RT>::voegVerbindingToe(int van, int naar){
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);
	
    if (knopen[van].count(naar) > 0)
    {
        std::ostringstream out;
        out << "verbinding " << van << "-" << naar << " bestaat al";
        throw GraafExceptie(out.str());
    }
    else {
        int verbindingsnummer;
        if (!vrijgekomenVerbindingsnummers.empty()){
            verbindingsnummer=vrijgekomenVerbindingsnummers.top();
            vrijgekomenVerbindingsnummers.pop();
        }else
            verbindingsnummer=hoogsteVerbindingsnummer++;
        voegVerbindingToeInDatastructuur(van,naar,verbindingsnummer);
        return verbindingsnummer;
    }
}

template<RichtType RT>//voor gerichte graaf
void Graaf<RT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer){
    knopen[van][naar] = verbindingsnummer;
}

template<>
void Graaf<ONGERICHT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer){
    knopen[van][naar] = verbindingsnummer;
    knopen[naar][van] = verbindingsnummer;
}

template<RichtType RT>
void Graaf<RT>::verwijderVerbinding(int van, int naar){
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);
    if (knopen[van].find(naar)!=knopen[van].end()){//verbinding bestaat
        vrijgekomenVerbindingsnummers.push(knopen[van][naar]);
        verwijderVerbindingUitDatastructuur(van,naar);
    }
    // (geen exception indien verbinding niet bestaat)
}

template<RichtType RT>
void Graaf<RT>::verwijderVerbindingUitDatastructuur(int van, int naar){
    knopen[van].erase(naar);
}

template<>
void Graaf<ONGERICHT>::verwijderVerbindingUitDatastructuur(int van, int naar){
    knopen[van].erase(naar);
    knopen[naar].erase(van);
}

template<RichtType RT>
int Graaf<RT>::aantalKnopen() const { return knopen.size(); }

template<RichtType RT>
int Graaf<RT>::aantalVerbindingen() const {
    return hoogsteVerbindingsnummer-vrijgekomenVerbindingsnummers.size();
}

template<RichtType RT>
int Graaf<RT>::verbindingsnummer(int van, int naar) const{
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);
    Knoop::const_iterator verbindingit = knopen[van].find(naar);
    
    if (verbindingit == knopen[van].end())
        return -1;
    else
        return (*verbindingit).second;
}

template<RichtType RT>
void Graaf<RT>::wis(){
    knopen.clear();
    hoogsteVerbindingsnummer = 0;
    while (!vrijgekomenVerbindingsnummers.empty())
        vrijgekomenVerbindingsnummers.pop();
}

template<RichtType RT>
void Graaf<RT>::schrijf(std::ostream &os) const{
    os << "Graaf: " << aantalKnopen() << " knopen en "
	<< aantalVerbindingen() << " verbindingen:" << std::endl;
    for (int k=0; k<aantalKnopen(); k++)
        schrijfKnoop(os, k);
}

template<RichtType RT>
void Graaf<RT>::schrijfKnoop(std::ostream &os, int k) const{
    os << "knoop " << k << ":" << std::endl;
    for (Knoop::const_iterator it=knopen[k].begin(); it!=knopen[k].end(); ++it)
    {
        os << "  ->" << it->first;
        schrijfVerbinding(os, it->second);
    }
}

template<RichtType RT>
void Graaf<RT>::schrijfVerbinding(std::ostream &os, int v) const{
    os << " via " << v << std::endl;
}

template<RichtType RT>
std::ostream &operator<<(std::ostream &os, const Graaf<RT> &g){
    g.schrijf(os);
    return os;
}


// ---------------------------- GraafMetTakdata ----------------------------------//



template<RichtType RT,class Takdata>
class GraafMetTakdata: public virtual Graaf<RT>{
public:
    // Construeert een graaf met gegeven RichtType en aantal knopen (default 0), zonder verbindingen.
    GraafMetTakdata(int n=0):Graaf<RT>(n){};
    //Noot: toevoegfunctie zonder takdata op te geven kan alleen gebruikt als de klasse
    //      Takdata een defaultconstructor heeft.
    virtual int voegVerbindingToe(int van, int naar);
    virtual int voegVerbindingToe(int van, int naar, const Takdata&);
    //Noot: verwijderVerbinding wordt ongewijzigd overgenomen van Graaf!
	
    //TakData vrijgeven (eventueel voor wijziging). Geeft nullpointer als tak niet bestaat
    //Noot: pointers teruggegeven door geefTakdata worden ongeldig
    //door toevoegen van een tak.
    const Takdata* geefTakdata(int van,int naar) const;
	Takdata* geefTakdata(int van,int naar)      ;
	
    virtual void wis();
	
	/*void zetAantalKnopen(int aantal) {
	 for (int i = 0; i < aantal) {
	 
	 }
	 }*/
	
	// ********************** TOEGEVOEGD *********************** //
	
	void wordMaxStroomVan(const GraafMetTakdata<RT,Takdata>& netwerk,int producent, int verbruiker);
	void geefVergrotendPad(Pad<Takdata>& vergrotendPad, int producent, int verbruiker);
	Takdata berekenMaxStroomVan(int producentKnoopNr);
    // Schrijft de gegevens van de verbinding met verbindingsnummer v naar outputstream os.
    virtual void schrijfVerbinding(std::ostream &os, int v) const;
	
protected:
    std::vector<Takdata> takdatavector;
};

template<RichtType RT,class Takdata>
int GraafMetTakdata<RT,Takdata>::voegVerbindingToe(int van, int naar){
    return this->voegVerbindingToe(van,naar,Takdata());
}

template<RichtType RT,class Takdata>
int GraafMetTakdata<RT,Takdata>::voegVerbindingToe(int van, int naar, const Takdata& td){
    bool isnieuwtaknummer=this->vrijgekomenVerbindingsnummers.empty();
    int taknummer=Graaf<RT>::voegVerbindingToe(van,naar);
    if (isnieuwtaknummer)
        takdatavector.push_back(td);
    else
        takdatavector[taknummer]=td;
	
	return taknummer;
}

template<RichtType RT,class Takdata>
const Takdata* GraafMetTakdata<RT,Takdata>::geefTakdata(int van,int naar) const{
    int taknummer=this->verbindingsnummer(van,naar);
    if (taknummer!=-1)
        return &takdatavector[taknummer];
    else
        return 0;
}

template<RichtType RT,class Takdata>
Takdata* GraafMetTakdata<RT,Takdata>::geefTakdata(int van,int naar){
    int taknummer=this->verbindingsnummer(van,naar);
    if (taknummer!=-1)
        return &takdatavector[taknummer];
    else
        return 0;
}


template<RichtType RT,class Takdata>
void GraafMetTakdata<RT,Takdata>::wis(){
    Graaf<RT>::wis();
    takdatavector.clear();
}

template<RichtType RT,class Takdata>
void  GraafMetTakdata<RT,Takdata>::schrijfVerbinding(std::ostream &os, int v) const{
    os << " via " << v <<"(Data: "<<takdatavector[v]<<")"<< std::endl;
}

// ********************** TOEGEVOEGD *********************** //


template <RichtType RT,class Takdata>
void GraafMetTakdata<RT,Takdata>::wordMaxStroomVan(const GraafMetTakdata<RT,Takdata>& netwerk,int producent, int verbruiker){
	//    if (!isGericht())
    //   throw GraafExceptie("Ongerichte grafen zijn geen stroomnetwerken.");
	
    //zetAantalKnopen(netwerk.aantalKnopen());   // this => lege graaf die we willen opvullen
    for(int i=0;i<netwerk.aantalKnopen();i++){
        this->voegKnoopToe();
    }
    
    GraafMetTakdata<RT,Takdata> restnetwerk=netwerk;   // originele graaf kopieren in de restnetwerk om te starten
    Pad<Takdata> vergrotendPad;   //Uitvoerparameter pad!!!!!!!!!!!
	
    restnetwerk.geefVergrotendPad(vergrotendPad,producent,verbruiker);
    //cout << "RESTNETWERK:\n" << restnetwerk;
    //cout << vergrotendPad;
    
    while(vergrotendPad.size() !=0 ){ //zolang pad bestaat v P->V
        //restnetwerk-=vergrotendPad;
        int van, naar;
        for(int i=0; i<vergrotendPad.size()-1;i++){
            van = vergrotendPad[i];
            naar = vergrotendPad[i+1];
            
            Takdata * huidigeCapaciteit = restnetwerk.geefTakdata(van,naar);
            *huidigeCapaciteit-=vergrotendPad.geefCapaciteit();
			
			//voeg terugverbindingen toe
			/*	Takdata * capaciteitTerugVerbinding = restnetwerk.geefTakdata(naar,van);
			 if(capaciteitTerugVerbinding == 0){ //nog gn verbinding
			 restnetwerk.voegVerbindingToe(naar,van,vergrotendPad.geefCapaciteit());
			 }else{
			 *capaciteitTerugVerbinding+=vergrotendPad.geefCapaciteit();
			 }   */
        }
        
        //*this+=vergrotendPad;
        for(int i=0; i<vergrotendPad.size()-1;i++){
            van = vergrotendPad[i];
            naar = vergrotendPad[i+1];
            
            Takdata * huidigeCapaciteit = this->geefTakdata(van,naar);
            if(huidigeCapaciteit == 0){ //nog gn verbinding
                this->voegVerbindingToe(van,naar,vergrotendPad.geefCapaciteit());
            }else{
                *huidigeCapaciteit+=vergrotendPad.geefCapaciteit();
            }
			
        }
		
        restnetwerk.geefVergrotendPad(vergrotendPad,producent,verbruiker); // kijk of je nu nog een pad kan vinden
        //cout << "RESTNETWERK:\n" << restnetwerk;
        //cout << vergrotendPad;
    }
	
}


template <RichtType RT,class Takdata>
void GraafMetTakdata<RT,Takdata>::geefVergrotendPad(Pad<Takdata>& vergrotendPad, int producent, int verbruiker) {
	//Vergrotendpadzoeker(*this, producent, verbruiker,  vergrotendPad);
	Vergrotendpadzoeker<Takdata>(*this, producent, verbruiker,  vergrotendPad);
}


template <RichtType RT,class Takdata>
Takdata GraafMetTakdata<RT,Takdata>::berekenMaxStroomVan(int producentKnoopNr){
	Takdata som=0;
	for (std::map<int, int>::const_iterator it=this->knopen[producentKnoopNr].begin(); it!=this->knopen[producentKnoopNr].end(); ++it)
    {
	    som += takdatavector[it->second];
    }
    return som;
}



// ---------------------------- GraafMetKnoopdata ----------------------------------//


template<RichtType RT, class Knoopdata>
class GraafMetKnoopdata:public virtual Graaf<RT>{
public:
    // Construeert een graaf met gegeven RichtType, en lijst van Knoopdata;
    template<class InputIterator>
    GraafMetKnoopdata(InputIterator start,InputIterator eind);
    GraafMetKnoopdata():Graaf<RT>(){};
    
    virtual int voegKnoopToe();
    virtual int voegKnoopToe(const Knoopdata&);
	
    const Knoopdata* geefKnoopdata(int knoopnr) const;
	Knoopdata* geefKnoopdata(int knoopnr)      ;
    virtual void wis();
    virtual void schrijfKnoop(std::ostream &os, int k) const;
	
protected:
    //datavelden
    std::vector<Knoopdata>  knoopdatavector;
};
template<RichtType RT, class Knoopdata>
template<class InputIterator>
GraafMetKnoopdata<RT,Knoopdata>::GraafMetKnoopdata(InputIterator start,InputIterator eind)
:Graaf<RT>(0){
	for(;start!=eind;start++)
		voegKnoopToe(*start);
}

template<RichtType RT, class Knoopdata>
int GraafMetKnoopdata<RT,Knoopdata>::voegKnoopToe(){
    return this->voegKnoopToe(Knoopdata());
}

template<RichtType RT, class Knoopdata>
int GraafMetKnoopdata<RT,Knoopdata>::voegKnoopToe(const Knoopdata& kd){
    int ret=Graaf<RT>::voegKnoopToe();
    knoopdatavector.push_back(kd);
    return ret;
}

template<RichtType RT,class Knoopdata>
const Knoopdata* GraafMetKnoopdata<RT,Knoopdata>::geefKnoopdata(int knoopnummer) const{
    this->controleerKnoopnummer(knoopnummer);
    return &knoopdatavector[knoopnummer];
}

template<RichtType RT,class Knoopdata>
Knoopdata* GraafMetKnoopdata<RT,Knoopdata>::geefKnoopdata(int knoopnummer){
    this->controleerKnoopnummer(knoopnummer);
    return &knoopdatavector[knoopnummer];
}


template<RichtType RT,class Knoopdata>
void GraafMetKnoopdata<RT,Knoopdata>::wis(){
    Graaf<RT>::wis();
    knoopdatavector.clear();
}


template<RichtType RT, class Knoopdata>
void GraafMetKnoopdata<RT,Knoopdata>::schrijfKnoop(std::ostream &os, int k) const{
    os << "knoop " << k <<"(Data: "<<knoopdatavector[k]<< "):" << std::endl;
    for (std::map<int, int>::const_iterator it=this->knopen[k].begin(); it!=this->knopen[k].end(); ++it){
        os << "  ->" << it->first;
        this->schrijfVerbinding(os, it->second);
    }
}

template<RichtType RT, class Knoopdata, class Takdata>
class GraafMetKnoopEnTakdata:public GraafMetKnoopdata<RT,Knoopdata>,
public GraafMetTakdata<RT, Takdata>{
public:
    template<class InputIterator>
    GraafMetKnoopEnTakdata(InputIterator start,InputIterator eind):
	GraafMetKnoopdata<RT,Knoopdata>(start,eind){}
    GraafMetKnoopEnTakdata():
	GraafMetKnoopdata<RT,Knoopdata>(){}
    virtual void wis(){
        GraafMetKnoopdata<RT,Knoopdata>::wis();
        this->takdatavector.clear();
    }
};


int main()
{
	GraafMetTakdata<GERICHT,int> g(8);
	g.voegVerbindingToe(0,1,7);
	g.voegVerbindingToe(0,2,10);
	
	g.voegVerbindingToe(1,4,4);
	
	g.voegVerbindingToe(2,1,3);
	g.voegVerbindingToe(2,3,6);
	g.voegVerbindingToe(2,5,5);
	
	g.voegVerbindingToe(3,0,2);
	g.voegVerbindingToe(3,6,6);
	
	g.voegVerbindingToe(4,5,6);
	
	g.voegVerbindingToe(5,7,8);
	
	g.voegVerbindingToe(6,5,2);
	g.voegVerbindingToe(6,7,7);
	
	g.voegVerbindingToe(7,4,1);
	
	GraafMetTakdata<GERICHT, int>f;
	
	f.wordMaxStroomVan(g, 0, 7);
	cout << "RESULTAAT:\n" << f;
	cout << f.berekenMaxStroomVan(0);
	
	return 0;
}