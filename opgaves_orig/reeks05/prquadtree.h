#ifndef __QUADTREE
#define __QUADTREE
//Quadtree voor punten met int-co"ordinaten
//de PRquadtree kan alleen punten bevatten met
//-maxcoordinaat <= x < maxcoordinaat
//-maxcoordinaat <= y < maxcoordinaat
class PRquadtree{
public:
    PRquadtree(int _maxcoordinaat):maxcoordinaat(_maxcoordinaat),k(0){};
//voegtoe veronderstelt dat het punt in het gebied ligt.
    void voegtoe(int x,int y);
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
    private:
        static const int OOST, WEST, NOORD, ZUID;
        Knoop* kind[4];//indexeren met windrichting (bv. kind[NOORD+WEST] bevat punten
                          //met x en y kleiner dan grenswaarde)
                          //leeg gebied: nulpointer
    };
    const int maxcoordinaat;//wordt opgegeven in constructor;
    Knoop* k;
    
};
const int PRquadtree::Nietblad::OOST=??;
const int PRquadtree::Nietblad::WEST=??;
const int PRquadtree::Nietblad::NOORD=??;
const int PRquadtree::Nietblad::ZUID=??;

#endif

