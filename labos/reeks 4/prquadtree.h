#ifndef __QUADTREE
#define __QUADTREE
#include <iostream>
using namespace std;
//Quadtree voor punten met int-co"ordinaten
//de PRquadtree kan alleen punten bevatten met
//-maxcoordinaat <= x < maxcoordinaat
//-maxcoordinaat <= y < maxcoordinaat

const int OOST=0;//PRquadtree::Nietblad::
const int WEST=1;//PRquadtree::Nietblad::
const int NOORD=0;//PRquadtree::Nietblad::
const int ZUID=2;//PRquadtree::Nietblad::

class PRquadtree{
public:
    PRquadtree(int _maxcoordinaat):maxcoordinaat(_maxcoordinaat),k(0){};
	//voegtoe veronderstelt dat het punt in het gebied ligt.
    void voegtoe(int x,int y){
    	cout << "adding {" << x << " ; " << y << "} to tree" <<endl;
    	
		Gebied g(maxcoordinaat);
    	int index = g.geefKwadrant(x,y);
    	cout << "\t" << index;
    	
    	
    	if (k == 0){
    		Nietblad* hulp = new Nietblad();
    		hulp->kind[index] = new Blad(x,y);
    		k = hulp;
    	}else{
	    	//Nietblad* h = (Nietblad) k;
	    	Nietblad* h = dynamic_cast<Nietblad*>(k);
	    	
	    	while ( h->kind[index] != 0 ){

	    		//opschuiven
	    		if (h->kind[index]->isblad()){ //blad => opsplitsen
					//cout << "splits";					
					Blad* temp = dynamic_cast<Blad*>(h->kind[index]);//blad later terug toevoegen
					int indextemp = g.geefKwadrant(temp->x, temp->y);
						
					while (index == indextemp) {
						//cout << "splitseninwhile";
						
						h->kind[index] = new Nietblad();
						h = dynamic_cast<Nietblad*>(h->kind[index]);
						g.maakdeelgebied(x,y);
						
						//nieuwe indexen
						index = g.geefKwadrant(x,y);
						indextemp = g.geefKwadrant(temp->x, temp->y);
						cout << "\t" << index;
					}
					
					h->kind[indextemp] = temp;
					//h->kind[index] = new Blad(x,y);
					    			
	    		}else{ //niet blad
	    			//cout << "nietblad";
	    			g.maakdeelgebied(x,y);
	    			h = dynamic_cast<Nietblad*>(h->kind[index]);
	    			index = g.geefKwadrant(x,y);
	    			cout << "\t" << index;
	    		}
			}		
			
			h->kind[index] = new Blad(x,y);
		}
	}
	void write(){
    	write(k);
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
    private:
        static const int OOST, WEST, NOORD, ZUID;
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
	    };
	    int geefKwadrant(int x, int y) const{
	        return (x<xc? WEST : OOST) + (y<yc? NOORD: ZUID);
	    }
	private:
	    int xl,xr,yl,yr,xc,yc;//geven grenzen gebied; l,r,c: links, rechts, centraal.
	};
	
	void write(Knoop* k){
		if (k != 0){
			if (k->isblad()){
				cout << "blad op " << static_cast<Blad*>(k)->x << " ; " << static_cast<Blad*>(k)->y << endl;
			}else{
				for (int i = 0 ; i < 4 ; i++){
					write(static_cast<Nietblad*>(k)->kind[i]);
				}
			}
		}
	}

    const int maxcoordinaat;//wordt opgegeven in constructor;
    Knoop* k;
};

//NO = 0
//NW = 1
//ZO = 2
//ZW = 3
#endif

