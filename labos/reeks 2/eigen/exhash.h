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
//eindigend op nul. De nieuwe pagina, met hasindex eindigend op 1,
//wordt aan eenbit gehangen.

    void splits(Schijfpagina<T,D>*& eenbit){
         //alles overlopen
         for (int i = 0 ; i < PAGINAGROOTTE ; i++){
             //blijven / verzetten  
             if((kn[i].sleutel.geefExhashindex()>>(sizeof(T)*8-i)) & 1 ){
             //true => 1 verzetten
                    eenbit->voegtoe(kn[i].sleutel, kn[i].data);
                    verwijder(kn[i].sleutel); //mag niet !!
             }
         }
         //k verzetten
         k++;
         eenbit->k = k;
    }

    void schrijf()const {
        for (int i = 0; i < PAGINAGROOTTE; i++){
            std::cout << kn[i].sleutel.geefString() << " " << kn[i].data << std::endl;
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
};

template <class T,class D>
class Exhash{
    friend class Schijfpagina<T,D>;
    private: Schijfpagina<T,D>** tab;
    private: int maxk; //breedte van de tabel, max aantal bits van de diepste pagina
    private: int grootte;//grootte tabel; volgt uit maxk, maar alla
    private: int aantalSleutels;
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
            unsigned int q=sleutel.geefExhashindex();
            return (q>>(AANTBITS-maxk));
        }
    };

    private: void verdubbel(){
        //oude bijhouden
        Schijfpagina<T,D>** t = tab;
        
        //nieuwe maken
        tab=new Schijfpagina<T,D>*[grootte*2];
        grootte *= 2; //grootte fixen
        maxk++;
        
        //pointers overnemen & verdubbelen
        for (int i = 0 ; i < grootte ; i+=2){
            tab[i] = t[i/2];
            tab[i+1] = t[i/2];
        }
        
        //oude wegsmijten
        delete[] t;
    }
    
    public: void voegtoe(const T& sleutel,const D& data){
            aantalSleutels++;
            //pagina zoeken
            int index = zoekPaginaIndex(sleutel);
            Schijfpagina<T,D>* pag = tab[index];
            
            //isvol?
            if (!pag->isVol()){
               pag->voegtoe(sleutel,data);
            }else{
            //verdubbel?
            //index even => ++ kijken of gelijk               
            //index oneven => -- kijken of gelijk
	            int a = index%2?+1:-1;
	            if (tab[index] != tab[index+a]){//kijken of pagina onderaan zit
	            
	                 verdubbel();
	                 index = zoekPaginaIndex(sleutel);
	                 pag = tab[index];
	                 a = index%2?+1:-1;
	                 std::cout << "a " << a << std::endl;
	            }
            
	            //pag zowieso gelijk
	            
	            //splits?
	            Schijfpagina<T,D>* p = new Schijfpagina<T,D>;
	            //herordenen
	            pag->splits(p);
	            //pointer verleggen
	            a = index%2?+1:0;
	            tab[index+a] = p;//nieuwel pagina altijd rechts
	            
	            voegtoe(sleutel,data);//indien nog vol => recursie
            }
    }

// verwijder: lege pagina's blijven bestaan.
    public: void verwijder(const T& sleutel){
            aantalSleutels--;//niet altijd -- eerst kijken of het erin zit
            //verwijderen uit pagina?
            Schijfpagina<T,D>* pag = tab[zoekPaginaIndex(sleutel)];
            pag->verwijder(sleutel);
    }

    public: D* zoekData(const T& sleutel){
            Schijfpagina<T,D>* pag = tab[zoekPaginaIndex(sleutel)];
            return pag->zoekData(sleutel);
    }
    
    private: int zoekPaginaIndex(const T& sleutel){//geeft index
        int index = 0;
        int hulp = 1;//index zoeken => altijd helft van de deeltabel
        
        for (int i = 1 ; i <= maxk ; i++){
            hulp*=2;
            if((sleutel.geefExhashindex()>>(sizeof(T)*8-i)) & 1){
               //true => 1   
               index += grootte / hulp;
            }
        }
        return index;
    }

    public: void write(){
        for (int i = 0; i < grootte ; i++ ){
            std::cout << "pagina " << i << std::endl;
            tab[i]->schrijf();
            std::cout << std::endl << std::endl;
        }
    }
};
