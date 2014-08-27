#ifndef __SHIFTAND
#define __SHIFTAND

#include "bitpatroon.h"
#include <queue>
typedef unsigned int uint;
typedef unsigned char uchar;
class Shiftand{
public:
//patroonlengte moet kleiner dan de grootte van letterpatroon[] zijn,
//anders wordt een exceptie opgegooid.
    Shiftand(const uchar* patroon,uint _patroonlengte){
        if(_patroonlengte > 256){
            throw _patroonlengte;
        }

        this->patroonlengte = _patroonlengte;
        for(uint i = 0; i < patroonlengte; i++){
            Bitpatroon p(patroon[i]);
            letterpatroon[i] = p;
        }
    };

    /*
    *   T|E|S|T
    *   1|0|0|0
    *   > shift rechts
    *   0|1|0|0
    *   > 1-bit invoegen links
    *   1|1|0|0
    *   > alle bits 1 per 1 vgl'n met huidige char in tekst, result & huidige bitwaarde
    */
    void zoek(queue<const uchar*>& plaats, const uchar* tekst, uint tekstlengte){
        Bitpatroon tabel(uint(0));
        Bitpatroon controle(uint(0));
        controle |= controle.eenbit(patroonlengte-1);

        for(uint i = 0; i < tekstlengte; i++){
            cout<<"Iteration    i = "<<i<<endl;
            cout<<"Huidige letter = "<<tekst[i]<<endl;
            //'SHIFT'
            tabel = tabel.shiftrechts(uint(1));
            tabel|= tabel.eenbit(uint(0));
            cout<<"Tabel na SHIFT = "<<tabel<<endl;

            //'AND'
            //Alle overeenkomsten opslaan in een bitpatroon AND
            //Na overlopen alle tekens AND & tabel = 'AND' voltooid
            Bitpatroon AND(uint(0));
            for(uint t = 0; t < patroonlengte; t++){
                if(Bitpatroon(tekst[i]) == letterpatroon[t]){
                    AND |= AND.eenbit(t);
                }
            }
            cout<<"AND-tabel      = "<<AND<<endl;
            tabel &= AND;
            cout<<"Resultaat na & = "<<tabel<<endl;

            cout<<"Controle       = "<<controle<<endl;
            if(controle.en(tabel)){
                cout<<"Een overeenkomst is gevonden!"<<endl;
                cout<<"Plaats          = "<<i - patroonlengte + 1<<endl;
                plaats.push(&tekst[i-patroonlengte+1]);
            }

            cout<<"Einde iteratie."<<endl<<endl;
        }

    };

private:
    Bitpatroon letterpatroon[256];
    uint patroonlengte;
};


#endif
