#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cmath>
#include <vector>
#include "csv.h"
#include "zoekboom.h"
#include "puntgenerator.h"
using namespace std;

int aantalDatapunten=6000;

int main(int argc, char *argv[]){
    CsvData grafiek("dieptedata",',');
    Puntgenerator dePuntgenerator;
    Zoekboom<double,unsigned int> deZoekboom;
    vector<double> dieptedata;
    int aantal=0;
    for (int i=0; i<aantalDatapunten; i++ ){
        double nupunt=dePuntgenerator.geefpunt();
//        deZoekboom.voegtoe(dePuntgenerator.geefpunt(),i);
        deZoekboom.voegtoe(nupunt,i);
        aantal++;
        if (aantal*50 >= aantalDatapunten){
            dieptedata.push_back(deZoekboom.diepte());
            aantal=0;
        }
    }
    grafiek.voegDataToe(dieptedata);
    return 0;
}
