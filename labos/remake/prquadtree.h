#ifndef __QUADTREE
#define __QUADTREE

#include <iostream>
using namespace std;
//Quadtree voor punten met int-co"ordinaten
//de PRquadtree kan alleen punten bevatten met
//-maxcoordinaat <= x < maxcoordinaat
//-maxcoordinaat <= y < maxcoordinaat
const int OOST=0;
const int WEST=1;
const int NOORD=0;
const int ZUID=2;

class PRquadtree {
public:
    PRquadtree(int _maxcoordinaat):maxcoordinaat(_maxcoordinaat),k(0){};
//voegtoe veronderstelt dat het punt in het gebied ligt.
    void voegtoe(int x,int y){
    	//init
		Gebied g(maxcoordinaat);
		int index = g.geefKwadrant(x,y);
		Nietblad* hulp;
		
		if (k == 0){//lege boom easy fix
			hulp = 	new Nietblad();
			hulp->kind[index] = new Blad(x,y);
			k = hulp;
		}else{//niet leeg
			hulp = static_cast<Nietblad*>(k);
			
			while (hulp->kind[index] != 0){//zolang we kindjes ebben => 2 gevallen
				if (hulp->kind[index]->isblad()){
					//blad apart zetten
					Blad* bladje = static_cast<Blad*>(hulp->kind[index]);
					int bladIndex = g.geefKwadrant(bladje->x,bladje->y);
					
					//nietblad insteken zolang ze binnen zelfde kwadrant vallen
					while (index == bladIndex){
						//nieuw blad
						hulp->kind[index] = new Nietblad();
						
						//doorschuiven
						hulp = static_cast<Nietblad*>(hulp->kind[index]);
						g.maakdeelgebied(x,y);
						index = g.geefKwadrant(x,y);
						bladIndex = g.geefKwadrant(bladje->x,bladje->y);
					}
					
					//originele terug zetten
					hulp->kind[bladIndex] = bladje;
					//andere toevoegen na de while
				}else{
					//niet blad gevonden => doorschuiven
					hulp = static_cast<Nietblad*>(hulp->kind[index]);
					
					g.maakdeelgebied(x,y);
					index = g.geefKwadrant(x,y);
				}				
			}
			
			//nullpointer gevonden => blad insteken
			hulp->kind[index] = new Blad(x,y);
		}

    }
protected:
    class Knoop{
    public:
        virtual bool isblad() const =0;
    };
    class Blad:public Knoop{
    public:
        Blad(int _x,int _y):x(_x),y(_y){};
        virtual bool isblad() const { return true;}
        int x,y;//co"ordinaten punt
    };
    class Nietblad:public Knoop{
    public:
        Nietblad(){
            for (int i=0; i<4; i++ )
                    kind[i]=0;
        }
        virtual bool isblad() const{ return false;}
        Knoop* kind[4];//indexeren met windrichting (bv. kind[NOORD+WEST] bevat punten
                          //met x en y kleiner dan grenswaarde)
                          //leeg gebied: nulpointer
    };
    class Gebied{
		public:
		    Gebied(int maxc):xl(-maxc),xr(maxc),yl(-maxc),yr(maxc),xc(0),yc(0){};
		    void maakdeelgebied(int x,int y){
		            if (x<xc)
		                xr=xc;
		            else
		                xl=xc;
		            if (y<yc)
		                yr=yc;
		            else
		                yl=yc;
		            xc=(xl+xr)/2;
		            yc=(yl+yr)/2;
		    }
		    int geefKwadrant(int x, int y) const{
		        return (x<xc? WEST : OOST) + (y<yc? NOORD: ZUID);
		    }
		private:
		    int xl,xr,yl,yr,xc,yc;//geven grenzen gebied; l,r,c: links, rechts, centraal.
	};
    
    const int maxcoordinaat;//wordt opgegeven in constructor;
    Knoop* k;
    
};



#endif

