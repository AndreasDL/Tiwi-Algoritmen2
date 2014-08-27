#ifndef __BINSEARCHTREE
#define __BINSEARCHTREE
#include <cstdlib>
#include <iostream>
#include <queue>
using std::ostream;
/**********************************************************************

   Klasse: BinTree met BinNode: binaire boom
   
   beschrijving: Domme klasse om de gegevens van een boom te bevatten
                 Omdat ze enkel in een wrapper wordt gebruikt, heeft ze
                 alles publiek staan. Ze mag dan ook nooit buiten een
                 wrapper gebruikt worden!
   
***************************************************************************/

template <class Sleutel,class Data>
class Binknoop;

template <class Sleutel,class Data>
class Binboom{
public:
    Binboom<Sleutel,Data>():k(0){}
    ~Binboom<Sleutel,Data>(){
        delete k;
    }
    
	int depth() const{
        if (k==0)
            return 0;
        else
            return max(k->left.depth(),k->right.depth())+1;
    };

    void write(ostream&) const;
    void write(ostream& os,Binknoop<Sleutel,Data>* kn) const{
        if (kn!=0)
            os<<"("<<kn->sl<<","<<kn->data<<")";
        else
            os<<"(,)";
    }
	
    void roteer(bool left);
//pointer naar wortelknoop
     Binknoop<Sleutel,Data>* k;

};



template <class Sleutel,class Data>
class Binknoop{
    public:
        Sleutel sl;
        Data data;
        Binknoop<Sleutel,Data>* parent;
        Binboom<Sleutel,Data> left,right;
        Binknoop():parent(0){}
        Binknoop(const Sleutel& _sl,const Data& _data):parent(0),sl(_sl),data(_data){}
};

/**********************************************************************

   klasse: BinSearchTree
   
   beschrijving: Dit is een wrapper voor een Binboom,
                 die ervoor zorgt dat de sleutels in de Binboom
                 op volgorde staan
   
***************************************************************************/

template <class Sleutel,class Data>
class BinSearchTree{
public:
     virtual void voegtoe(const Sleutel&,const Data&);
//volgende functie doet niets als er geen knoop is met het gegeven Sleutel
     virtual void remove(const Sleutel&);
//geef pointer naar data horend bij een sleutel
     virtual Data* zoekdata(const Sleutel&);
     int depth() const{return deBinboom.depth();};
     friend ostream& operator<<(ostream& os,BinSearchTree<Sleutel,Data>& b){
        b.deBinboom.write(os);
        return os;
    };
protected:
//searchPosition: geeft adres van boom waaraan knoop hangt, of zou moeten hangen
//en adres van de parent.
     virtual void searchPosition(const Sleutel&,Binboom<Sleutel,Data>*&,Binknoop<Sleutel,Data>*&);
//De binaire boom die de data bevat
     Binboom<Sleutel,Data> deBinboom;
//geeft de voorganger. Veronderstelt dat de linkerboom onder de knoop niet leeg is
     Binboom<Sleutel,Data>* ondervoorganger(Binknoop<Sleutel,Data>*);
};

template <class Sleutel,class Data>
void Binboom<Sleutel,Data>::roteer(bool left){
    Binknoop<Sleutel,Data>* op;
    Binknoop<Sleutel,Data>* neer=k;
    if (left){
        op=k->right.k;
        neer->right.k=op->left.k;
        if (op->left.k!=0)
            op->left.k->parent=neer;
        op->left.k=neer;
    }
    else{
        op=k->left.k;
        neer->left.k=op->right.k;
        if (op->right.k!=0)
            op->right.k->parent=neer;
        op->right.k=neer;
    }
    k=op;
    op->parent=neer->parent;
    neer->parent=op;
}


