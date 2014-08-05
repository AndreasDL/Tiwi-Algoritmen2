#ifndef __SAT_H
#define __SAT_H
//SAT: klasse van objecten die 1 SAT-probleem kunnen bevatten.
//Elke rij van de matrix bevat een formule, en alle rijen zijn even lang.
//Merk op: het gaat over disjunctieve formules
// (*this)[i][j] = 0: variabele x_j komt niet voor in F_j
// (*this)[i][j] =-1: variabele x_j komt voor in F_j in de vorm niet(x_j)
// (*this)[i][j] = 1: variabele x_j komt voor in F_j in de vorm x_j
class SAT:public vector<vector<int> >{
public:
    int aantalFormules(){
        return size();
    }
    //er wordt verondersteld dat er minstens 1 formule is
    int aantalVariabelen(){
        return (*this)[0].size();
    }
};
#endif