template <class Sleutel,class Data>
void BinSearchTree<Sleutel,Data>::searchPosition(const Sleutel& sl,Binboom<Sleutel,Data>*& position,
                                         Binknoop<Sleutel,Data>*& parent){
    position=&deBinboom;
    parent=0;
    while (position->k!=0 && position->k->sl!=sl){
        parent=position->k;
        if (sl<position->k->sl)
            position=&(position->k->left);
        else
            position=&(position->k->right);
    }
}
        
template <class Sleutel,class Data>
Data* BinSearchTree<Sleutel,Data>::zoekdata(const Sleutel& sl){
    Binboom<Sleutel,Data>* position;
    Binknoop<Sleutel,Data>* parent;
    searchPosition(sl,position,parent);
    if (position->k==0)
        return 0;
    else return &(position->k->data);
}

template <class Sleutel,class Data>
void BinSearchTree<Sleutel,Data>::voegtoe(const Sleutel& sl,const Data& data){
    Binboom<Sleutel,Data>* position;
    Binknoop<Sleutel,Data>* parent;
    searchPosition(sl,position,parent);
    if (position->k == 0){
        position->k=new Binknoop<Sleutel,Data>(sl,data);
        position->k->parent=parent;
    }
}


template <class Sleutel,class Data>
void BinSearchTree<Sleutel,Data>::remove(const Sleutel& sl){
    Binboom<Sleutel,Data>* position;
    Binknoop<Sleutel,Data>* parent;
    searchPosition(sl,position,parent);
    Binknoop<Sleutel,Data>* weg=position->k;
    if (position->k != 0){
        if (weg->left.k==0){
            position->k=weg->right.k;
            if (position->k != 0)
                position->k->parent=parent;
            weg->right.k=0;
            delete weg;
        }
        else if (weg->right.k==0){
            position->k=weg->left.k;
            if (position->k != 0)
                position->k->parent=parent;
            weg->left.k=0;
            delete weg;
        }
        else{
            //vervangen door voorganger
            Binboom<Sleutel,Data>* voorlink=ondervoorganger(weg);
            Binknoop<Sleutel,Data>* voorganger=voorlink->k;
            if (voorlink!=&(weg->left)){
                voorlink->k=voorganger->left.k;
                if (voorganger->left.k !=0)
                    voorganger->left.k->parent=voorganger->parent;
                //geen test voor volgende twee: deze bestaan!
                position->k=voorganger;
                voorganger->parent=parent;
                voorganger->left.k=weg->left.k;
                voorganger->left.k->parent=voorganger;
                voorganger->right.k=weg->right.k;
                voorganger->right.k->parent=voorganger;
            }
            else{
                voorganger->right.k=weg->right.k;
                weg->right.k->parent=voorganger;
                voorganger->parent=weg->parent;
                position->k=voorganger;
            }
            weg->left.k=0;
            weg->right.k=0;
            delete weg;
        }
    }
}


template <class Sleutel,class Data>
Binboom<Sleutel,Data>* BinSearchTree<Sleutel,Data>::ondervoorganger(Binknoop<Sleutel,Data>* kn){
    Binboom<Sleutel,Data>* onder=&(kn->left);
    while (onder->k->right.k!=0)
        onder=&(onder->k->right);
    return onder;
}


template <class Sleutel,class Data>
void Binboom<Sleutel,Data>::write(ostream& os) const{
    if (k!=0){
       std::queue<Binknoop<Sleutel,Data>*, std::deque<Binknoop<Sleutel,Data>*> > q;
       q.push(k);
       while(!q.empty()){
           Binknoop<Sleutel,Data>* nu=q.front();
           write(os,nu);
           os<<" left: ";
           write(os,nu->left.k);
           os<<" right: ";
           write(os,nu->right.k);
           os<<" parent: ";
           write(os,nu->parent);
           os<<std::endl;
           if (nu->left.k!=0)
               q.push(nu->left.k);
           if (nu->right.k!=0)
               q.push(nu->right.k);
           q.pop();
       }
    }
    else{
        write(os,k);
    }
}

#endif

